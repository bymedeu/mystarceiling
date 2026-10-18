# Authenticated Gaia acquisition recovery — 2026-10-18

Owner-approved YELLOW scope: add external session support and bounded recovery to
`tools/acquire_gaia.py`, focused Python tests and operational records. No C++, M2
implementation, scientific selection change, manual checkpoint repair, account
creation or credential entry by the agent. The user performed login locally.

## Cause and recovery contract

The saved 250,000-row job was ABORTED after 60.096 seconds, with
`executionDuration=60`. Reducing earlier batches to 375,000 and 250,000 rows did
not advance the checkpoint. A diagnostic pending anonymous job rejected 600
seconds explicitly as greater than its user's 60-second quota. The generic TAP
capability advertising 600 seconds was not proof of an anonymous allowance.

The downloader now accepts an external, private, owned regular cookie jar using
`--cookie-jar` or `MSC_GAIA_COOKIE_JAR`. Cookie contents and passwords must never
be placed in commands, logs or repository files. The session helper and jar stay
outside the repository; the owner's corrected helper was preserved. curl reads
cookies by path, disables implicit curl configuration and retains TLS checking.
Raw response headers are held in a private temporary directory; `Set-Cookie`
is removed before submission headers are saved in the acquisition directory.

`--replace-aborted-job URL` must match the checkpoint's started job and live
metadata must confirm a timeout abort. The replacement is submitted PENDING
from the same committed cursor. Its authenticated owner, exact query and MAXREC
are checked; the requested execution duration (default 600 seconds) must be read
back before saving its job URL and issuing RUN. Failed verification attempts
cleanup of only that new candidate and leaves the old committed checkpoint.
No blind RUN retry occurs after a lost response: resume checks the saved job's
phase and adopts a verified queued/running/completed job. DELETE never follows
redirects to the collection; mutations are attempted once. Read transfers retain
three bounded attempts. Authenticated polling has a duration-plus-300-second
observation deadline. Session/authentication failures stop acquisition.

The original nine mandatory fields, completeness policy, brightness/source-ID
ordering, float32 magnitude cursor reconstruction, 134,217,728-row target,
10,000,000,000-byte compressed cap, 4096-row gzip members and SQLite duplicate
checks remain in place. A short authenticated result fails instead of declaring
source exhaustion. Overflow fails before committing unexpected rows. An archive
shorter than the committed size fails instead of being padded. Existing resume
truncation and ID-index reconciliation recover uncommitted append/index tails.

Completed authenticated jobs are deleted only after the next-batch checkpoint
has committed, to release remote result storage. Cleanup failure stops with valid
committed data and may leave an orphan requiring deliberate cleanup. No unrelated
account jobs are deleted; the final target job is retained.

## Commands and operational verification

The first recovery run uses the private jar path in `MSC_GAIA_COOKIE_JAR` outside
the repository; the password is never supplied to the downloader:

```sh
python3 -B tools/acquire_gaia.py \
  --root data/gaia-dr3 --target-rows 134217728 --max-bytes 10000000000 \
  --execution-duration 600 --max-batches 1 \
  --replace-aborted-job https://gea.esac.esa.int/tap-server/tap/async/7e8a4bf9-cae9-11f1-9670-bc97e148b76b-O
```

Before recovery, the host service had MainPID=0 and the acquisition lock was
free. Baseline: 76,048,576 committed rows, 4,878,012,154 bytes; cursor
(15.981961250305176, 5938936058012995200). Archive size and indexed boundary ID
matched. Exact generated ADQL and HTTP command outcomes are appended by the
existing downloader to the [local execution log](../data/gaia-dr3/acquisition-log.md).
Its pre-existing changes are preserved and excluded from the recovery commit.

The authenticated pending job accepted 600 seconds and completed. The one-batch
command exited successfully at 2026-10-18 12:56:31 UTC. It committed 250,000 rows
and 15,959,342 compressed bytes, reaching **76,298,576 rows / 4,893,971,496 bytes**,
cursor (15.986473083496094, 5314559860260730368), batch 27 with no pending job.
Verification decompressed only the new gzip tail, checked its 250,000 strictly
ordered rows and every new ID's exact SQLite ordinal, and counted 76,298,576 total
indexed IDs. Archive length equalled checkpoint bytes. The original execution-log
prefix and corrected external helper were verified unchanged.

