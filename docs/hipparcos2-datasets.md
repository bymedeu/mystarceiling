# Hipparcos-2 bright-star supplement

Status: the 2026-10-28 bright-range repair supersedes the narrow September
selection. The owner authorized this bounded source-selection correction. It
keeps the accepted photometric estimates, colour preparation, J2016 propagation,
binary schema and Hipparcos-first capacity policy. Historical run results below
remain historical evidence, not the current coverage claim.

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

Select **inclusive `estimated_g <= 6` by default**, matching the viewer's G-like
limiting magnitude. `--g-limit` overrides that inclusive limit explicitly. The
old 1.9425238 value was only the observed minimum in the usable Gaia subset;
it never established a universal Gaia detection threshold or filled brighter-sky
holes above that value.

Cross-match the selected candidates using the published Gaia DR3 Hipparcos-2
best-neighbour table. Keep every usable native candidate as a fallback in the
prepared supplement. A complete published Gaia neighbour inside the declared
bright range receives a preference only when its ID is verified in local Gaia.
The build applies that preference only if the Gaia record actually survives its
accepted prefix. Missing, incomplete, nonlocal or prefix-truncated neighbours
therefore cannot silently remove a bright Hipparcos source.

A complete published neighbour outside the declared range cannot establish
coverage of the primary. Retain Hipparcos and exclude that published Gaia ID
from the builder's fill, under the existing published-crossmatch deduplication
policy. This is a range/precedence rule, **not a new photometric identity test**:
no positional matching or arbitrary magnitude-difference threshold is invented.
The association remains probabilistic; a real companion can be omitted by this
policy. No claim of resolved component/system identity follows.

The builder first reserves unconditional Hipparcos records and reads at most
the remaining capacity plus the number of explicit exclusions from Gaia. It
filters excluded IDs, then processes deferred preferences by decreasing Gaia
prefix rank. Every out-of-prefix preference becomes a Hipparcos fallback,
reducing the remaining Gaia slots; this continues until all suppressed native
rows have selected Gaia representatives. Hipparcos still fills capacity first,
with its brightest native records retained if it overflows. Sorting the final
selection preserves magnitude/catalogue/native-ID ordering. This bounded
resolution uses O(capacity + candidates) memory and expected O(capacity +
candidates log candidates + capacity log capacity) work, without a whole-catalogue
index or new runtime matching system.

A capacity smaller than the bright candidate set can normally truncate sources;
this differs from dropping a source because a counterpart was never selected.
Neither a 4,096-record build nor its view guarantees complete coverage through G=6.

## Reload command

From `msc/`:

```sh
./tools/download_hipparcos2_catalogue --source-url https://vizier.cfa.harvard.edu/ftp/cats/I/311/
```

The wrapper invokes [acquire_hipparcos2.py](../tools/acquire_hipparcos2.py) with
separate Hipparcos and Gaia data roots and a default inclusive derived-G limit of 6.
The command above uses the official CfA VizieR mirror after the Strasbourg
archive endpoints timed out during this repair. It needs
Python 3.9 or newer, curl, Linux file locking, network access, and a valid local
Gaia checkpoint/index or completed Gaia archive/manifest. It uses the Python
standard library and adds no application dependency.

Every invocation downloads the roughly 9 MB compressed source and creates
`data/hipparcos2/runs/<UTC timestamp>/`. Previous completed runs remain immutable.
A failed run has no completion manifest and cannot update
`data/hipparcos2/latest.json`; the pointer advances only after complete
validation and publication.

The script accepts `--g-limit`, `--root`, `--gaia-root`, `--source-url`, and
`--tap-endpoint`.
Changing the limit or conversion policy changes the derived dataset and must be
reviewed and recorded.

## Files and fields

Each completed run contains:

- `hip2.dat.gz` and `ReadMe`: the unmodified full source table and CDS schema;
- `hipparcos2.csv.gz`: all selected candidates, including derivation provenance;
- `gaia-crossmatch.csv.gz`: the published neighbour evidence and local status;
- `hipparcos2-supplement.csv.gz`: native fallbacks with explicit Gaia preference
  and exclusion IDs, resolved against the build capacity;
