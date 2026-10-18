# Hipparcos-2 bright-star supplement

Status: prepared and validated on 2026-09-12. Gaia remains MSC's primary stellar
catalogue. Hipparcos supplies bright stars that are not represented by a complete
record in its recorded Gaia snapshot. The latest prepared run uses the previous
76,048,576-row checkpoint, not the now-completed 134,217,728-row archive. Its
membership/overlap results have not been revalidated against final Gaia;
regeneration is not authorized by the 2026-10-18 documentation update.

See the [consolidated work log](hipparcos2-work-log.md) for the investigation,
implementation, commands, failures, and validation evidence. Exact executed
network commands remain in each run's immutable acquisition log.

## Implemented preparation policy

Use Hipparcos-2, the 2007 new reduction distributed as CDS I/311. A source row
must contain finite values for these ten native fields: HIP identifier, reference
epoch, RA, Dec, proper motion in RA and Dec, parallax, Hp magnitude, B-V, and V-I.
The source's J1991.25 position is converted from radians to degrees; it is not
propagated to another epoch.

Estimate Gaia EDR3 G offline from Hipparcos photometry:

```text
G - Hp = -0.02392 - 0.4069(B-V) + 0.04569(B-V)^2 - 0.0452(B-V)^3
```

Use that lower-scatter relation for `-0.25 < B-V <= 1.4`. Outside that
conservative range, use the documented V-I relation when `-0.25 < V-I < 5.0`:

```text
G - Hp = 0.01546 - 0.4308(V-I) - 0.01872(V-I)^2
```

The published population scatters are 0.02417 and 0.08191 magnitude,
respectively. The derived G value is an estimate and remains labelled with its
method; it never replaces the native measurements.

Select strictly `estimated_g < 1.9425238`, the current lower boundary of MSC's
complete nine-field Gaia selection. Sort by estimated G and then HIP identifier.
Cross-match only those bright candidates through the published Gaia DR3
Hipparcos-2 best-neighbour table.

For each candidate, prefer an already downloaded Gaia row only when the published
neighbour is complete under MSC's nine-field contract and its source ID is proven
to be within the recorded local Gaia checkpoint. Otherwise retain the Hipparcos
record. This is an offline preparation rule; the application is still intended
to consume one normalized runtime catalogue.

## Reload command

From `msc/`:

```sh
./tools/download_hipparcos2_catalogue
```

The wrapper invokes [acquire_hipparcos2.py](../tools/acquire_hipparcos2.py) with
separate Hipparcos and Gaia data roots and the explicit derived-G limit. It needs
Python 3.9 or newer, curl, Linux file locking, network access, and a valid local
Gaia checkpoint/index or completed Gaia archive/manifest. It uses the Python
standard library and adds no application dependency.

Every invocation downloads the roughly 9 MB compressed source and creates
`data/hipparcos2/runs/<UTC timestamp>/`. Previous completed runs remain immutable.
A failed run has no completion manifest and cannot update
`data/hipparcos2/latest.json`; the pointer advances only after complete
validation and publication.

The script accepts `--g-limit`, `--root`, `--gaia-root`, and `--tap-endpoint`.
Changing the limit or conversion policy changes the derived dataset and must be
reviewed and recorded.

## Files and fields

Each completed run contains:

- `hip2.dat.gz` and `ReadMe`: the unmodified full source table and CDS schema;
- `hipparcos2.csv.gz`: all selected candidates, including derivation provenance;
- `gaia-crossmatch.csv.gz`: the published neighbour evidence and local status;
- `hipparcos2-supplement.csv.gz`: the Hipparcos rows that remain after the
  approved record-resolution policy;
- `match-count.xml`, `matches.xml`, and matching `.adql` files: original TAP
  responses and exact queries;
- `acquisition-log.md`: commands, endpoints, UTC times, exit codes, and outcome;
- `manifest.json`: formulae, ranges, units, counts, hashes, script hash, and Gaia
  snapshot provenance.

| Field | Meaning |
| --- | --- |
| `hip_id` | HIP identity; never fabricated as a Gaia source ID |
| `ref_epoch` | Julian epoch 1991.25 |
| `ra`, `dec` | ICRS angles in degrees, converted from source radians |
| `pmra`, `pmdec` | Proper motions in mas/year; RA includes cos(dec) |
| `parallax` | Milliarcseconds; negative values would remain valid measurements |
| `hp_mag` | Native Hipparcos Hp magnitude |
| `b_v`, `v_i` | Native Hipparcos colour indices used by the documented relations |
| `estimated_g` | Derived Gaia-like G selection value |
| `g_estimation_method` | `edr3_hp_bv` or `edr3_hp_vi_fallback` |
| `resolution_reason` | Why the Hipparcos row remains in the supplement |

