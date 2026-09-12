# Gaia dataset acquisition

Status: 76,048,576 rows downloaded and validated; checkpoint size remains
4,878,012,154 bytes. On 2026-09-12 batch size was reduced from 1,500,000 to
750,000 rows and `msc-gaia-acquisition-750k.service` ran once without automatic
restart. ESA async submission for batch 26 timed out after 300000 ms with zero
bytes received (curl exit 28). No job was recorded. The service is stopped/failed
and acquisition remains incomplete. Last updated 2026-09-12.
This record supersedes earlier proposed caps and pending ten-field decisions in
objectives-and-decisions.md. No application loader or star class is implemented.

## Selected scope

Use Gaia DR3. The current nine required fields are source_id, ref_epoch, ra,
dec, pmra, pmdec, parallax, phot_g_mean_mag, and bp_rp. The owner removed all
_error fields and radial_velocity; do not retain them as nullable columns.
Proper motion (pmra/pmdec) remains mandatory. Calculations may assume zero radial
motion; this is an approximation, not a measurement.

The owner confirmed retaining bp_rp and all nine selected fields.
Separate BP/RP magnitudes are excluded. Separate magnitudes
provide more photometric information, but do not improve the precision of that
same colour difference or establish a visible rendering improvement.

Omit ruwe, astrometric_params_solved, duplicated_source, random_index, and raw
fluxes. Validate unique source_id values in the acquired catalogue. The Gaia
processing flag duplicated_source is not a duplicate-row filter; its definition
says only one source identifier was retained after processing duplication.

A name remains optional for future enrichment. The owner confirmed excluding
designation. Keep catalogue identity unchanged; release/source_id can identify a
future name mapping. No name-file format is selected.

The application catalogue target is 134,217,728 records (2^27), with a hard
10 Go safety limit interpreted as 10,000,000,000 bytes for one compressed
CSV.gz. At 76,048,576 validated records, the archive occupies 4,878,012,154
bytes; that measured density estimates 2^27 records at about 8.61 GB. The next
power, 2^28, estimates to about 17.21 GB and cannot fit. The estimate is not a
guarantee: acquisition stops before exceeding the byte limit if later rows
compress substantially worse.

Retain exactly the brightest 2^27 qualifying records when they fit. Gzip members
contain at most 4096 records.
Temporary downloaded batches and the duplicate-ID validation index require
additional working disk space; the index is removed after successful completion.

Only sources meeting the agreed completeness policy are eligible.
No positive-parallax,
RUWE, stellar-classification, or other scientific quality cut has been approved.
Gaia records are sources; a strict star-only classification filter is not implied.

Sort ascending by phot_g_mean_mag, then ascending source_id. Smaller G magnitude
means brighter. Every smaller dataset is a prefix of the same ordered source
selection. Preserve identical values for shared records.

Benchmark counts chosen under the owner's delegation:

| Records | Purpose |
| ---: | --- |
| 4096 | Fast development runs |
| 65536 | Initial scaling |
| 262144 | Approximately 250k-star benchmark |
| 1048576 | Million-source benchmark |
| 134217728 | Application catalogue (2^27), subject to the 10,000,000,000-byte safety limit |

Exact powers of two are reproducible test sizes, not a claimed optimal hardware
alignment. Source coverage is brightness-selected, not an unbiased sky sample.

## Batch and merge requirements

Acquisition of the selected scope may use multiple bounded queries with
disjoint ranges covering that scope. Every batch must be checked for
truncation and transport failure. Validate
schema, ordering, duplicate IDs, row counts, and subset nesting before declaring
success. Record release, exact selection/query, retrieval details, and checksums.
Record the selected scope explicitly; do not describe a limited selection as
the complete qualifying Gaia catalogue.

Globally sorted output needs either ordered nonoverlapping magnitude/ID ranges
or a real merge of sorted batches. Concatenating arbitrary sky-region batches
does not establish global brightness order. Repeated headers must not become rows.
Keep resumable progress so a failure does not require downloading completed work.
The acquisition utility implements these checks and resumable keyset paging.

The owner prefers one brightness-ordered catalogue, taking prefixes for the
benchmark sizes above. This supersedes the disjoint-increment proposal.
CSV with gzip compression is used for acquisition. Distribution remains open:
a 10 GB file exceeds the GitHub Free/Pro LFS per-file limit; LFS is not configured.
No LFS installation, configuration, commit, or push has been authorized here.
Retain the current usable source until the replacement is verified.

## Acquisition and resume command

From the repository root, run:

```sh
./tools/download_gaia_catalogue
```

On this computer the command resumes from the committed checkpoint. On a fresh
checkout it creates the acquisition state and starts from zero. It requires
Python 3, curl, network access to the ESA Gaia TAP service, and enough disk for
the archive plus temporary validation files. The checkpoint records both parts
of the last ordered key: `phot_g_mean_mag` and `source_id`. A `gaia-dr3.csv.gz.partial`
file is incomplete; successful verification renames it to `gaia-dr3.csv.gz` and
creates `manifest.json`.

## Acquisition command record

