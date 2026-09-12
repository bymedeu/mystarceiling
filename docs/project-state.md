# Current project state

Last updated: 2026-09-12.

This is the short hand-off record for continuing MSC from another account or
session. Detailed decisions remain in the linked focused documents.

## Implemented and verified

- M1 has an end-to-end 4,096-row Gaia path from CSV-gzip parsing through the
  typed catalogue, binary round trip, fixed Toulouse transformation, zenith
  projection, and GLFW/OpenGL rendering.
- The current C++ automated suite contains 96 discovered tests and passed at the
  last recorded verification.
- The display uses fixed-size grayscale points with a minimal relative Gaia G
  brightness cue. Colour, calibrated exposure, scintillation, and proper-motion
  propagation are not implemented.
- Hipparcos-2 offline preparation derives Gaia-like G from Hp/B-V with an Hp/V-I
  fallback, cross-matches 63 bright candidates, and produces a resolved 62-row
  supplement. Ten focused Python tests pass.
- Build/include/test cleanup and portfolio-level agent-contract consolidation
  are complete in the current commit history.

## Approved, not yet implemented

- Keep Gaia and Hipparcos source parsers separate, resolve them offline, and
  normalize accepted records into one source-independent runtime catalogue.
- Preserve separate measurement, calculated-state, and visual-state boundaries.
- The application should ultimately load the normalized combined catalogue;
  current C++ loading still consumes the Gaia-specific M1 input.

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

- normalized source identity and provenance representation;
- representation of native versus derived photometry;
- conflict reporting for otherwise usable source records;
- whether magnitude layers first appear during M1 binary preparation or after
  measuring the existing 4,096-row baseline;
- the remaining M1 screenshot/acceptance record;
- the Gaia acquisition recovery strategy.

## Key records

- [M1 fixed Toulouse sky](milestones/m1-fixed-toulouse-sky.md)
- [Gaia acquisition](gaia-datasets.md)
- [Hipparcos-2 supplement](hipparcos2-datasets.md)
- [Catalogue integration and Stellarium reference](decisions/catalogue-integration.md)
- [Project objectives and decision history](objectives-and-decisions.md)
