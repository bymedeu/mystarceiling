# Hipparcos-2 acquisition work log — 2026-09-12

This consolidates the investigation, owner decisions, implementation, execution,
validation, problems, and remaining concerns from the bright-star supplementation
session. It is a retrospective account; exact acquisition commands and times are
preserved in the linked run log rather than reconstructed here.

## Evidence and executable records

- [Dataset contract and usage](hipparcos2-datasets.md).
- [Python acquisition script](../tools/acquire_hipparcos2.py).
- [Reload wrapper](../tools/download_hipparcos2_catalogue).
- [Focused tests](../tools/test_acquire_hipparcos2.py).
- [Executed acquisition commands and outcomes](../data/hipparcos2/runs/20260912T162859.761589Z/acquisition-log.md).
- [Completed run manifest, hashes, and Gaia checkpoint](../data/hipparcos2/runs/20260912T162859.761589Z/manifest.json).
- [Latest completed run pointer](../data/hipparcos2/latest.json).

The completed run started at 16:28:59.761589 UTC and finished at
16:29:15.189822 UTC on 2026-09-12. Local time was UTC+02:00.

## Investigation and decisions

1. Read the shared and MSC project contracts, README, dataset documentation,
   milestone and decision notes, and relevant acquisition/loader code. Inspected
   working-tree status and recent history. Many unrelated changes already existed;
   they were preserved. Documentation claiming that no loader existed was stale:
   the working tree contained a Gaia loader and rendering pipeline.
2. Read the first rows of the actual Gaia archive and its checkpoint. The first
   source was `418551920284673408`, with G `1.9425238`. The acquisition query
   sorts by G then source ID and requires all nine fields to be non-null.
   Therefore the observed minimum is for the selected qualifying catalogue,
   not an established universal Gaia detection cutoff.
3. Established that direct insertion into the downloading Gaia file would
   invalidate its checkpoint byte positions, ordering, and provenance. Also,
   Hipparcos identifiers, epoch, and photometric bands cannot simply be labelled
   as Gaia values.
4. Researched official Gaia bright-end limitations, Hipparcos releases, source
   schemas, photometric systems, and the published Gaia DR3 cross-match. No
   third-party implementation was copied.
5. The owner directed that Gaia remain primary and unchanged, that Hipparcos
   remain separate, and that fusion be deferred until Gaia acquisition finishes.
   The owner selected Hipparcos-2 (2007) and the official DR3 cross-match.
6. Clarified the initially incomplete magnitude request and the distinction
   between Hp and G. The owner explicitly approved the provisional native
   **Hp < 1.9425238** filter, retaining original photometry and available Gaia
   photometry for later interpretation. No photometric estimates were authorized
   or generated. The resulting script applies this Hp filter to the native
   Hipparcos selection; linked Gaia G values are retained as evidence, not used
   to override that selection.
7. Acquisition implementation proceeded under the approved direction. This
   consolidated documentation update is GREEN mechanical documentation work.

## Problems encountered and handling

| Problem | Handling and result |
| --- | --- |
| Smaller numerical magnitude means brighter; different bands are not directly interchangeable | Explained the scale/band distinction and obtained the explicit provisional Hp selection. |
| Missing or incomplete bright stars cannot all be represented as complete measured Gaia rows | Preserved nine native Hipparcos roles with honest column names; available Gaia fields live in the cross-match file with blanks for missing values. |
| Gaia was still being acquired | Used its existing ID index in read-only mode, bounded by a captured checkpoint; recorded the snapshot. |
| Initial broad Markdown read produced excessive, truncated output, especially from the Gaia acquisition log | Narrowed subsequent reads to relevant files, sections, checkpoint data, and source code. |
| A guessed `gaia_source.hh` path did not exist | Inspected the actual `star.hh`, `stellar_measurements.hh`, and `gaia_csv.cc` definitions. |
| Some official documentation URLs could not be opened through the browser tool | Consulted other official provider pages and retrieved the CDS schema directly with curl. |
| Sandboxed CDS curl request failed with exit code 6, unable to resolve the host | Requested network escalation and retried successfully; did not bypass the approval mechanism. |
| ESA TAP cross-match probe timed out after 40 seconds, exit code 28 | Verified the same official DR3 table at Gaia@AIP and used that partner endpoint, recorded explicitly in each run. No change was made to the Gaia downloader's endpoint. |
| HTTP success alone would not prove a complete query result | Checked VOTable query status including trailing overflow indicators, exact field schema, and a separately queried matching-row count. |
| A published match can be questionable | Kept the original pair, distance, neighbour count and flag; documented the HIP 71681 discrepancy without silently accepting or repairing its identity. |
| Reload failure must not replace usable output | Each invocation writes a new run directory; `latest.json` advances only after successful validation and manifest creation. |

