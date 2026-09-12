# MSC objectives and decisions

Captured from the owner discussion beginning on 2026-09-08. This is the detailed
scope, learning, and decision record. It now includes the approved milestone
schedule, while AGENTS.md remains authoritative for ownership modes and delivery
gates. Recording a milestone does not authorize its implementation.

## Product ambition stated by the owner

Render a reconstructed sky from a chosen Earth location and date/time, including
present-day and historical settings. Examples include Toulouse today, Argentina
around the colonial period, and exploration of 2500 BCE.

“Any date” is a product ambition, not a claim of uniform scientific accuracy.
Supported ranges, calendar conventions, motion models, Earth-orientation models,
and accuracy claims remain to be decided and validated. Date acceptance and
reconstruction accuracy are separate capabilities.

The primary learning and portfolio objective is C++ systems engineering and
measured optimization. Astronomy provides a substantive real-data workload and
a compelling output. The owner accepts documented approximations and modest
visual inaccuracies: the sky should remain grounded in astronomy, without
certification-grade or research-grade accuracy becoming a prerequisite.
Numerical accuracy targets will be decided later from evidence and intended use.

## Current direction and owner policies

- Gaia acquisition targets 134,217,728 records (2^27), the largest power of two
  estimated to fit under the hard 10,000,000,000-byte compressed safety limit.
  Benchmark datasets are prefixes. Acquisition is incomplete; see gaia-datasets.md.
- The current Gaia contract supersedes historical choices below: nine required
  fields (source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag,
  bp_rp). All _error fields, radial_velocity, and designation are excluded.
  A name is optional for later enrichment. Assume zero radial motion for now.
- Preserve exact executed acquisition commands and queries in Markdown.
  Distribution is unresolved: one 10 GB archive exceeds Free/Pro LFS file limits.
- Preserve a resumable two-part continuation key (G magnitude and source_id),
  and keep a repository command that resumes locally or downloads from zero on
  a fresh computer.
- Current scope is ingestion and making real catalogue directions usable by
  the existing core. Accurate observer-and-date reconstruction is later work.
- Keep source-specific parsing separate from downstream astronomy. A common
  interpretation of units, frames, epochs, and missing values is necessary.
  The nine normalized fields and their semantic wrappers are approved. Exact
  class API and binary/in-memory layout remain unselected; the initial container
  is a vector.
- All nine Gaia fields are required for an accepted row. Proper motions are
  measured rates and remain mandatory; do not derive missing values from another
  coordinate pair. The later motion model may assume zero unknown radial motion.
- Preserve signed Gaia parallax, including negative measurements. Direction-only
  use remains valid; distance conversion must reject nonpositive parallax rather
  than applying an absolute value.
- Source brightness and colour measurements remain distinct from RGB/pixels;
  display appearance is derived later, as now approved by the owner.
- Continue past rejected records. Aggregate accepted/rejected counts, rejection
  reasons, and the percentage accepted; detailed row diagnostics can be optional
  debug/trace output, not per-row warnings. This is owner-approved.
  The owner suggested spdlog; library integration and exact severity choices are not implemented.
- File-level failure is logged as an error and the application reports failure.
  The owner withdrew the fixed code 2 requirement and delegated conventional code
  selection. Use EXIT_FAILURE for ordinary operational load failures; parsing
  components report failure rather than terminating the process themselves.
- Tests include known catalogue rows and their interpreted values, not only range
  validation. This is an agreed requirement, not an implemented test suite.
- Do not impose an arbitrary global acceptance-percentage threshold. Use the
  reported counts for investigation; deterministic fixture expectations and
  known dataset comparisons are distinct from a universal runtime cutoff.
- Treat columns according to the source schema and documented units. Missing
  required values are rejected rather than converted to default numeric values.
- Prefer short aliases for fixed-width integer types in .cc files. Avoid defining
  these aliases in public headers by default. This is the owner's style preference.