- `match-count-NNNN.xml`, `matches-NNNN.xml`, and matching `.adql` files:
  original count-checked TAP responses and exact queries in sorted batches of
  at most 256 HIP IDs (older runs used unnumbered files);
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
| `resolution_reason` | Native fallback or deferred-preference reason |
| `gaia_source_id` | Eligible published Gaia preference, or 0 |
| `excluded_gaia_source_id` | Published out-of-range neighbour to omit from Gaia fill, or 0 |

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
| `matched_not_in_verified_local_selection` | Published link is not proven within the inspected local selection; this is not proof of absence from the full archive |
| `no_published_best_neighbour` | Official table returned no row for the HIP ID |

During an incomplete Gaia acquisition, membership uses the existing SQLite ID
index in read-only mode and is bounded by the committed checkpoint row count.
The manifest records that checkpoint and its SHA-256. When Gaia is complete, the
script verifies the completed archive's byte size against its manifest and
streams its ordered bright prefix through the first row above the declared limit.
The manifest explicitly records rows scanned and this verification scope. It does
not claim a fresh full-archive count or checksum check. Published queries are
count-checked and VOTable overflow/errors fail; multiple best neighbours for one
HIP ID fail rather than being silently chosen.

## Raw-source retention correction

The two September run directories were found to lack `hip2.dat.gz` on
2026-10-28, despite their manifests and the intended retention contract above.
Their derived files and historical checksums have not been rewritten. The fresh
run retains the complete official raw source locally: 8,977,907 bytes, SHA256
`8e624f843d4254a9b7c2e8dda8e3158dbe825bf98f6bf3dbbae7f0d0b73d6858`,
identical to the historically recorded source. Raw downloads remain ignored by
Git; retaining a manifest alone is not retaining the raw data.

## Verified result: run `20261028T194241.299001Z`

Completed 2026-10-28T19:52:19.315084+00:00. The
[manifest](../data/hipparcos2/runs/20261028T194241.299001Z/manifest.json)
records script SHA256, source/mirror, all file hashes, formulas, units and the
completed Gaia archive's manifest. Exact count/data ADQL queries and original
VOTable responses are retained in the run. All 27 sorted batches passed their
independent count and overflow checks: 5,458 published neighbours.

| Measurement | Result |
| --- | ---: |
| Full native source rows / complete rows | 117,955 / 117,955 |
| Supported estimates / unsupported colour rows | 117,904 / 51 |
| Inclusive estimated-G<=6 candidates / prepared fallback rows | 6,752 / 6,752 |
| No published best neighbour | 1,294 |
| Published Gaia neighbour missing required fields | 143 |
| Complete published neighbour outside declared bright range | 138 |
| Complete, verified-local, in-range Gaia preferences | 5,177 |
| Required native fallbacks before prefix resolution | 1,575 |
| Completed Gaia rows recorded in its manifest | 134,217,728 |
| Gaia rows independently scanned here | 6,519 |

The Gaia inspection includes 6,518 usable rows through G=6 and the first row
above it. It checks header, row widths, finite fields, IDs, brightness/ID order
and archive byte size against the existing manifest. It does not rehash or
recount the full 8,575,699,250-byte archive or alter acquisition services.

The new supplement is 391,467 bytes, SHA256
`87d518cc1027b53eb69306caf7ff845adcee2b59934d1d415b9e2a02f916a81c`.
All 5,177 preference IDs and 138 exclusion IDs are individually distinct.

Real C++ builds at capacities 65,536 and 4,096 completed without rejected-row
warnings. The larger binary contains 1,575 Hipparcos and 63,961 Gaia records,
4,456,480 bytes, SHA256
`df6aaacce5f3f684597fe8e09969823aeb153d2f52299de9a38336b07dfc4735`.
Every one of the 6,752 declared candidates has its native or eligible Gaia
representation; none has both, including a check against every published link.
There are no duplicate native identity/provenance keys. All records have finite
measurements, J2016 positions, RGB8 and deterministic brightness ordering.