The previously running Gaia acquisition log also showed repeated ESA HTTP 500
responses and timeouts. These were existing acquisition problems observed during
inspection, not failures caused by the Hipparcos work and not repaired here.

## Access probes before the recorded acquisition

These commands were executed during investigation. Their temporary files are
not permanent provenance; the successful acquisition independently downloaded
and preserved its own sources and responses in the completed run directory.
The CDS schema command below first failed in the sandbox, then succeeded after
approval. The Hipparcos source and AIP probe succeeded. The ESA probe timed out.

```sh
curl --fail --silent --show-error --location --max-time 30 https://cdsarc.cds.unistra.fr/ftp/I/311/ReadMe -o /tmp/msc-hipparcos2-ReadMe
curl --fail --silent --show-error --location --max-time 30 https://cdsarc.cds.unistra.fr/ftp/I/311/hip2.dat.gz -o /tmp/msc-hip2.dat.gz
curl --fail --silent --show-error --location --max-time 40 --get --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=votable --data-urlencode 'QUERY=SELECT TOP 1 * FROM gaiadr3.hipparcos2_best_neighbour' https://gea.esac.esa.int/tap-server/tap/sync -o /tmp/msc-hip2-xmatch-probe.xml
curl --fail --silent --show-error --location --max-time 30 --get --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=votable --data-urlencode 'QUERY=SELECT TOP 1 * FROM gaiadr3.hipparcos2_best_neighbour' https://gaia.aip.de/tap/sync -o /tmp/msc-hip2-aip-probe.xml
```

An exploratory Python read counted all 117,955 raw records, verified their
276-character lengths, and printed the 42 rows passing the Hp cutoff. This was
preliminary inspection; the acquisition script subsequently performed its own
complete validation.

## Files created or edited

| File | Work performed |
| --- | --- |
| `tools/acquire_hipparcos2.py` | Added standard-library Python/curl acquisition, native parsing, explicit Hp selection, official cross-match query, local membership checks, validation, logs, hashes, and run publication. |
| `tools/download_hipparcos2_catalogue` | Added executable wrapper beside the Gaia wrapper, with separate paths and explicit `--hp-limit 1.9425238`. |
| `tools/test_acquire_hipparcos2.py` | Added six focused tests. |
| `data/hipparcos2/runs/20260912T162859.761589Z/` | Created the raw source/schema, native and cross-match CSV.gz outputs, ADQL queries, original XML responses, execution log, and manifest. |
| `data/hipparcos2/latest.json` | Published the completed run reference. |
| `data/hipparcos2/.acquire.lock` | Added a lock used to prevent concurrent Hipparcos acquisitions. |
| `.gitignore` | Added only Hipparcos lock and raw-download ignore patterns, preserving prior changes. |
| `docs/hipparcos2-datasets.md` | Recorded selection, schema, usage, provenance, validation, results, and limitations. |
| `docs/README.md` | Linked the new dataset documentation. |
| `docs/hipparcos2-work-log.md` | Added this consolidated retrospective at the owner's request. |

The Gaia archive, acquisition state/index, downloader, existing dataset
documentation, application source, and C++ tests were not edited by this work.
No commit, push, database fusion, photometric conversion, or renderer integration
was performed.

## Execution and validation

Executed from `msc/`:

```sh
python3 -B -m unittest discover -s tools -p 'test_acquire_hipparcos2.py'
chmod +x tools/download_hipparcos2_catalogue tools/acquire_hipparcos2.py
./tools/download_hipparcos2_catalogue
git diff --check
```

The acquisition ran with network approval. Its exact expanded curl commands,
queries, output paths, times, exit codes and outcome are in the linked execution
log. Only one completed full acquisition was run; a second reload was not
executed as a separate demonstration.

All six tests passed:

