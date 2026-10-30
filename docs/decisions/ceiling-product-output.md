# Star ceiling, product uses and output boundaries

Status: owner clarifications updated on 2026-10-28; documentation only
(MODE GREEN). Requirements below describe direction, not implementation approval.
The [catalogue understanding checkpoint](../project-state.md#required-understanding-checkpoint)
remains open.

## Owner requirements and priorities

The main product is a star ceiling with twinkle/scintillation and optional events
such as shooting stars. Its two main uses are a fixed astronomical sky with
effects and a sky with astronomical time progression. A completely static image
is secondary. Screenshot export is only an idea, not required scope.

Catalogue building now uses the implemented Hipparcos-first capacity policy,
then stores the selection in brightness order. View quantity controls the loaded
prefix; neither policy mandates spatial selection. Camera exploration is secondary.
Remaining M2 appearance/control scope stays open; camera movement and zoom are
assigned to M5 alongside observer/time validation. Existing dated estimates have
not been recalculated for the owner's 2026-10-28 scope revision; see the current
[roadmap](../../README.md#milestones).

LED hardware is illustrative only, not a required product target.
Physical consumers could include LED panels or installations. Logical output
records must be separable from device mapping, projection/geometry and transport.
Desired consumer-facing examples include pixel/device-element colour and
intensity, or an assigned animation. These examples do not replace structured
celestial-object data as the astronomy/simulation output or select a schema.

Desired flexibility includes rectangular outputs, fisheye views and curved
hemispherical/bubble fixtures. A hemispherical surface is physical geometry;
fisheye is a projection. Neither determines the other.

## Accepted catalogue preparation — 2026-10-25

Owner-selected direction for M2, now implemented for catalogue preparation and
the initial coloured viewer:

- Convert Gaia and Hipparcos photometry into common linear RGB8 colour during
  binary building; the owner selected compact uint8 channels over floats. Keep observed colour, including existing
  interstellar reddening; synthetic colours use the same RGB interface.
- Preserve magnitude as a separate double-precision baseline for core algorithms.
  Accept the existing estimated Gaia G for Hipparcos. Common photometric units
  do not mean averaging different stars' magnitudes; overlapping light is later
  combined in linear intensity.
- Document how source fields are converted or fused into common fields. Original
  values need not be recoverable from the binary; retain the source CSVs so better
  conversions can regenerate it. Do not discard useful stellar inputs silently.
- Leave Hipparcos `g_estimation_method` and `resolution_reason` out of the binary.
  They remain preparation/debugging metadata in the source files.
- Move the existing common-epoch position preparation into building, targeting
  the current J2016 baseline. Accept the existing simplified motion approximation;
  a more accurate astrometric model is not an M2 prerequisite. Epoch labels and
  associated motion assumptions must remain explicit, and the viewer must not
  repeat the source-to-common-epoch displacement. See [motion limits](stellar-motion.md).
- Store complete, valid records, including usable RGB for either source. Skip
  malformed/incomplete source rows and report warnings. This rejection policy
  already exists in catalogue building; Gaia acquisition also filters missing
  fields and validates rows before writing the source archive.

The binary holds stellar baselines. Owner clarification, 2026-10-30: catalogue
magnitude supplies the initial brightness/visibility input; the view may derive
its own working light intensity and modify it as later effects are introduced.
Preserve the original magnitude rather than rewriting it with those effects.
This records the data flow, not a selected future visibility formula. A
presentation conversion supplies values to the simple UI. M2 starts with an ideal
night sky. Light pollution, clouds and Sun/Moon effects remain later work rather
than additional M2 requirements.

The owner accepts colour-conversion and motion-model approximation errors to
keep M2 proportional to the star-ceiling objective. A calibration campaign or
full astrometric upgrade would exceed the intended scope. Basic implementation
checks still cover finite values, documented conversions, epoch handling and
binary round trips; accepted model error does not imply accepting inconsistent
units, corrupted records or applying a displacement twice.

The concrete encoding is three linear-sRGB uint8 channels, with source-specific
temperature approximations documented in the [colour decision](stellar-colour.md).
Binary version 3 stores prepared J2016 positions and retains native motion values
under the stated approximation. The earlier RGBE/float packing proposals are
superseded. No glare, contrast/detectability or future-date motion model is added.

## Deferred accuracy work

These are optional later improvements, not new M2 acceptance requirements or
release promises:

- Refine source-index-to-RGB conversion against observed reference spectra,
  compare colour errors across stellar populations, and consider richer input
  photometry/spectra if justified. More storage bits alone cannot improve a
  conversion model; retained CSVs permit rebuilding after model improvements.
- Revisit consistent motion-state propagation, radial velocity and perspective
  effects with M5 observer/time planning. Declare the supported model/date range
  and limitations there; full astrometric accuracy is not promised.

## Proposed perceived colour

The owner proposes reducing saturation toward grey as visibility decreases in
the naked-eye view. This follows perceived visibility rather than catalogue
magnitude alone. Preserve original catalogue RGB as the baseline and keep any
perceptual treatment separate. No visibility algorithm, threshold or
implementation is selected or approved. This remains a future appearance
decision, rather than an unspecified extra M2 implementation requirement.

## Appearance, exposure and data limits

“Animated” means realistic-looking colour, modest glow and subtle effects.
Meteors/Perseids/clouds are possible later, not mandatory near-term feature count.
The owner accepts deterministic artistic long-exposure/universe imagery.
Procedural gas, dust and nebula content may be invented, but must be labelled
synthetic/artistic rather than reconstructed from Gaia. This adds no mandatory
scope or implementation approval. A bounded visible-light exposure prototype
remains one possible optional extension; full photographic realism is not a
release promise. See [M2 planning](../milestones/m2-ceiling-appearance.md).

Visible exposure changes can use linear HDR radiance/intensity derived from
RGB colour and magnitude, plus a scene/time model. A final clipped or tone-mapped
image alone cannot recover lost range or generate motion integration. Time integration is distinct from a
brightness slider; tracking, optics/PSF/aperture, sensor noise/response/saturation
and atmosphere/background remain explicit model choices.

A display consumes image updates; it need not know astronomy or carry spectra.
Artistic colour treatment needs no real spectrum. A different physically
wavelength-selective filter requires spectral/multiband information or an explicit
approximation: G + BP-RP/display RGB cannot uniquely recover spectra, infrared
flux or nebular emission. RGBE does not retain spectroscopy. Real filtered
astrophotography can be physically grounded; the limitation concerns information
available to this model, not all filtering. No filter model is selected.

The owner welcomes the 134,217,728-source workload as an optimisation challenge.
This promises neither simultaneous interactive rendering on the 16 GB laptop nor
a change to core deadlines. Define offline throughput separately from interactive
latency before making performance claims. No memory layout, indexing, streaming
or renderer algorithm is selected. See [current memory evidence](memory-accounting.md).

The owner is concerned about duplicating physical photometry and derived pixels.
Different representations need not coexist at every stage. Compact prepared
records may omit unused fields from hot memory while reproducible source data,
including future spectra, remain offline on disk. Do not irreversibly discard
sources. Magnitude remains a baseline field; source colour indices may be
converted to common RGB under the accepted direction above. Exact packing still
requires a bounded implementation plan. No future packed representation follows
from [current size accounting](memory-accounting.md).

Multiple consumers/renderers need explicit output boundaries; no do-everything
schema is selected. Cloud/display effects may adapt, degrade or be omitted
according to consumer capability, resolution and geometry. Policy remains open;
no arbitrary global dimension threshold is chosen.

## Shared-animation candidate

Owner clarification, 2026-10-30: a shared animation can start at a different
state for each star. Deterministic starting offsets can avoid synchronized
flicker without storing an independent animation per source. This is a candidate
for measured comparison after M2, not an implemented optimization. Preserve RGB
variation, horizon dependence, exposure behavior and the accepted appearance
when evaluating it; a lookup trades arithmetic for memory accesses. No texture
format, animation length or approximation tolerance is selected.

## Agent explanations and recommendations

Camera orientation and astronomical time are conceptually independent inputs.
A simplified rotational component may share or invert frame transforms. This
does not select camera rotation as a full astronomy model: proper motion and
other time-dependent effects remain separate. No camera/time code is authorized.

A proposed no-argument launcher would be GREEN if it only resolves paths and
invokes the existing CLI. Adding prepare/view modes or changing the lifecycle
would require a separate bounded plan. No launcher implementation is approved.
The previous assistant's claim of immediate difficulty reflected its
recommendation about user convenience; it was neither a technical blocker nor
an owner-selected priority.

## Open decisions and implementation state

No wire protocol, animation placement (engine or controller), update rate,
controller capability, geometry, fixture mapping, projection or ID scheme is
selected. Device output remains direction for design discussion.

The ordinary viewer uses the fixed Toulouse/time geometry with prepared
RGB8 colour, limiting magnitude 6, scintillation, glow and unequal rays.
The owner requested promotion to plain `msc view` and subsequently accepted
the stronger native comparison as the new ×1 baseline, with a 30 fps ceiling.
Temporary comparison controls and guides are removed. See the
[viewer model and acceptance](stellar-appearance-trial.md). Events, camera/time
controls and hardware output remain future work. See [project state](../project-state.md) and
[application controls](application-controls.md) for existing behavior and plans.

## M1 comparison correction

The owner considers this mismatch low priority. No new accuracy claim or matched
comparison was made during the planning revision.

Ten years alone does not explain gross changes in familiar stellar patterns.
The latest comparison is with tonight's sky on 2026-10-18; its exact observing
hour and direction have not been established here. The September east-southeast
observation belongs to the historical M1 closeout. The fixed January zenith
crop differs in season from both observations. Hour and view must also be
matched before drawing conclusions; these possible differences do not disprove
a mismatch. No matched reference comparison has been performed. M1 visual
acceptance and automated checks remain evidence with their stated limits, not
a resolved independent sky comparison. See the [M1 record](../milestones/m1-fixed-toulouse-sky.md#visual-closeout--2026-09-13)
and [discussion history](../development-history/2026-10-18.md).