When acquiring the catalogue, record the exact executed commands in a Markdown
file, including complete ADQL queries or the exact contents of referenced query
files, endpoint, selected columns, filters, ordering, limits, and output paths.
Include any batch/merge/compression commands and their outcomes. Distinguish
commands actually executed from proposed update commands. Record source release,
retrieval date, counts, and checksums so future updates can be reproduced.
See [execution log](../data/gaia-dr3/acquisition-log.md) for exact executed
commands and [download utility](../tools/acquire_gaia.py) for resumable acquisition.
The final catalogue is not complete until manifest.json exists alongside
gaia-dr3.csv.gz; a .partial file is unfinished. Benchmark CSV hashes are recorded
in the completion manifest. No application loader is implemented.

## Volume and storage evidence

The main Gaia DR3 source table contains roughly 1.8 billion sources; around
1.46 billion have full position/parallax/proper-motion solutions. About 33.8
million have radial velocities. None is the exact count for MSC's intersection
of required fields.

Local disk currently has approximately 162 GiB available. /tmp is a 7.6 GiB
memory filesystem and cannot stage the full catalogue.

A historical 1000-row sample of the then-selected 12 columns used 153904 bytes as CSV and 69556
bytes with gzip. Simple extrapolation to 1.46 billion rows gives roughly 225 GB
of CSV, before indexes, sorting workspace, extra columns, or duplicate copies.
This is a scale illustration, not a measured total or guaranteed compression
ratio. It shows why the all-source request needs a storage plan before transfer.

Batching resolves per-query row limits; it does not eliminate total disk or
sorting requirements. Compression, a different output format, or another disk
may be needed. No choice has been silently made.

## Additional-field review

The main source table has 153 columns. Full definitions are linked below.
Candidate groups for owner discussion, not approved additions:

| Group | Example fields | Potential MSC use |
| --- | --- | --- |
| Position/motion uncertainty | ra_error, dec_error, pmra_error, pmdec_error | Assess historical reconstruction confidence. |
| Correlations | ra_dec_corr, ra_pmra_corr, pmra_pmdec_corr and related correlations | More complete uncertainty propagation. |
| Astrometric reliability | ruwe, astrometric_params_solved, duplicated_source | Investigate unreliable or complex source solutions. |
| Separate colour bands | phot_bp_mean_mag, phot_rp_mean_mag | Retain more photometric information than their difference. |
| Photometric diagnostics | phot_*_mean_flux, phot_*_mean_flux_error, phot_bp_rp_excess_factor | Inspect brightness/colour reliability. |
| Variability | phot_variable_flag; separate variability tables | Date-dependent intrinsic brightness rather than static mean brightness. |
| Stellar properties | teff_gspphot, logg_gspphot, mh_gspphot | Future physical properties or source inspector. |
| Distance and dust | distance_gspphot, ag_gspphot, ebpminrp_gspphot | Estimated distance and interstellar extinction; not terrestrial light pollution. |
| Classification | classprob_dsc_combmod_star and related probabilities | Optional source selection; thresholds would be new decisions. |
| Other sky coordinates | l, b, ecl_lon, ecl_lat | Galactic/ecliptic views or diagnostics. |
| Processing metadata | solution_id, observation counts, scan-direction/IPD/RV diagnostics | Detailed catalogue troubleshooting. |
| Additional-product flags | non_single_star, has_xp_continuous, has_epoch_photometry and related flags | Locate richer products; flags do not contain those products. |
| Sampling and labels | random_index, designation | Other sampling strategies or readable catalogue labels. |

The table above is a reference to discussed candidates, not the selected schema.
The Selected scope section is authoritative. Variability is still an unselected
future possibility; no extra quality or classification cuts are approved.

A selected-column archive and a runtime star representation need not have the
same fields. Retaining a measurement for later work does not require computing
with it or transferring it to the GPU now. This is a conceptual distinction,
not a selected memory layout.

## Remaining decisions

Distribution of the capped catalogue remains open. Fields, brightness ordering,
and the 10 Go storage cap are settled. The name-enrichment file is an owner proposal; its concrete
format is not selected. No loader or star class has been implemented.

## References

- [Gaia DR3 contents](https://www.cosmos.esa.int/web/gaia/dr3).
- [Main source fields](https://gaia.aip.de/metadata/gaiadr3/gaia_source/).
- [Official source definitions](https://gea.esac.esa.int/archive/documentation/GDR3/Gaia_archive/chap_datamodel/sec_dm_main_source_catalogue/ssec_dm_gaia_source.html).
- [Astrophysical parameters](https://gaia.aip.de/metadata/gaiadr3/astrophysical_parameters/).
- [Variability summary](https://gaia.aip.de/metadata/gaiadr3/vari_summary/).
- [ESA extraction guide](https://www.cosmos.esa.int/web/gaia-users/archive/extract-data).

## Verified acquisition evidence

The acquisition has committed 76,048,576 records in 4,878,012,154 compressed
bytes. All committed rows passed the nine-field, finite-number, coordinate-range,
unique-ID, and strict G-magnitude/source-ID ordering checks. The continuation key
is (15.981961250305176, 5938936058012995200). This remains an incomplete prefix
until the exact target is reached and the final integrity checks create the
manifest.
