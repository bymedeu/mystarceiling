# Separate Hipparcos-2 bright-star catalogue

Status: acquired and validated on 2026-09-12. Gaia remains the main catalogue.
No database fusion, application loading, epoch propagation, or photometric
conversion is implemented by this acquisition work.

See the [consolidated work log](hipparcos2-work-log.md) for the investigation,
decisions, implementation, executed checks, problems and solutions, and remaining
concerns. Exact per-run acquisition commands remain in each run's execution log.

## Approved selection

Use Hipparcos-2, the 2007 new reduction, distributed as CDS I/311. Require the
nine native counterparts below to be present and finite, and select strictly
**Hp < 1.9425238**. Sort by Hp, then HIP identifier. The owner explicitly approved
this provisional Hp filter after discussing the difference between Hp and G.
The numeric threshold came from the first, brightest row of the current ordered
Gaia selection; it is fixed in the wrapper, not silently recomputed on reload.

This selection is not equivalent to G < 1.9425238 and does not establish
completeness at a Gaia G threshold. The same logarithmic magnitude convention
can be applied to different wavelength bands. Hipparcos Hp and Gaia G measure
different bands; B-V and Gaia BP-RP are different colour indices. No one-to-one
numerical substitution is made.

The selected Hipparcos file retains every qualifying row, including published
Gaia matches. It is not an automatic list of stars to add to Gaia. The separate
cross-match file is evidence for a future decision, not an identity/fusion rule.

## Reload command

From `msc/`:

```sh
./tools/download_hipparcos2_catalogue
```

The wrapper runs [acquire_hipparcos2.py](../tools/acquire_hipparcos2.py) with the
two separate data directories and the explicit Hp limit. It requires Python
3.9 or newer, curl, Linux file locking, network access, and a valid local Gaia
checkpoint/index or completed archive/manifest. It uses only the Python standard
library; no new application dependency is introduced.

Every invocation downloads the roughly 9 MB compressed source again and creates
`data/hipparcos2/runs/<UTC timestamp>/`. Previous runs are retained. Unlike the
large Gaia acquisition, this small download restarts after failure rather than
paging/resuming. A failed run has no completion manifest and does not change
`data/hipparcos2/latest.json`. That pointer advances only after all validation
and output writes succeed. Read it to find the latest completed run.

The script accepts `--hp-limit`, `--root`, `--gaia-root`, and `--tap-endpoint`;
the wrapper forwards additional arguments. Changing the limit is an explicit
selection change, not a photometric conversion. No limit automatically changes
in response to a Gaia download.

## Files and field meanings

Each completed run contains:

- `hip2.dat.gz` and `ReadMe`: the unmodified full source table and CDS schema,
  preserving fields not selected into the small derived catalogue.
- `hipparcos2.csv.gz`: nine complete native fields for the selected records.
- `gaia-crossmatch.csv.gz`: links and any available Gaia measurements for those
  HIP IDs, plus the local-download status. An unmatched HIP has an explicit row
  with empty Gaia fields. Multiple returned links are retained, not collapsed.
- `match-count.xml`, `matches.xml`, and their `.adql` files: original TAP
  responses and exact queries.
- `acquisition-log.md`: executed commands, endpoints, UTC times, exit codes,
  diagnostics, and verified outcome.
- `manifest.json`: selection, units, counts, hashes, script hash, and the local
  Gaia snapshot used for comparison.

| Hipparcos CSV field | Meaning |
| --- | --- |
| `hip_id` | HIP identity, never a fabricated Gaia source ID |
| `ref_epoch` | Julian epoch 1991.25 |
| `ra`, `dec` | ICRS angles in degrees, converted from source radians |
| `pmra`, `pmdec` | Proper motions in mas/year; RA component includes cos(dec) |
| `parallax` | Milliarcseconds; no positive-parallax cut |
| `hp_mag` | Native Hipparcos Hp magnitude |
| `b_v` | Native B-V colour index |

These are nine analogous roles, not nine interchangeable Gaia measurements.
The different header intentionally prevents the Gaia-specific parser from
silently interpreting Hipparcos photometry as G/BP-RP. The source's solution
types, component counts, uncertainties, V-I and other information remain in
`hip2.dat.gz`. No scientific quality cut or replacement of complex solutions
with simple ones has been applied.