| Landmark | Runtime identity | Estimated G | J2016 RA, Dec (degrees) | Index in 65,536 binary |
| --- | --- | ---: | --- | ---: |
| Algol | HIP 14576 | 2.0742011124304 | 47.042236152449, +40.955639292555 | 73 |
| Mintaka | HIP 25930 | 2.185029 | 83.001669904988, -0.299098178071 | 85 |
| Phecda | HIP 58001 | 2.3868610055232 | 178.458505443814, +53.694808656917 | 107 |

Indices are zero-based. All three, plus Capella (24608), Betelgeuse (27989),
Rigel (24436) and Sirius (32349), occur in the default 4,096-record view prefix.
Algol and Phecda have no published best neighbour. Mintaka's published bright
Gaia source 3220756843825383936 has G=2.2487726 but lacks proper motions and
parallax. The nearby G=6.8278775 source is not accepted as primary coverage.
Position regressions agree with independent first-order J2016 propagation
within 1 mas; this is pipeline validation, not a new astrometric accuracy claim.
Magnitudes retain the accepted estimates from native Hp and colours, not a
light-curve model or externally calibrated brightness measurement.

The 4,096-capacity build contains the brightest 4,096 native fallbacks, with no
Gaia rows after prefix resolution. Its SHA256 is
`b7e6368c73685fda5413b83a2aa53bd498a013a89140a62c92e73f5f19179a83`.
It retains all seven regression landmarks but normally truncates 2,656 declared
candidates. Hipparcos-first membership and a different capacity can change
membership; it is not the same selection as viewing the first 4,096 of the larger
binary and does not establish completeness through G=6.

The validated larger binary was atomically installed at the existing default
application-data path after a verified recoverable backup. Only
`sources.hipparcos` changed in the backed-up local configuration; quantity and
build capacity remain 4,096, while the active binary retains 65,536 records.
Future default builds use this fresh supplement. No graphical review is claimed.

Validation: 217 C++ tests pass in the warning-as-error build and ASan/UBSan with
`detect_leaks=1:halt_on_error=1`; 35 Python tests, cppcheck 2.22.0,
Lizard 1.24.2 (`-L40 -C10`), clang-format 23.1.1 and Ruff checks pass.
The seven-row [offline fixture](../tests/fixtures/hipparcos2/README.md) tests
raw selection through published evidence and actual C++ build/binary preparation.
Additional regressions cover inclusive boundaries/overrides, strict input
validity, incomplete/nonlocal fallback, faint-neighbour rejection/exclusion,
selected-prefix cascades, deterministic ordering and exact capacity.

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
- Acquisition preserves the native epoch and performs no independent identity
  matching. The existing C++ build propagates positions to J2016.
- No published best neighbour does not prove that a source is absent from Gaia.
- Resolution is tied to the recorded local Gaia snapshot and must be regenerated
  after that snapshot changes.
- Unsupported photometric ranges remain excluded (51 rows in the full raw
  source); missing values are not imputed. Estimated-G selection near the limit
  is uncertain and is not an observational completeness guarantee.
- Runtime identity, colour, motion and binary contracts remain those in
  [catalogue integration](decisions/catalogue-integration.md).

## Sources and attribution

- [CDS Hipparcos-2 schema and source](https://cdsarc.cds.unistra.fr/ftp/I/311/ReadMe),
  van Leeuwen (2007), Astronomy & Astrophysics 474, 653.
- [Gaia EDR3 photometric relationships](https://gea.esac.esa.int/archive/documentation/GEDR3/Data_processing/chap_cu5pho/cu5pho_sec_photSystem/cu5pho_ssec_photRelations.html).
- [Gaia DR3 Hipparcos-2 best-neighbour documentation](https://gaia.aip.de/metadata/gaiadr3/hipparcos2_best_neighbour/).
- [Gaia DR3 source field documentation](https://gaia.aip.de/metadata/gaiadr3/gaia_source/).

Preserve ESA/DPAC, Gaia@AIP, CDS, and catalogue-publication attribution when
redistributing derived data.