The different source schema intentionally prevents the Gaia parser from treating
Hipparcos values as native Gaia measurements. Source flags, errors, solution
types, and all unselected fields remain available in `hip2.dat.gz`.

## Cross-match and local-presence evidence

The script queries `gaiadr3.hipparcos2_best_neighbour` and joins the linked
`gaiadr3.gaia_source` row through Gaia@AIP. The ESA endpoint was unavailable
during development; the partner endpoint and every query are recorded.

The published table is probabilistic positional best-neighbour evidence. It does
not guarantee that a match is physically correct, and absence does not prove a
star is absent from Gaia. MSC does not invent a local positional match.

| Status | Evidence |
| --- | --- |
| `matched_locally_downloaded` | Published Gaia source ID is within the recorded local checkpoint |
| `matched_not_locally_downloaded` | Published link exists, but its ID is outside that snapshot |
| `no_published_best_neighbour` | Official table returned no row for the HIP ID |

During an incomplete Gaia acquisition, membership uses the existing SQLite ID
index in read-only mode and is bounded by the committed checkpoint row count.
The manifest records that checkpoint and its SHA-256. When Gaia is complete, the
script can instead stream and validate the completed CSV-gzip archive.

## Verified result: run `20260912T191816.016104Z`

| Measurement | Result |
| --- | ---: |
| Hipparcos-2 source rows | 117,955 |
| Rows complete in all ten native fields | 117,955 |
| Hp/B-V primary estimates | 106,255 |
| Hp/V-I fallback estimates | 11,649 |
| Rows outside both supported colour ranges | 51 |
| Candidates with derived G below 1.9425238 | 63 |
| No published best-neighbour | 60 |
| Published neighbours incomplete under MSC's Gaia contract | 2 |
| Complete candidates already represented in local Gaia | 1 |
| Final Hipparcos supplement rows | 62 |
| Gaia checkpoint rows used for resolution | 76,048,576 |

HIP 3179 is the first Gaia row in the local catalogue and is excluded from the
supplement because its published neighbour is complete and locally present. HIP
71681 and HIP 36850 have published neighbours, but both lack Gaia proper motion
and parallax, so their Hipparcos rows remain. HIP 71681's large Hp/G discrepancy
remains a documented identity concern rather than being silently accepted.

Validation covers source row count and widths, unique integer identifiers,
finite required fields, coordinate ranges, formula selection boundaries,
strict derived-G selection, VOTable status and row count, match membership,
ambiguous best-neighbour rejection, checkpoint-bounded local membership,
deterministic gzip output, hashes, and resolution outcomes.

Run focused tests with:

```sh
python3 -B -m unittest discover -s tools -p 'test_acquire_hipparcos2.py'
```

## Limitations

- Estimated G is derived photometry, not a Gaia measurement.
- Reported fit scatter describes a population and is not a per-star error bar.
- No position propagation or independent identity matching is performed.
- No published best neighbour does not prove that a source is absent from Gaia.
- Resolution is tied to the recorded local Gaia snapshot and must be regenerated
  after that snapshot changes.
- A future normalized runtime record still needs its approved C++ identity,
  provenance, and source-measurement representation.

## Sources and attribution

- [CDS Hipparcos-2 schema and source](https://cdsarc.cds.unistra.fr/ftp/I/311/ReadMe),
  van Leeuwen (2007), Astronomy & Astrophysics 474, 653.
- [Gaia EDR3 photometric relationships](https://gea.esac.esa.int/archive/documentation/GEDR3/Data_processing/chap_cu5pho/cu5pho_sec_photSystem/cu5pho_ssec_photRelations.html).
- [Gaia DR3 Hipparcos-2 best-neighbour documentation](https://gaia.aip.de/metadata/gaiadr3/hipparcos2_best_neighbour/).
- [Gaia DR3 source field documentation](https://gaia.aip.de/metadata/gaiadr3/gaia_source/).

Preserve ESA/DPAC, Gaia@AIP, CDS, and catalogue-publication attribution when
redistributing derived data.
