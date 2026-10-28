# Application controls and interface direction

Status: requirements clarified through 2026-10-28. The bounded reader,
configured viewer, logging and catalogue build/info commands are implemented.
The [ceiling/output note](ceiling-product-output.md) records current
owner priorities, proposals and open decisions; it does not approve implementation.

## Shared behavior

The CLI and GUI must provide settings to the same validated application path.
They must not duplicate catalogue selection, astronomy, visibility, projection,
or appearance calculations. Invalid explicitly supplied values fail rather than
silently falling back. Cheap camera/appearance changes may update immediately;
location, time, or catalogue changes may use an Apply action while the current
visualization remains visible. A change must not require restarting the app.

## Location and time

- Toulouse is the fixed M1 observer chosen for owner inspection, not the
  permanent application default.
- The future time default is the current local instant, converted internally to
  the declared astronomical time representation.
- An explicitly supplied timestamp without a timezone is interpreted as UTC.
- Future time controls must support a custom instant, pause/resume, forward and
  reverse progression, and adjustable rate.
- When location changes, the camera should retain its orientation relative to
  the local ground rather than retaining an unrelated celestial orientation.
- Place text such as `Toulouse, France` is desirable. Converting it requires a
  resolver. An online geocoder conflicts with the offline-first runtime unless
  invoked explicitly; a packaged/local gazetteer adds data and maintenance.
  The resolver and default observer remain open decisions.

## Command interface

The owner accepted `msc view`, `msc catalogue build`, `msc catalogue info`,
`msc --help` and `msc --version`. Source options are
`--gaia` and `--hipparcos`, matching the `gaia` and `hipparcos` keys in `[sources]`.
This accepts the interface direction, not implementation.

The executable supports `msc view`, `msc catalogue build`, `msc catalogue info`,
help and version. The former positional source-building launcher is removed.
Build/info use the same strict configuration and temporary override path as view.

User-facing help and runtime output use product terms rather than milestone
names. Milestone names remain appropriate in development documentation.

## Catalogue controls

Implemented part-1 behavior: normal visualization consumes
prepared runtime data. Acquisition, normalization and binary regeneration are
separate explicit commands; launching must not rebuild the binary.

Catalogue builds write a temporary file in the target's **same directory**,
finish and check writing/closing, and check its structure, records and ordering
before publication. Failure removes the temporary file during normal cleanup
and preserves the previous catalogue. Existing targets require `--force`;
the destination must not overwrite either input source.
Building capacity is an integer of at least 64, initially 4096, independent of
viewing quantity. Existing source parsers skip unusable rows; retain that policy
with aggregate warnings. File-level failures stop the build.
A requested build capacity is **exact**: insufficient usable input is
an error, superseding the earlier upper-bound-with-warning proposal. Report the
requested and available counts. When a valid alternative is known, show an
otherwise-identical corrected command; never suggest a quantity below 64 or claim
an exact maximum before consuming enough input to establish it. Give advice for
the actual failure only, without universal guesses or echoing secrets to stdout.
Only a command that consumes a source opens it. Log destinations cannot alias
sources, the binary or the configuration. Build destinations also cannot overwrite
the configuration.

Quantity caps loaded records before scene construction. The owner's 2026-10-22
contract requires an integer with `64 <= quantity <= available_count`, where
`available_count` is the opened prepared catalogue record count; equality is allowed.
Reject values below 64 or above availability,
including zero, negatives, malformed values and overflow. Excess is an error,
superseding warn-and-clamp. A prepared catalogue below 64 records cannot satisfy
this viewing contract. CLI quantity overrides TOML before validating the effective
quantity against the opened catalogue count; never adjust quantity automatically.
The initial generated viewing quantity is 4096, retaining the M1 baseline.

Owner selection change, 2026-10-24: building gives usable Hipparcos records
priority, then fills the exact remaining capacity from the ordered Gaia source.
If Hipparcos alone fills capacity, keep its brightest N and do not open Gaia.
This supersedes global brightest-N membership across both sources. The selected
records are still sorted by G-like magnitude, catalogue and native ID before
storage, independently of camera, time and view visibility.
Quantity selects the first N records of this brightness-ordered prepared
catalogue; a limiting magnitude filters loaded records and may yield
fewer visible stars. Zoom does not automatically deepen limiting magnitude.
Any future explicit `all` syntax remains undecided; missing quantity is not
implicit `all`.
Quantity bounds work, not the need for later measured scaling evidence.

The owner accepted prefix-only viewing validation: check the header, complete
file length and requested count before allocating for and decoding only the
requested records. The complete file must match the header's total count
(`32 + count * 68` bytes in version 3), independently of the viewing quantity.
Catalogue building establishes brightness ordering within the stored selection;
viewing does not re-sort or scan the unused tail to re-establish it. Prefix
loading does not claim to check unconsumed records.

`catalogue info` reports the resolved catalogue path, stored record count and
file size in bytes. Records are stars stored in the binary, independently of
viewing quantity. The accepted output is:

```text
Catalogue: /path/to/catalogue.bin
Records: 4096
File size: 303136 bytes (0.000303 GB)
```

The byte count is exact. GB means 1,000,000,000 bytes. Use ordinary decimal
notation with three decimal places at or above 0.001 GB; below that, retain
leading zeros and three significant digits so a nonempty catalogue never rounds
to zero GB. Format/schema identifiers remain internal compatibility checks in
the binary header and are not displayed by info. It reads the header and checks
file length without scanning the records.
Slice 1 has no public validation command or status, content hashing, saved
verification records, or associated configuration/dependency. Building must
succeed with a complete, correct catalogue or fail before publication.
Magnitude-range and spatial build filters remain future ideas.