Only after that verification and a fresh host concurrency check, the following
transient user service was started (the external jar path was supplied to
`--setenv`; its contents were never inspected):

```sh
systemd-run --user --unit=msc-gaia-acquisition-auth \
  --description='MSC authenticated Gaia acquisition' \
  --property=Type=exec --property=Restart=no \
  --working-directory=/home/amade/projects/portfolio/msc \
  --setenv=MSC_GAIA_COOKIE_JAR="$MSC_GAIA_COOKIE_JAR" \
  --setenv=PYTHONUNBUFFERED=1 \
  /home/amade/projects/portfolio/msc/tools/download_gaia_catalogue
```

The existing failed `msc-gaia-acquisition-750k.service` was left untouched. The new
unit is independent of the terminal and transient; nothing was enabled for login
or reboot startup. Inspect or stop it with:

```sh
systemctl --user show msc-gaia-acquisition-auth.service -p ActiveState -p SubState -p MainPID -p Restart -p UnitFileState
journalctl --user -u msc-gaia-acquisition-auth.service -n 8 --no-pager
systemctl --user stop msc-gaia-acquisition-auth.service
```

Server EXECUTING/COMPLETED phases alone are not local progress. The verified
first-batch advancement above is distinct from subsequent background observations.

At 12:59:46 UTC the service committed a second 250,000-row batch, reaching
76,548,576 rows / 4,909,929,854 bytes. Its archive length and indexed cursor
boundary matched the checkpoint. At 12:59:48 UTC it was executing batch 28;
`ActiveState=active`, `UnitFileState=transient`, `Transient=yes`, `Restart=no`.
This establishes background committed advancement, not only process liveness.

## Validation, rollback and limitations

```sh
python3 -B -W ignore::ResourceWarning -m unittest discover -s tools -p 'test_acquire_*.py'
git diff --check
```

All 26 focused Python acquisition tests passed (16 Gaia recovery, 10 Hipparcos).
The offline Gaia tests use synthetic cookie fixtures and mock all network calls.
They check exact ADQL, secret-header redaction, genuine fixture archive/index
advancement, anonymous/insufficient-quota/query/MAXREC refusals, live-old-job
refusal, single submission attempts, saved pending revalidation, lost-RUN-response
adoption, index-before-append and append-before-JSON interruptions, committed
prefix replay, byte-cap failure, duplicates, overflow, short results, expired
sessions, cleanup failures, truncated archives and cookie permissions. Resource
warnings from existing process-lifetime resource ownership are suppressed in the
single-process test harness; no C++ changes require C++ suite repetition.

Stop the service before reverting code. Reverting source must never restore an
older checkpoint or discard validated rows. After replacement, omit the one-shot
replacement flag. On authentication/quota/query failure stop and investigate;
never fall back to anonymous execution or create an auto-restart loop. Logout
only after stopping the downloader; the user-run external helper can POST logout
and remove its own jar. The downloader reads but does not refresh the jar.

A live accepted duration proves at least that allowance, not the account's
maximum quota or the success of future batches. Later queries may still exceed
600 seconds; account storage limits, session lifetime, disk space and network
failures remain operational limits. The catalogue remains incomplete until final
gzip/count/hash checks create the final archive and manifest. No full acquisition
or full-archive revalidation is claimed by the one-batch recovery evidence.

Official references: [ESA login/logout](https://www.cosmos.esa.int/web/gaia-users/archive/programmatic-access#Sect_2_ss2.1),
[ESA quota/encoding FAQ](https://www.cosmos.esa.int/web/gaia/faqs),
[TAP duration negotiation §5.1.2](https://ivoa.net/documents/TAP/20190927/REC-TAP-1.1.html),
and [curl cookie/stdin options](https://curl.se/docs/manpage.html).