- When requesting permission, show the exact command to be run each time.
- Benchmark subsets are nested brightness-ordered prefixes of 4,096, 65,536,
  262,144, and 1,048,576 records. The target application catalogue is the
  134,217,728-record prefix subject to the 10 GB acquisition cap.
- The owner directs requirements and important engineering decisions, and wants
  the assistant to teach, explain alternatives, and disagree when warranted.

## Milestone schedule — approved 2026-09-11

The owner approved a performance-first ten-milestone sequence and asked for
dated objectives. The public README contains each milestone's objective, newly
introduced capabilities, and engineering problems. Target dates assume roughly
alternating weeks between MyStarCeiling and another portfolio project, with
lower availability beginning in February 2027. Dates are planning targets rather
than promises; scope should contract before correctness or evidence is weakened.

| Milestone | Target | Portfolio meaning |
| --- | --- | --- |
| M0 — Typed astronomy foundations | 2026-10-04 | Required base |
| M1 — Fixed Toulouse sky | 2026-11-29 | First complete vertical slice |
| M2 — Scalable stellar rendering | 2027-01-31 | 262k benchmark and inspectable output |
| M3 — Arbitrary observer and time | 2027-03-28 | Core product claim demonstrated |
| M4 — Million-source interactive pipeline | 2027-05-09 | Correct performance baseline |
| M5 — Profile-guided CPU engine | 2027-06-27 | Preferred portfolio evidence gate |
| M6 — CUDA acceleration and comparison | 2027-08-29 | Stronger acceleration evidence |
| M7 — Large-catalogue systems | 2027-11-28 | Out-of-core scale evidence |
| M8 — Solar-system geometry | 2028-02-27 | Physical objects and eclipse behavior |
| M9 — Dynamic and synthetic skies | 2028-06-28 | Procedural and atmospheric stress workloads |

M5 is the preferred June portfolio target because it demonstrates an end-to-end
product, real scientific data, numerical validation, profiling, and measured CPU
improvements. M6 strengthens the optimization story because it adds a defensible
heterogeneous-computing comparison. CUDA precedes synthetic skies.

### M1 decisions

The approved M1 contract, its evidence, and remaining questions now live in the
focused [M1 milestone record](milestones/m1-fixed-toulouse-sky.md). This file
retains the dated discussion below as history rather than duplicating the active
contract.

## Repository evidence inspected on 2026-09-08

The core contains angle/coordinate validation, direction-to-unit-vector
conversion, a local east/north/up rotation, and an Earth rotation angle function.
The CLI is a placeholder. No catalogue loader or rendered sky exists.

The Hipparcos export contains 118,218 data records. Its decimal coordinate pairs
refer to J1991.25 and J2000. Both pairs have the same missing-value pattern:
263 records lack them; there are no partly populated decimal pairs. Cross-epoch
fallback therefore recovers no additional records in this file. The four
celestial_*.csv files are empty. The export uses semicolons despite its .tsv
extension and includes comments, column labels, units, and a separator row.

The existing local rotation and Earth rotation calculation are not yet a
validated complete ICRS-to-observed-direction chain. Existing test definitions
cover useful geometric properties and an Earth-rotation reference value.

## Learning notes and unresolved scientific requirements

An epoch is the date at which a position applies. Propagation estimates a new
position using elapsed time and a motion model. The epoch interval is not an
expiry period. Re-expressing a position at J2000 does not create a new observation
or improve the original measurement precision.

Stellar motion, changing Earth-axis orientation, daily Earth rotation, and
observer location are distinct physical effects. Their calculations may later
be composed or share precomputed work; no fusion or optimization is selected.

Long historical reconstruction requires a declared validity range and error
assessment. More floating-point digits cannot remove measurement uncertainty,
missing velocity information, or historical Earth-rotation uncertainty.