In the cross-match CSV, the nine `gaia_*` fields retain the original Gaia DR3
values, including G and BP-RP when available. Gaia positions remain at their
own reference epoch. Blank means missing, not zero or an estimate. The file also
retains published angular distance in arcseconds, neighbour count and `xm_flag`.
Use `hip_id` to relate the two outputs; no positional match is invented locally.

## Cross-match and local-presence evidence

The script queries `gaiadr3.hipparcos2_best_neighbour` and joins the linked
`gaiadr3.gaia_source` rows. Gaia@AIP is the configured partner archive: the ESA
endpoint timed out during access verification, whereas AIP responded. The exact
endpoint is recorded per run and can be selected explicitly on reload.

The official table selects positional best neighbours; it does not guarantee
every match is correct or that every missing match represents a star absent
from Gaia. Its match flags are preserved without assigning a new interpretation.

| Status | Evidence |
| --- | --- |
| `matched_locally_downloaded` | The published Gaia source ID occurs in the local snapshot |
| `matched_not_locally_downloaded` | A published link exists, but its Gaia ID does not occur in that snapshot |
| `no_published_best_neighbour` | No row was returned from the official best-neighbour table for this HIP ID |

For ongoing acquisition the script reads the existing SQLite index with
`mode=ro` and a read transaction. It bounds membership by the recorded checkpoint
row count, excluding index entries from a later, uncommitted append. It records
the checkpoint and its SHA-256. It does not independently revalidate every byte
of the partial Gaia archive. Missing/inconsistent state is an error, not absence.

Once Gaia is complete, its temporary index is removed by the existing downloader.
The Hipparcos script then streams the completed CSV.gz, checks the header, gzip
integrity, row widths, size and count against the Gaia manifest, and checks IDs.
This requires one full pass through Gaia. It does not merge or rewrite Gaia.
Refresh the report after Gaia finishes before deciding which stars to supplement.

## Verified result on 2026-09-12

- Source rows: **117,955**, all with finite values in the selected native roles.
- Provisional Hp-selected rows: **42**.
- No published best-neighbour match: **40**.
- Published links absent from the local Gaia snapshot: **2**.
- Published links present locally: **0**.
- Local Gaia snapshot: **76,048,576** committed records, still incomplete.

Both matched Gaia sources lack `pmra`, `pmdec`, and `parallax`, so they fail the
current nine-required-field Gaia selection, regardless of download progress.

| HIP | Hp | Published Gaia source ID | Gaia G |
| --- | ---: | --- | ---: |
| 71681 | 1.2429 | 5877748442128924544 | 20.948614 |
| 36850 | 1.5811 | 892348694913501952 | 2.9247973 |

The large discrepancy for HIP 71681 is an unresolved cross-match concern.
These are the returned values, not a claim that they measure the same physical
component or that the difference is explained by the bands. Neither link was
used to replace Hipparcos data or discard a selected star.

Validation checks include source row length/count, unique HIP IDs, complete
finite native fields, coordinate ranges, strict Hp selection, TAP query status
including trailing truncation indicators, independently queried match count,
requested HIP membership, unique match pairs, gzip/CSV round trips, and file
hashes. Identifiers remain integers/text rather than passing through floating
point. Gzip outputs use deterministic headers.

Run focused tests with:

```sh
python3 -B -m unittest discover -s tools -p 'test_acquire_hipparcos2.py'
```

## Sources and attribution

- [CDS Hipparcos-2 schema and source](https://cdsarc.cds.unistra.fr/ftp/I/311/ReadMe),
  van Leeuwen (2007), Astronomy & Astrophysics 474, 653; corrected CDS distribution.
- [Gaia DR3 Hipparcos-2 best-neighbour documentation](https://gaia.aip.de/metadata/gaiadr3/hipparcos2_best_neighbour/),
  including algorithm references, attribution, and distribution terms.
- [Gaia source field documentation](https://gaia.aip.de/metadata/gaiadr3/gaia_source/).

This dataset uses ESA Gaia data processed by DPAC, accessed through Gaia@AIP,
and the Hipparcos new reduction distributed by CDS. Preserve those source
attributions and consult the linked provider terms when distributing data.
