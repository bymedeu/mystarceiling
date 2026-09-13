# Current project state

Last updated: 2026-09-13.

This is the short hand-off record for continuing MSC from another account or
session. Detailed decisions remain in the linked focused documents.

M1 is complete on 2026-09-13. M2 — scalable and inspectable stellar rendering —
is the next planning target.

The first bounded M2 refactor separates source-independent
`core::astronomy::Star` and its measurement wrappers from
`catalogue::CatalogueStar` identity/provenance. See the
[boundary decision](decisions/catalogue-integration.md#first-bounded-m2-refactor--owner-decision-2026-09-13).
The version-2 binary contract and M1 scene behavior are preserved. The current
suite passes all 119 tests, including a frozen-byte compatibility check against
the pre-refactor writer.

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

The latest verified Gaia checkpoint contains 76,048,576 complete, unique,
brightness-ordered records. The target remains 134,217,728 records, subject to
the 10,000,000,000-byte compressed-file limit. A final Gaia catalogue does not
exist until acquisition verification creates `manifest.json`.

ESA failed repeatedly while submitting or transferring batch 26. The retry loop
reached restart 58 without adding rows and was stopped at 19:04 local time on
2026-09-12. That transient service no longer exists.

The subsequent 2026-09-12 recovery attempt reduced batch size from 1,500,000 to
750,000 rows. `msc-gaia-acquisition-750k.service` ran once without automatic
restart. ESA async submission timed out after 300000 ms with zero bytes received
(curl exit 28); no job was recorded. The checkpoint remains 76,048,576 rows and
4,878,012,154 bytes. The service is stopped/failed; the checkpoint is intact and
resumable, but acquisition remains incomplete. Further recovery implementation
requires separate owner approval.

The latest Hipparcos completed run is `20260912T191816.016104Z`. It records the
76,048,576-row Gaia snapshot used to exclude one already represented star.
Regenerate the supplement after Gaia materially changes.

## Open owner decisions

- classification taxonomy/bit allocation and future metadata layout;
- richer native/derived photometry beyond the bounded runtime contract;
- conflict reporting for otherwise usable source records;
- whether magnitude layers first appear during M1 binary preparation or after
  measuring the existing 4,096-row baseline;
- the Gaia acquisition recovery strategy.

## Key records

- [M1 fixed Toulouse sky](milestones/m1-fixed-toulouse-sky.md)
- [Gaia acquisition](gaia-datasets.md)
- [Hipparcos-2 supplement](hipparcos2-datasets.md)
- [Catalogue integration and Stellarium reference](decisions/catalogue-integration.md)
- [Stellar proper-motion propagation](decisions/stellar-motion.md)
- [Development discussion history](development-history/README.md)
- [Project objectives and decision history](objectives-and-decisions.md)