Parser success must include correct interpretation, not merely legal numeric
ranges. For example, 12 degrees and 12 hours are both valid RA inputs but denote
different directions. Expected rejection of incomplete records is compatible
with a correct loader; silently assigning the wrong meaning to accepted records
is not.

Aggregate rejection reporting is now owner-approved. Separate record rejection
from a broken file/schema or parser. The RA/declination swap example was a
hypothetical parser mapping bug, not a defect observed in Hipparcos.

In this export, RAICRS and _RA.icrs are degree-valued columns; RAhms is a separate
hours/minutes/seconds column. The schema is explicit. Choosing the correct angle
conversion and checking representative records is sufficient for this concern;
there is no proposal to guess units per row.

The J2000 columns are propagated from the catalogue astrometry, not independent
newer observations. Selecting the closer epoch does not inherently improve the
underlying measurement accuracy. Algorithmic approximation and rounding effects
may differ, so any proposed advantage needs evidence.

## Capability checkpoints for future owner planning

These are candidate completion statements, not an ordered or scheduled roadmap.
They do not replace the delivery gates in AGENTS.md.

| Capability | Evidence needed to claim it |
| --- | --- |
| Real-data ingestion | Accepted values preserve source meaning; required-field policy and rejection accounting are verified. |
| Dated reconstruction | Positions for chosen observers and instants agree with independent references within declared errors and date ranges. |
| Interactive preview | A real catalogue sky is projected and rendered; camera and time controls behave consistently. |
| Performance evidence | Representative workloads, profiles, and reproducible before/after results support the chosen optimizations. |
| Historical reach | Wider date intervals have explicit calendar/model conventions and measured or documented limitations. |

## Performance ambition and learning priorities

The owner targets a 250k-star demonstration, ideally a full sky in under one
second. This is an unmeasured ambition. Hardware, accuracy, output, and whether
that second includes cold disk loading, parsing, stellar motion, projection,
and rendering must be defined before it becomes an acceptance threshold.
The owner explicitly intends to benchmark optimizations, including CUDA, and
use measured speed improvements as central portfolio evidence.

Loading and data processing are major portfolio and learning priorities.
Potential investigations include avoiding unnecessary work, visibility filtering,
spatial queries, reusing common calculations, matrix operations, CPU parallelism,
and CUDA. These are hypotheses and learning interests, not selected solutions.
A correct measured CPU baseline remains necessary; GPU work should have both an
explicit learning objective and an honest end-to-end performance comparison.

A sequential text file normally still needs scanning to discover invisible
stars. Avoiding later calculations or drawing is different from avoiding disk
reads. A visibility selection for one instant must remain correct when time or
observer location changes. Per-row logging also contributes to ingestion cost.

Visibility filtering is a candidate optimization, not an automatic speedup at
any stage. Its classification/compaction cost must be compared with the downstream
work avoided. Retaining catalogue records in memory while omitting their current
draw work may support changing time/location without repeated disk loading.
Exact workload, filtering stage, and data retention decisions remain open.

## Additional capabilities requested by the owner

- Pause/resume time, speed changes, and moving the camera.
- Rendering personalization: brightness/exposure, explicit magnitude changes,
  daylight, Moon illumination, light pollution, and visibility adaptation.
- Sun, Moon, planets, clouds, and additional celestial objects.
- Meteor behavior informed by location, date, and plausible appearance.
- Reproducible synthetic skies, including artificial planet placements and
  meteor events, distinguishable from reconstructed observations.

These remain backlog interests. Sun/Moon/planet prediction, atmosphere/clouds,
and meteor generation need their own domain models; loading star records alone
does not implement them. Randomized astronomical inputs and invented object
arrangements must remain distinguishable.

## Scientific references