1. Strict Hp boundary, missing native colour, source epoch and radians-to-degrees
   interpretation on a small fixture.
2. Duplicate HIP identifier rejection.
3. Rejection of a trailing TAP overflow status and preservation of an integer
   source ID larger than the exact integer range of binary64.
4. Exclusion of ID-index entries beyond the checkpoint and unchanged SQLite
   fixture hash after the read-only lookup.
5. Completed-Gaia membership lookup and row-count mismatch rejection on a fixture.
6. Deterministic gzip output and preservation of empty Gaia measurements.

After acquisition, an independent Python inspection verified every file hash
and byte count listed in the manifest, the executing script hash, 42 native
records, 42 cross-match/status rows covering exactly the same HIP IDs, ascending
Hp/HIP ordering, and strict compliance with the selected cutoff. `git diff
--check` reported no whitespace errors. The completed-Gaia path was tested on a
fixture only; the real Gaia archive was still incomplete. No unrelated C++
build or tests were run for this Python/data/documentation task.

## Result and remaining concerns

| Measurement | Verified result |
| --- | ---: |
| Raw Hipparcos-2 rows | 117,955 |
| Rows with finite selected native fields | 117,955 |
| Selected rows, Hp < 1.9425238 | 42 |
| No published best-neighbour row | 40 |
| Published matches absent from local Gaia snapshot | 2 |
| Published matches present locally | 0 |
| Gaia snapshot committed rows | 76,048,576 |
| Gaia acquisition target | 134,217,728 |

The Gaia target is the project's capped qualifying selection, not the full
roughly 1.8-billion-source Gaia release. Completing that target does not mean
downloading every Gaia source.

The two published links returned the following original values:

| HIP | Native Hp | Linked Gaia source ID | Gaia G | Missing Gaia fields |
| --- | ---: | --- | ---: | --- |
| 71681 | 1.2429 | 5877748442128924544 | 20.948614 | pmra, pmdec, parallax |
| 36850 | 1.5811 | 892348694913501952 | 2.9247973 | pmra, pmdec, parallax |

Both fail the current Gaia nine-field completeness requirement. Their absence
from the local selection therefore is not simply evidence that downloading has
not reached them yet. HIP 71681's very large photometric discrepancy remains an
unresolved identity concern. Neither a physical explanation nor a corrected
match has been established. The raw table's match flags are retained; they were
not independently interpreted into quality guarantees.

Remaining limitations and future decision points:

- **Photometric completeness:** the provisional Hp cut does not guarantee all
  stars brighter than Gaia G 1.9425238. Missing G/BP-RP measurements remain blank.
- **Matching completeness:** no published best neighbour does not establish
  absence from Gaia. No independent positional/component-level matching was done.
- **Different epochs:** Hipparcos positions remain at J1991.25, Gaia positions
  at their reported epoch. Radians-to-degrees conversion does not propagate time.
- **Complex sources:** finite fields do not prove a single-star motion model is
  valid. Original solution/component/uncertainty information remains in the raw
  main table; supplementary Hipparcos solution tables were not acquired.
- **Snapshot limits:** local membership relies on the acquisition's existing
  index/checkpoint. The partial Gaia archive was not independently rehashed or
  revalidated in full. Refresh the cross-match report when acquisition finishes.
- **Future fusion:** decide identity handling, ambiguous matches, photometric
  interpretation, epoch propagation and missing-data rules before combining.
  Neither the Gaia loader nor binary format accepts this native Hipparcos file
  as if it were a Gaia file.
- **Operational limits:** each rerun downloads the small source afresh and
  retains old run directories; failed runs also remain for inspection. There is
  no automatic cleanup, paging/resume, retry, or provider fallback. A completed
  Gaia comparison needs a full streaming pass because its temporary index is
  removed by the existing Gaia downloader.
- **Distribution:** raw downloads are local and ignored by Git. Source
  attribution and provider terms are linked in the dataset documentation; no
  distribution or publication action was taken.

## Logging on later runs

The script automatically records acquisition commands, query texts, provider
responses, failures, counts, hashes and snapshot provenance for each invocation.
It does not automatically write an interpretation of new scientific anomalies;
those require inspection and an explicit update to this work log or a new dated
record. Keep the original run logs and manifests unchanged as historical evidence.
