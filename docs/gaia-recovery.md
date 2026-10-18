# Authenticated Gaia acquisition recovery — 2026-10-18

Acquisition is complete. Canonical [selection, command and version](../data/gaia-dr3/acquisition-log.md)
and [final metadata/verification limits](gaia-datasets.md#verified-acquisition-evidence)
replace transient job and polling history.

## Cause and outcome

Many anonymous retries failed at the 60-second execution quota, including reduced
batches. A pending-job diagnostic rejected 600 seconds; advertised TAP capability
was not proof of anonymous allowance. Authenticated recovery accepted 600 seconds.
A bounded run committed 250,000 rows, verified the new gzip tail, strict ordering,
every new ID's SQLite ordinal and checkpoint/archive/index agreement before
background continuation. The transient service used Restart=no, with no login or
reboot persistence. The later completed acquisition supersedes intermediate counts.
The owner logged in locally; agents used only an external jar path.

## Durable recovery contracts

- `--cookie-jar` or `MSC_GAIA_COOKIE_JAR` names a private, owned regular file outside
  the repo. curl disables implicit configuration and keeps TLS checks; saved
  submission headers redact Set-Cookie. No credentials belong in logs or commands.
- `--replace-aborted-job URL` must match the checkpoint and live timeout-aborted
  job. Replacement uses the unchanged committed cursor, starts PENDING, verifies
  authenticated owner, exact query/MAXREC and duration readback before saving/RUN.
  Failed verification attempts deletion of only the new candidate and preserves
  the committed checkpoint. It is a one-shot option; omit it on later resumes.
- A lost RUN response is reconciled from saved job metadata, with no blind RUN
  retry. DELETE does not follow redirects; mutations are attempted once. Reads
  have three bounded attempts; polling has a duration-plus-300-second deadline.
  Authentication/quota/query failure stops rather than falling back to anonymous
  execution or creating a restart loop. Sessions are read, never refreshed.
- Selection, float32 magnitude cursor reconstruction, 4096-row gzip members, row
  target, compressed cap and SQLite duplicate checks are unchanged. Short
  authenticated results, overflow and truncated committed archives fail. Resume
  reconciles uncommitted append/index tails without discarding committed rows.
- Completed remote jobs are deleted only after next-batch checkpoint commit;
  cleanup failure stops with valid data and may leave an orphan. No unrelated
  jobs are deleted; the final target job is retained.
- Stop any downloader before reverting code or logging out. Never restore an
  older checkpoint over validated data. Session, storage, disk, network and
  query-duration limits can still affect a future acquisition.

## Evidence and limitations

The recovery implementation at `9b10552` passed 26 focused offline Python tests
(16 Gaia, 10 Hipparcos): synthetic cookie fixtures, mocked network, exact queries,
quota/session refusals, redaction, lost responses, crash/replay paths, ordering,
duplicates, byte caps, truncated archives and cleanup failures. The live bounded
batch separately demonstrated real committed progress. Final archive verification
has the distinct scope linked above; recovery alone did not prove completion.
No C++ or scientific-selection change was part of recovery; the catalogue
[understanding checkpoint](project-state.md#required-understanding-checkpoint) stays open.

The existing downloader still emits verbose Markdown. Logger changes remain
future implementation scope; this cleanup changes documentation only. There is
no active acquisition accumulating logs.

References: [ESA programmatic access](https://www.cosmos.esa.int/web/gaia-users/archive/programmatic-access#Sect_2_ss2.1),
[ESA quotas](https://www.cosmos.esa.int/web/gaia/faqs),
[TAP duration negotiation](https://ivoa.net/documents/TAP/20190927/REC-TAP-1.1.html),
[curl options](https://curl.se/docs/manpage.html).