- [ERFA stellar propagation](https://raw.githubusercontent.com/liberfa/erfa/master/src/starpm.c): source and target epochs, motion inputs, and constant-space-velocity assumptions.
- [ERFA observed positions](https://raw.githubusercontent.com/liberfa/erfa/master/src/atco13.c): catalogue-to-observer transformation effects and conventions.
- [NASA historical Earth rotation uncertainty](https://eclipse.gsfc.nasa.gov/SEcat5/uncertainty.html).
- [ESA Gaia DR3 contents](https://www.cosmos.esa.int/web/gaia/dr3): the approximately 33 million radial-velocity sample is a subset of the larger catalogue.

## Decision history — 2026-09-08 clarification

- Confirmed C++ and optimization as the central learning objectives; accepted
  practical astronomical approximations. Detailed accuracy targets remain open.
- Accepted rejecting records with missing required magnitude or motion data as
  an initial coverage/simplicity trade-off; harmless metadata stays optional.
- Accepted aggregate rejection counts/reasons and acceptance percentage, with
  optional detailed logs; rejected arbitrary global percentage cutoffs.
- Confirmed benchmark-driven CPU/CUDA work as central portfolio evidence.
- Requested durable Markdown decision records, with Doxygen/MkDocs deferred.
- No new implementation task, epoch-selection algorithm, filtering architecture,
  accuracy threshold, or milestone order was selected.

## Decision history — 2026-09-08 scope confirmation

- Selected using an available complete coordinate pair with its source epoch;
  nearest-epoch selection and loading both pairs are unnecessary for now.
- Requested stricter initial completeness for easier debugging, with possible
  relaxation later. The exact meaning of “all values” remains to be clarified;
  no blanket requirement on optional raw columns is silently adopted.
- Invalid records should be rejected and accounted for. This is intended behavior,
  not evidence that a loader is implemented or verified.
- Project direction remains C++ systems learning, owner-directed engineering,
  and measured performance improvements using an astronomically grounded sky.

## First-loader clarifications — 2026-09-09

### Coordinates and motion

The source epoch is in the header's column descriptions, not a standalone row
column. J1991.25 applies to both the original decimal pair and the textual pair;
J2000 applies to the derived decimal pair. Epoch means the date at which the
position applies, and is needed with proper motion to estimate another date.
Textual coordinates are truncated compared with the original decimal positions;
they are an alternative encoding with reduced precision, not extra measurements.

pmRA and pmDE are mandatory by explicit owner decision. They are measured motion
rates, not a full prediction model by themselves. The pmRA field includes the
cosine of declination, as documented in the export. Units are mas/year.

Read-only audit of all 118,218 data records found no wrong column counts and no
nonblank decimal coordinate pairs failing numeric, finite-value, or angular-range
checks. There are 263 rows with neither decimal pair but with textual coordinates;
all 263 lack both proper motions and fail the agreed required-motion rule.
This audit is not an implemented C++ parser test or proof of all field semantics.

### Unresolved field and rendering choices

- HIP: retain for debugging and later object lookup. Missing/invalid identifier
  policy remains open; common-name lookup is a later feature.
- Plx: retain the distance-related parallax measurement. Missing-value and
  nonpositive-value policies remain open. Existing direction-only math does not
  require distance.
- e_Plx: retain uncertainty in that measurement. Missing-value policy remains
  open; no quality threshold is selected.
- B-V: retain for later display-colour estimation. Whether a missing value
  rejects a star remains open.
- Vmag: mandatory apparent visual brightness. Neither it nor B-V is directly an
  RGB pixel. The owner asked about a combined rendering representation; no such
  representation is approved. Preserving source values and deriving display
  appearance later, including glow/exposure, is now owner-approved.
- Notes: ignore for the first loader.

### Failure handling and output

The initial policy is now approved: blank alternative coordinate pairs permit
fallback; malformed nonblank numerical content in a field the loader interprets
rejects the row; Notes is ignored. This does not add validation requirements for
arbitrary ignored columns.

File-level failures: application error log and exit code 2, explicitly chosen.
Per-row failures: debug/trace with aggregate rejection accounting, as previously
agreed. No arbitrary acceptance-percentage cutoff.

The owner approved a new shared star class in principle and proposed an array
using existing domain types. The container, class layout, API, and ownership
design are not final. The owner explicitly prohibited implementing it yet.
Future catalogue imports should reuse the shared interpretation of star data;
no promise of an immutable schema covering arbitrary future sources is made.

## Decision history — 2026-09-09 field discussion

- Chose original decimal coordinates first, J2000 decimal second, textual pair
  third; retained the source-epoch requirement.
- Explicitly made pmRA and pmDE mandatory alongside coordinates and Vmag.
- Chose to ignore Notes and use application exit code 2 for file-level failure.
- Discussed HIP retention, parallax/uncertainty, colour, malformed unused fields,
  and a star collection; unresolved points remain marked above.

Additional reference: [NASA HEASARC Hipparcos field documentation](https://heasarc.gsfc.nasa.gov/w3browse/star-catalog/hipparcos.html).

## Decision history — 2026-09-09 motion and catalogue compatibility

- Reaffirmed that pmRA and pmDE are mandatory motion rates, separate from the
  coordinate fallback chain. Deriving motion from the two epochs was proposed
  for discussion; no derived-motion fallback is approved.
- Approved keeping HIP and B-V and deferring RGB/pixel conversion to rendering.
- Approved the initial blank-versus-malformed row policy described above.
- Approved a shared star class in principle, explicitly without implementation.
  Reuse across future imports is a design objective.
- Plx and e_Plx remain undecided; describing them as less important was not
  treated as a decision to discard them.

### Evidence and learning notes from this discussion

Read-only inspection found HIP identifiers from 1 to 120416; 52760 rows exceed
65535. A 16-bit unsigned identifier cannot hold this file's HIP values. The final
identifier representation is not selected. Gaia DR3 source_id is 64-bit and its
meaning includes the catalogue release.

For independent measurements at two known epochs, position differences can
estimate average motion, with angular wrapping, geometry, and units accounted
for. Here the J2000 coordinates are derived using the original motion. Differencing
them would reconstruct an approximation to an existing input, not supply new
information. All 263 rows missing motion also lack both decimal coordinate pairs;
that fallback recovers no rows from this export.

Plx describes a distance-related angular measurement; e_Plx is its standard
uncertainty, not a hard min/max range or a velocity. Random perturbation of parallax
would sample an uncertainty model, not implement stellar motion or ordinary
parallax displacement. No random uncertainty sampling is selected.

Gaia compatibility requires more than column renaming. Gaia supplies celestial
coordinates, reference epoch, proper motion, and parallax, but uses Gaia G/BP/RP
photometry rather than Hipparcos Johnson V and B-V. Substituting G for V or BP-RP
for B-V without an explicit interpretation or conversion changes meaning.
Handling these differences, missing measurements, and source identifiers is
unresolved design work; no photometric conversion, new fields, or storage layout
is approved by this note.

Reference: [Gaia DR3 source schema](https://gea.esac.esa.int/archive/documentation/GDR3/Gaia_archive/chap_datamodel/sec_dm_main_source_catalogue/ssec_dm_gaia_source.html).

## Decision history — 2026-09-09 Gaia direction and numeric types

### Owner decisions and preferences

- Rejected deriving missing proper motions from differences between epochs.
- Approved retaining Plx and e_Plx for possible later use.
- Prefers Gaia conventions where practical and accepts future refactoring rather
  than demanding an immutable universal star schema.
- Prefers explicit-width numeric types and is considering short aliases such as
  u16. Alias naming/scope and floating-point types are not yet selected.
- Is open to starting directly with Gaia rather than first implementing Hipparcos.
  Suggested catalogue subsets around 64k, 128k, 256k, and 1M+ stars. These are
  candidate workloads, not finalized datasets or downloads.

### Assistant recommendations, not selected implementation

Starting with a small Gaia subset is now recommended because the intended main
source is Gaia and no loader is implemented. This avoids paying for an interim
Hipparcos-specific photometric conversion solely to imitate Gaia. The trade-off
is upfront dataset selection and acquisition. Keeping Hipparcos as the immediate
offline input remains a viable alternative. No source switch is made by this note.

A shared application representation should give quantities stable meanings while
parsers translate source encodings. It need not copy the entire Gaia archive
schema or contain every possible measurement. Gaia G/BP/RP and Johnson V/B-V are
different photometric systems. Published conversions are empirical relationships
with applicability limits, not exact aliases or a generally lossless conversion.
No particular conversion or intermediate photometric representation is selected.

Larger subsets add catalogue coverage and workload, not inherently better
per-star astrometric accuracy. Brightness-selected skies and representative
performance samples can serve different purposes. Nested subsets are now owner-approved. Deterministic generation and the actual
selection/order rule remain to be finalized.

Short integer aliases have no runtime overhead and do not create distinct types.
The owner now prefers aliases in .cc files and avoiding their definition in public
headers. This supersedes the earlier shared-public-alias recommendation.
Explicit widths should express an actual range/storage requirement, not blindly
replace every integer or size type. Unsigned integers cannot represent signed
fractional measurements such as proper motion or declination.

C++ provides optional std::float32_t/std::float64_t via <stdfloat>. Availability
and library/CUDA interoperability must be checked before selection. Merely naming
a double alias f64 does not itself enforce a 64-bit IEEE representation. Numeric
aliases also do not replace domain invariants in existing coordinate types.

References:

- [C++ alias declarations](https://eel.is/c++draft/dcl.typedef).
- [C++ optional fixed-width floating-point types](https://eel.is/c++draft/basic.extended.fp).
- [Gaia DR3 photometric relationships](https://gea.esac.esa.int/archive/documentation/GDR3/Data_processing/chap_cu5pho/cu5pho_sec_photSystem/cu5pho_ssec_photRelations.html).

## Decision history — 2026-09-09 access and portability confirmation

- Require exact commands to be shown with permission requests.
- Prefer fixed-width integer aliases in .cc files; avoid defining them in public
  headers by default. Floating-point concern is representation portability,
  not opposition to the spelling double itself.
- Explicitly rejected converting Hipparcos photometry to Gaia merely as an
  interim first-loader strategy.
- Gaia is preferred if acquisition is straightforward; other sources remain
  acceptable if there is a better concrete reason. No bulk acquisition or
  application loader implementation is authorized by this discussion.
- Confirmed nested dataset sizes for scaling benchmarks. Exact sizes and sample
  selection remain open; earlier counts were examples.

### Verified access and platform evidence

A read-only anonymous HTTPS query to the ESA Gaia TAP synchronous endpoint
returned one Gaia DR3 source as CSV, including identity, epoch, coordinates,
proper motions, parallax/error, G magnitude, and BP-RP. No account, external
Python package, or local database installation was needed. The record was printed
for access verification only; it was not saved as a project dataset. This proves
small-query access, not bulk-query throughput or the suitability of a sample.

The local g++ default target reports 8-bit bytes, sizeof(double)=8, binary
significand precision 53, maximum exponent 1024, minimum exponent -1021, and
IEC 60559 support. This matches binary64 double on this target. The C++ language
itself leaves the representation implementation-defined. Assistant recommendation:
retain double where appropriate and add compile-time checks for the required
representation when implementation is authorized. No checks or type changes
were implemented. A 32-bit machine/program does not inherently imply a 32-bit double.

Remaining acquisition decisions are the Gaia release/sample scope, selected
columns, completeness rules, selection/order rule, and initial sizes. The
previous Johnson V/B-V contract must not silently become Gaia G/BP-RP; that
source-specific adjustment remains visible when the Gaia contract is settled.

Reference: [ESA Gaia programmatic access](https://www.cosmos.esa.int/web/gaia-users/archive/programmatic-access).

## Decision history — 2026-09-10 Gaia acquisition contract

- Authorized replacing active Hipparcos data with Gaia datasets.
- The application catalogue is all qualifying sources, not a fixed-size sample.
- Original ten Gaia fields are explicitly approved and mandatory: source_id,
  ref_epoch, ra, dec, pmra, pmdec, parallax, parallax_error, phot_g_mean_mag, bp_rp.
- Add radial_velocity and radial_velocity_error for later use. The owner chose
  nullable handling conditional on being able to represent unknown values;
  unknown values can be represented without fabricating a measurement.
  Both fields are retained when present; their absence does not reject a source.
- Unknown is distinct from measured zero. A zero substitution in a later motion
  calculation would be an explicit approximation, not a loader fallback. No
  zero substitution or uncertainty-sampling behavior is selected.
- Brightest-first ordering means ascending G magnitude, with source_id as the
  deterministic secondary ordering. Nested subsets are prefixes of this order.
- Benchmark counts selected under the owner's delegation: 4096, 65536, 262144,
  1048576; the full qualifying catalogue is the separate application dataset.
- Fixed exit code 2 was withdrawn. Conventional operational failure handling
  uses EXIT_FAILURE; no application code was changed.
- Additional Gaia fields are under review before bulk transfer. No optional
  additions or scientific quality thresholds are silently selected.

Read-only inspection found approximately 162 GiB free on the repository disk and
7.6 GiB total tmpfs capacity at /tmp. A 1000-row, 12-column Gaia sample satisfying
all ten mandatory fields occupied 153904 CSV bytes (about 154 bytes/row), or
69556 bytes compressed with gzip. It was an unordered sizing sample, not a
representative benchmark or final catalogue. 948 rows lacked at least one radial
velocity field. Its compression ratio and null fractions must not be assumed
constant across Gaia. No exact total qualifying count has been measured.

Dataset replacement is not complete. Full field selection, storage destination
and format, and a verified complete batch/merge method remain unresolved.

## Decision history — 2026-09-10 storage and modelling clarification

The owner withdrew exhaustive Gaia downloading, prefers Git-compatible datasets
and avoiding duplicate storage, and proposed disjoint increments for nested sets.
Physical packaging and application size remain open; see gaia-datasets.md.
Unknown radial motion may be assumed zero for calculations. Randomizing unknown
motion or measurement errors is not an approved policy. Existing selected error
columns have not been removed. A name field is requested, allowing absence until
later enrichment. Extra quality/photometry choices remain under discussion.
The requirement for constructive disagreement across all portfolio projects is
recorded in the portfolio README. No loader, star class, or bulk acquisition was
implemented as part of this clarification.

## Decision history — 2026-09-10 reduced fields and single catalogue

The owner removed all _error fields, radial velocity, raw fluxes, RUWE,
astrometric_params_solved, duplicated_source, and random_index. source_id stays;
acquired records must have unique IDs. Zero radial motion remains an accepted
calculation approximation. Proper motion and parallax remain required.
The owner conditionally prefers BP-RP unless separate bands are worthwhile;
the assistant recommends BP-RP for the initial renderer, with no demonstrated
colour improvement from the extra field. designation and a later name mapping
were discussed, without choosing their representation.
The owner prefers one brightness-ordered catalogue. Git LFS is under discussion,
not installed or selected. Exact acquisition commands must be logged in Markdown.
These decisions supersede older field and packaging snapshots in this file.

## Decision history — 2026-09-10 capped acquisition

The owner confirmed all nine fields, BP-RP, and exclusion of designation. After
revisiting an uncapped selection, the owner set a 10 Go cap. Acquisition uses
one compressed brightness-ordered catalogue with a 10,000,000,000-byte limit;
smaller benchmarks are prefixes. Exact commands are logged in Markdown. The first
1,048,576 sources were downloaded and validated; the complete application
catalogue remains in progress. No C++ loader or star class was implemented.
