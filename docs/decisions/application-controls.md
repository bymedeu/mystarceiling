# Application controls and interface direction

Status: requirements clarified on 2026-09-13 and 2026-10-18; controls are not
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

Selection remains magnitude based, brightest first. Catalogue size and limiting
magnitude are independent concepts. The count option below is a proposal, not
approved implementation or a spatial-selection requirement:

- a record-count limit selects the first N records of the common
  brightness-ordered catalogue and defines reproducible performance workloads;
- a limiting magnitude selects every loaded record bright enough for a chosen
  visual/observational policy, so its resulting count varies.

Normal visualization must consume prepared runtime data. Catalogue acquisition,
normalization, and binary regeneration are separate explicit commands or
endpoint-like operations; the application must not rebuild the binary at every
launch. No configuration file is currently required. If one is introduced,
explicit CLI arguments should override it.

## GUI direction

Using a small external GUI library is acceptable. The preferred workflow is one
visualization window with a collapsible settings panel. The panel has no permanent
on-screen button; a keyboard shortcut is an approved secondary access method.
An additional pointer gesture such as double-click remains undecided.

Settings persistence is not enabled by default and remains optional. Recalculation
should preserve the existing frame until a replacement is ready when practical.
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

M2 proposes realistic-looking colour/brightness, modest glow and useful bounded
inspection/projection where agreed. A quantity cap remains proposed. Screenshot
export is optional. A brightness slider is not exposure integration.

Long-exposure simulation remains an ambition in conditional M9 backlog, without
a mandatory date or full-model release promise. A bounded visible-light prototype
could be the single optional September–December 2027 extension. It should integrate
over time mathematically and be physically motivated and validated for its
bounded model. Tracked/fixed cameras, optics, sensor, atmosphere/background and
filter responses remain separate open choices. Full IR requires additional data
and wavelength/sensor/filter models; current G/BP-RP or RGB cannot supply it.
See [appearance and data limits](ceiling-product-output.md#appearance-exposure-and-data-limits).
