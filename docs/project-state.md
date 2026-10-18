# Current project state

Last updated: 2026-10-18.

This is the short hand-off record for continuing MSC from another account or
session. Detailed decisions remain in the linked focused documents.

M1 is complete on 2026-09-13. M2 — scalable and inspectable stellar rendering —
is the next planning target.

The first bounded M2 refactor separates source-independent
`core::astronomy::Star` and its measurement wrappers from
`catalogue::CatalogueStar` identity/provenance. See the
[boundary decision](decisions/catalogue-integration.md#first-bounded-m2-refactor--owner-decision-2026-09-13).
The version-2 binary contract and M1 scene behavior are preserved. The recorded
post-refactor suite passed all 119 tests, including a frozen-byte compatibility check against
the pre-refactor writer.

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

## Implemented and verified

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

## Future direction, not implemented

Automatic visibility and future CLI/GUI controls remain future work. Current
local time is the approved future time default; the default observer and
place-name resolution remain open. M1 still uses fixed Toulouse/time. Colour,
glow, exposure, camera controls, and scintillation are not implemented.

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

The latest Hipparcos completed run is `20260912T191816.016104Z`, tied to the
previous 76,048,576-row Gaia snapshot. Its overlap preparation has not been
revalidated against completed Gaia. Regeneration requires separate authorization
and was not performed. Do not treat the old supplement as freshly prepared for
the final archive.

## Open owner decisions

- classification taxonomy/bit allocation and future metadata layout;
- richer native/derived photometry beyond the bounded runtime contract;
- conflict reporting for otherwise usable source records;
- whether magnitude layers first appear during M1 binary preparation or after
  measuring the existing 4,096-row baseline;
- full M2 scope/order, including camera scope;
- output geometry/projection, fixture mapping, transport and controller contract;
- preparation-time colour/radiance conversion and runtime/cache representation.

Owner requirements and proposals are distinguished from agent explanations and
implementation state in the [ceiling/output decision note](decisions/ceiling-product-output.md).
No launcher, camera/time, colour/cache or device implementation is authorized by
these documentation clarifications. The understanding checkpoint remains open.

## Key records

- [Star ceiling and output boundaries](decisions/ceiling-product-output.md)
- [M1 fixed Toulouse sky](milestones/m1-fixed-toulouse-sky.md)
- [Gaia acquisition](gaia-datasets.md)
- [Hipparcos-2 supplement](hipparcos2-datasets.md)
- [Catalogue integration and Stellarium reference](decisions/catalogue-integration.md)
- [Stellar proper-motion propagation](decisions/stellar-motion.md)
- [Development discussion history](development-history/README.md)
- [Project objectives and decision history](objectives-and-decisions.md)
