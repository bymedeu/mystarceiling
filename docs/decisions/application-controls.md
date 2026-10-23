# Application controls and interface direction

Status: requirements clarified through 2026-10-23; controls are not
implemented. The [ceiling/output note](ceiling-product-output.md) records current
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

## Catalogue controls

Approved requirements, not implementation approval: normal visualization consumes
prepared runtime data. Acquisition, normalization and binary regeneration are
separate explicit commands; launching must not rebuild the binary.

Catalogue builds write a temporary file in the target's **same directory** and
replace the target only after successful completion. Existing targets require
`--force`. A requested build capacity is **exact**: insufficient usable input is
an error, superseding the earlier upper-bound-with-warning proposal. Report the
requested and available counts. When a valid alternative is known, show an
otherwise-identical corrected command; never suggest a quantity below 64 or claim
an exact maximum before consuming enough input to establish it. Give advice for
the actual failure only, without universal guesses or echoing secrets to stdout.
Only the command that consumes a source checks its existence. These are owner
requirements for future implementation; the understanding checkpoint stays open.

Quantity caps loaded records before scene construction. The owner's 2026-10-22
contract requires an integer with `64 <= quantity <= available_count`, where
`available_count` is the opened prepared catalogue record count; equality is allowed.
Reject values below 64 or above availability,
including zero, negatives, malformed values and overflow. Excess is an error,
superseding warn-and-clamp. A prepared catalogue below 64 records cannot satisfy
this viewing contract. CLI quantity overrides TOML before validating the effective
quantity against the opened catalogue count; never adjust quantity automatically.
The assistant recommends an initial quantity of 4096 from M1, not an owner-selected
permanent default.

Selection is global magnitude-first, independent of camera, time and view
visibility. Quantity selects the first N records of the common brightness-ordered
prepared catalogue; a limiting magnitude filters loaded records and may yield
fewer visible stars. Zoom does not automatically deepen limiting magnitude.
Any future explicit `all` syntax remains undecided; missing quantity is not
implicit `all`.
Quantity bounds work, not the need for later measured scaling evidence.

## Launch configuration

Every launch checks the chosen config path. The owner requires a complete TOML
file containing all currently implemented settings. Preserve an existing file;
invalid configuration or missing required fields must fail clearly, without
silent fallback. If absent, generate the file with initial defaults. Explicit CLI
values override TOML for that launch, including the first run, but apply only in
memory and are not written into the newly generated file. CLI changes remain
temporary unless explicit saving is chosen later.

Automatic user configuration discovery and an explicit config-path override are
accepted direction. Linux honors `XDG_CONFIG_HOME`, falling back to `~/.config`.
To avoid collisions with the short name `msc`, the owner approved
`mystarceiling/config.toml` beneath that directory. Windows `APPDATA` is a future
portability direction.

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
