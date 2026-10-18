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

The existing roadmap places camera movement and zoom in M2. The owner now
clarifies that camera exploration is secondary; M2 camera scope and full scope/order
remain open for discussion. No established milestone or target date is removed
or moved by this clarification. Location, full date/time input and astronomical
time progression remain M3 work. Camera orientation and astronomical time are
independent; see the [transform explanation](ceiling-product-output.md#agent-explanations-and-recommendations).

M2 planning includes star colour, glow/PSF and exposure appearance controls;
the full scope is still to be agreed. Screenshot export is only an idea, not
required scope. A static brightness multiplier must not be called a long exposure.

The owner requires real astrophotography and long-exposure simulation in M9;
this is accepted scope, not merely a suggested brightness preset. Long exposure
must model radiance integrated over an exposure duration. Full M9 work may support
tracked and fixed cameras/star trails, sensor response/saturation/noise, optical
PSF, atmosphere/light pollution, photographic colour/tone mapping, and
reproducible high-resolution output. It depends on M3 time and may consume M8
bodies. Detailed models, parameter values, and APIs remain undecided. None of
this simulation is implemented by the current appearance mapping.