## Launch configuration

Every launch checks the chosen config path. The owner requires a complete TOML
file containing all currently implemented settings. Preserve an existing file;
invalid configuration or missing required fields must fail clearly, without
silent fallback. If absent, generate the file with initial defaults. Explicit CLI
values override TOML for that launch, including the first run, but apply only in
memory and are not written into the newly generated file. CLI changes remain
temporary unless explicit saving is chosen later.

The complete schema contains `catalogue.path`, `catalogue.capacity`,
`sources.gaia`, `sources.hipparcos`, `view.quantity`, `logging.file` and
`logging.level`; see the [usage and TOML example](../../README.md#run-the-prepared-catalogue-viewer).
All fields are required. Existing files with missing fields fail clearly and are
preserved. CLI overrides are `--gaia`, `--hipparcos` and `--capacity` for building,
`--quantity` for viewing, and `--catalogue` (alias `--path`) for each command.
Building creates missing destination parent directories and the binary file.

Unknown TOML keys and logging levels are errors naming the invalid setting.
Validate the complete TOML structure (including required fields and types), then
apply CLI overrides before validating effective command settings. In particular,
a saved quantity exceeding the opened catalogue count may be overridden by a
valid CLI quantity; a missing required TOML field still fails.

Automatic user configuration discovery and an explicit config-path override are
accepted direction. Linux honors `XDG_CONFIG_HOME`, falling back to `~/.config`.
To avoid collisions with the short name `msc`, the owner approved
`mystarceiling/config.toml` beneath that directory. Windows `APPDATA` is a future
portability direction.

The accepted Linux data root is `$XDG_DATA_HOME/mystarceiling`, falling back to
`~/.local/share/mystarceiling`. Its default layout is:

- `catalogue.bin` for the prepared binary;
- `sources/gaia-dr3.csv.gz` for Gaia;
- `sources/hipparcos2-supplement.csv.gz` for the prepared Hipparcos input.

Existing datasets may remain in their current locations and be referenced by
TOML. Accepting this layout does not authorize moving or regenerating them.

## Logging

Logging means spdlog messages only; command results (including help, version,
build completion and catalogue info) keep their normal terminal output. The owner requires
a configurable destination and level, with the initial level `warning` mapped
to `spdlog::level::warn` (warnings, errors and critical messages).
The viewer's above-horizon source count and renderer magnitude-limit summary is an
`info` log, hidden at the default level and sent to the configured destination
when enabled.

A nonempty file path directs enabled spdlog messages to that file, creating it if
absent; an empty path uses stdout/stderr. The accepted default is
`/tmp/mystarceiling.log`. Overwrite existing contents when initializing file
logging for a new run; messages within that run accumulate normally. If the file
cannot be opened, report the failure to stderr and stop. This behavior is
implemented for the configured viewer.

## GUI direction

Using a small external GUI library is acceptable. The preferred workflow is one
visualization window with a collapsible settings panel. The panel has no permanent
on-screen button; a keyboard shortcut is an approved secondary access method.
An additional pointer gesture such as double-click remains undecided.

Saving interactive settings remains a future choice; launch configuration follows
the TOML contract above. Recalculation should preserve the existing frame until a
replacement is ready when practical.
Visual controls such as magnitude/exposure and camera movement can update live;
astronomical or catalogue changes can use Apply. Exact behavior remains
context-dependent rather than one universal rule.

Plain `msc view` always opens the [animated viewer](stellar-appearance-trial.md),
as requested by the owner on 2026-10-28. Both `--appearance-trial` and
`--animation-trial` reject during CLI parsing, before configuration creation or
reading, logging setup, catalogue reading or window launch. No alias, toggle or
replacement setting exists. The former static renderer is removed.
On 2026-10-27 the owner rejected extra appearance options and tuning shortcuts.
Space freezes/resumes picture mode, Escape closes the window and resizing remains
available. Limiting magnitude stays 6; model coefficients remain C++ defaults
and are not user settings. The strict saved TOML schema is unchanged. A possible
later JSON customization mechanism is [deferred for discussion](../todo.md#later-customization).
The owner clarified two pause meanings: astronomical time pause allows effects
to continue; picture mode also freezes animation. The current fixed astronomical
scene already has paused time.

## Milestone boundary

The [revised roadmap](../../README.md#milestones) estimates M2 at 2026-12-13
and M3 at 2027-02-28; scope remains to discuss. Camera exploration is secondary
and its M2 extent remains open. Location, full date/time input and astronomical
time progression remain M3 work. Camera orientation and astronomical time are
independent; share appropriate coordinate mathematics where justified without
forcing a universal implementation.

M2 follows the owner-approved [four-part sequence](../milestones/m2-ceiling-appearance.md#approved-sequence).
Useful bounded inspection/projection remains to agree. Screenshot
export is optional. A brightness slider is not exposure integration.

Long-exposure simulation remains an ambition in conditional M9 backlog, without
a mandatory date or full-model release promise. A bounded visible-light prototype
could be the single optional September–December 2027 extension. It should integrate
over time mathematically and be physically motivated and validated for its
bounded model. Tracked/fixed cameras, optics, sensor, atmosphere/background and
filter responses remain separate open choices. Full IR requires additional data
and wavelength/sensor/filter models; current G/BP-RP or RGB cannot supply it.
See [appearance and data limits](ceiling-product-output.md#appearance-exposure-and-data-limits).
