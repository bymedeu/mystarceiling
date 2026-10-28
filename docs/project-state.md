# Current project state

Last updated: 2026-10-28.

This is the short hand-off record for continuing MSC from another account or
session. Detailed decisions remain in the linked focused documents.

M1 is complete on 2026-09-13. M2 — ceiling appearance and data controls —
is the active planning target (estimate 2026-12-13; scope to discuss).

The first bounded M2 refactor separates source-independent
`core::astronomy::Star` and its measurement wrappers from
`catalogue::CatalogueStar` identity/provenance. See the
[boundary decision](decisions/catalogue-integration.md#first-bounded-m2-refactor--owner-decision-2026-09-13).
That first refactor preserved the version-2 binary and M1 scene behavior; later
M2 preparation changes are recorded below. The recorded
post-refactor suite passed all 119 tests, including a frozen-byte compatibility check against
the pre-refactor writer.

## Current planning baseline

The [README roadmap](../README.md#milestones) is canonical. Owner revision,
2026-10-28: M3 benchmarking precedes M4 documentation; camera movement/zoom
belongs to M5. M6 now covers high-usage measurement and optimization without a
mandatory million-source gate, including the historical large-catalogue
milestone (formerly M7). M7 now covers profile-guided work, the output boundary
and consolidation. M8 is optional CUDA; M9 is discrete objects; M10–M13 cover
JSON-driven synthetic skies and customization, long exposure, illumination,
and clouds. Optional extensions remain conditional; the changed scope has no
recalculated schedule.
The [M3 record](milestones/m3-benchmarking.md) distinguishes accepted goals
from the proposed measurement design. The standalone developer executable is
accepted; it is not implemented. The [M2 sequence](milestones/m2-ceiling-appearance.md#approved-sequence)
and [existing controls](decisions/application-controls.md) remain in effect.
[M4 documentation](todo.md#m4-project-documentation) follows
benchmarking and precedes M5; tooling remains to choose.
Part 1 catalogue
build/info and configured viewing are implemented; the broader understanding
checkpoint remains open.

CPU-first portable delivery targets a 16 GB Linux laptop without an NVIDIA GPU.
The RTX 3060 is on a separate Windows desktop; Linux there is a future possibility.
CUDA is conditional on setup/access and a justified measured workload.
[Size accounting](decisions/memory-accounting.md) measured Star/CatalogueStar/
TrialStar at 64/80/48 bytes on 2026-10-28, all aligned to 8 bytes. ScreenPoint
and the static renderer have been removed; these sizes are not peak RSS or a
benchmark.
Completed 134m acquisition does not demonstrate full-catalogue runtime capability.

## Required understanding checkpoint

Owner decision, 2026-09-14: catalogue integration is the first identified debt
in algorithmic understanding. Before the next substantial implementation task,
pause for the [Algorithmic Ownership Gate](../README.md#algorithmic-ownership-gate).
The owner must explain the current pipeline against its seven questions where
applicable, close any gaps through discussion and teach-back, and explicitly
approve implementation before work proceeds. Status: open; understanding has
not yet been demonstrated. Earlier implementation approvals do not close it.

Start from [catalogue integration](decisions/catalogue-integration.md) and trace
the actual acquisition/preparation, overlap resolution, parsers, normalized
records, brightness selection, binary storage, and scene hand-off. Inspect code
and tests to distinguish implemented behavior from intended design. See the
[decision history](development-history/2026-09-14.md).

## Historical M1 evidence

- M1 normalizes the 4096-row Gaia prefix and resolved 62-row Hipparcos supplement
  through separate parsers, then selects the brightest 4096 combined records.
  All 62 supplement stars survive selection. IDs are native uint64 values with
  separate provenance; Hipparcos BP-RP is absent and Gaia BP-RP is preserved.
- Binary version/schema 2 preserves provenance and optional colour. Regenerate
  old binaries through the CLI; version 1 is rejected.
- The fixed Toulouse scene propagates Hipparcos from J1991.25 to J2016 using the
  tested tangent-vector operation, leaves Gaia unchanged, and reports/skips
  propagation failures.
- The owner-approved temporary G<=4 filter follows astronomical visibility.
  Three-pixel grayscale points use stronger brightness contrast without a floor.
- Debug build and all 117 C++ tests passed. Real-data execution selected 4096,
  reported 0 propagation failures, excluded 720 visible records by magnitude,
  and created 125 screen points. The owner successfully ran the combined
  application in a graphical session and supplied the closeout screenshot.
  They found the rendering very nice and the magnitude distribution realistic,
  without recognizing their usual constellations. The screenshot is visual
  evidence; automated known-row catalogue-to-screen tests provide positional
  and orientation evidence. See [M1](milestones/m1-fixed-toulouse-sky.md).
- Hipparcos preparation and the source files are unchanged by this integration.

## Current M2 part 1

The CLI supports configured `view`, `catalogue build` and `catalogue info`.
Builds fill exact capacity from Hipparcos first and the remaining ordered Gaia
prefix, sort the selection and check the completed temporary binary before safe
publication. Info reports path, count and bytes with a decimal GB equivalent.
Source/capacity configuration, temporary CLI overrides and file logging are
integrated. All 176 C++ tests pass in ordinary and ASan/UBSan builds; all 30
Python tests pass. Cppcheck, Lizard and formatting checks pass.
The complete current usage and required TOML schema are in
[README](../README.md#run-the-prepared-catalogue-viewer); detailed behavior is in
[application controls](decisions/application-controls.md).

## Initial M2 part-2 implementation

Both parsers now prepare compact linear-sRGB uint8 colour through source-specific
temperature approximations and move positions to J2016 during building. Magnitude
remains double precision. Binary version 3 uses complete 68-byte records; old
binaries require rebuilding. The viewer uploads integer RGB attributes and encodes
sRGB exactly once, preserving the prior white-star display curve. Native motion
components/parallax are retained as an accepted approximation. Source CSVs remain
available for rebuilding; diagnostic metadata stays outside the binary.

All 189 C++ tests pass in ordinary and ASan/UBSan/leak builds. Software-OpenGL pixel
checks and a real 4096-record build pass, retaining all 62 Hipparcos supplement
stars and exact source magnitude values. At that stage, record sizes were
64/80/16 bytes for Star/CatalogueStar/ScreenPoint; the last type has since been
removed. No physical-display review or full-size runtime claim follows. See the [colour model](decisions/stellar-colour.md),
[prepared catalogue contract](decisions/catalogue-integration.md#current-prepared-catalogue--2026-10-25)
and [memory evidence](decisions/memory-accounting.md).

## Current animated viewer

Plain `msc view` always prepares and shows the animated scene. On 2026-10-28 the
owner requested promotion of the existing animation and removal of the static
alternative. Both `--appearance-trial` and `--animation-trial` reject before
configuration/logging/catalogue side effects; no replacement setting is added.
The renderer keeps limiting magnitude 6, RGB8, fixed Toulouse/time geometry,
Space picture freeze/resume, Escape close and resize behavior.

The owner accepted the stronger native comparison as the default on 2026-10-28.
Its bounded visual response is now the ×1 baseline, with a 30 fps ceiling.
Temporary comparison controls and constellation guides are removed. The shared
spectral signal, correlated colour delays and spatial profile are preserved;
the bounded response follows linear signal averaging. See the
[viewer model](decisions/stellar-appearance-trial.md) for verification and limits.
Optimization and million-source animation remain future measured work;
zone/tier strategies are unimplemented. This promotion does not close M2 or the
broader understanding checkpoint.

The stronger ×1 baseline passes 218 C++ tests in ordinary and leak-enabled
ASan/UBSan builds, formatting, cppcheck and Lizard. Its 100-frame offscreen crop
is byte-identical to the accepted comparison; picture freeze/resume passes.

Earlier promotion validation: all 211 C++ tests pass in the ordinary build and with
ASan, UBSan and leak detection enabled; all 30 Python tests and the cppcheck,
Lizard and clang-format checks pass. The leak-enabled suite was rerun outside
sandbox tracing after LeakSanitizer rejected that environment. The rebuilt
native viewer was launched using plain `msc view`; owner review of this launch
remains separate from automated checks.

## Future direction, not implemented

Automatic visibility and future CLI/GUI controls remain future work. Current
local time is the approved future time default; the default observer and
place-name resolution remain open. The viewer still uses fixed Toulouse/time.
The current viewer does not implement camera controls, physical exposure
integration, Moon/cloud models, zone selection or rendering tiers.

## Acquisition status

The selected Gaia acquisition completed at **2026-10-18T18:20:08.887686+00:00**:
134,217,728 rows / 8,575,699,250 compressed bytes, below the 10,000,000,000-byte
cap. Final archive and manifest exist; the partial archive and duplicate index
are absent. Checkpoint `done=true`, counts and cursor agree with the manifest;
archive size agrees with both. See [completion evidence](gaia-datasets.md#verified-acquisition-evidence)
for the SHA256 and distinction between downloader verification and independent
metadata checks. No archive rehash or service change was performed for this
record; live service status was unavailable through the sandbox's user bus.
Earlier incomplete checkpoints and service observations in the
[recovery record](gaia-recovery.md) are historical.

The latest Hipparcos completed run is `20261028T194241.299001Z`, prepared under
the owner's explicit bounded bright-source repair authorization on 2026-10-28.
The default selection is now inclusive estimated-G<=6, with all 6,752 native
fallbacks retained until actual capped-build resolution. The full 117,955-row
raw source is retained locally; September's missing raw files are documented.
Published Gaia preferences require complete fields, proven local selection and
actual output membership. Out-of-range published neighbours cannot substitute
for primaries and are excluded from Gaia fill. See the
[canonical policy and validation](hipparcos2-datasets.md#verified-result-run-20261028t194241299001z).

The backed-up default runtime binary was atomically replaced while preserving
65,536 records (1,575 Hipparcos, 63,961 Gaia). Its default 4,096-record view prefix
contains Algol/HIP 14576, Mintaka/HIP 25930 and Phecda/HIP 58001, plus the four
preservation landmarks. Only the local Hipparcos source path changed; configured
quantity/capacity stay 4,096. A separate 4,096-capacity build retains these seven
but normally truncates the declared bright set. Neither bounded selection nor
the catalogue's complete-row subset establishes general sky completeness.

Repair validation passes 217 C++ tests in ordinary and leak-enabled ASan/UBSan
builds, 35 offline Python tests, cppcheck, Lizard, formatting and Ruff checks.
Rendering, colour and motion models are unchanged; no new graphical acceptance
or scientific accuracy claim follows. The broader understanding checkpoint
remains open.

## Open owner decisions

- classification taxonomy/bit allocation and future metadata layout;
- richer native/derived photometry beyond the bounded runtime contract;
- conflict reporting for otherwise usable source records;
- whether magnitude layers first appear during M1 binary preparation or after
  measuring the existing 4,096-row baseline;
- remaining M2 appearance/control scope; camera movement/zoom belongs to M5;
- output geometry/projection, fixture mapping, transport and controller contract;
- remaining appearance/brightness controls and visual acceptance; future-date
  motion handling remains M5 work.

Owner requirements and proposals are distinguished from agent explanations and
implementation state in the [ceiling/output decision note](decisions/ceiling-product-output.md).
Camera/time and device implementation remain future scope beyond the implemented
controls and initial colour path. The understanding
checkpoint remains open.

## Key records

- [Star ceiling and output boundaries](decisions/ceiling-product-output.md)
- [M1 fixed Toulouse sky](milestones/m1-fixed-toulouse-sky.md)
- [Gaia acquisition](gaia-datasets.md)
- [Hipparcos-2 supplement](hipparcos2-datasets.md)
- [Catalogue integration and Stellarium reference](decisions/catalogue-integration.md)
- [Stellar proper-motion propagation](decisions/stellar-motion.md)
- [Development discussion history](development-history/README.md)
- [Project objectives and decision history](objectives-and-decisions.md)
