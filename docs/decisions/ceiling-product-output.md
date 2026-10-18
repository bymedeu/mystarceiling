# Star ceiling, product uses and output boundaries

Status: owner clarifications recorded on 2026-10-18; documentation only
(MODE GREEN). Requirements below describe direction, not implementation approval.
The [catalogue understanding checkpoint](../project-state.md#required-understanding-checkpoint)
remains open.

## Owner requirements and priorities

The main product is a star ceiling with twinkle/scintillation and optional events
such as shooting stars. Its two main uses are a fixed astronomical sky with
effects and a sky with astronomical time progression. A completely static image
is secondary. Screenshot export is only an idea, not required scope.

Catalogue selection remains magnitude based, brightest first. A count option is
an owner proposal, not approved implementation or a mandate for spatial
selection. Camera exploration is secondary. Full M2 scope and ordering remain
to be agreed, including its camera scope. Existing milestones and target dates
are retained pending agreement; astronomical time remains M3 in the current
[roadmap](../../README.md#milestones).

Physical consumers could include LED panels or installations. Logical output
records must be separable from device mapping, projection/geometry and transport.
Desired consumer-facing examples include pixel/device-element colour and
intensity, or an assigned animation. These examples do not replace structured
celestial-object data as the astronomy/simulation output or select a schema.

Desired flexibility includes rectangular outputs, fisheye views and curved
hemispherical/bubble fixtures. A hemispherical surface is physical geometry;
fisheye is a projection. Neither determines the other.

## Owner proposal: prepared colour and radiance

The owner proposes deriving rendering colour/radiance from BP-RP and G during
ingestion/preparation, potentially retaining compact RGBE or an equivalent in a
runtime representation instead of the original photometry. Source measurements
can remain in the source catalogue while a device/render cache contains only
derived data it needs. Click-to-inspect is optional and no longer drives the
representation.

Agent explanation: BP-RP/G conversion to display colour and radiance is
approximate. Arbitrary synthetic or artistic colours need not fabricate BP-RP.
Keeping measurement provenance separate from display appearance remains useful;
it does not require every runtime layer to retain every source field.

Open decisions: representation and layer boundary, colour model and packing.
RGBE is a candidate, not a selected format. No photometry conversion, cache or
binary-schema change is approved or implemented by this clarification.

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

The current application still renders the fixed M1 Toulouse scene with grayscale
points. The bounded source-independent Star refactor is implemented; colour,
scintillation, events, camera/time controls and hardware output are not added by
this work. See [project state](../project-state.md) and
[application controls](application-controls.md) for existing behavior and plans.

## M1 comparison correction

Ten years alone does not explain gross changes in familiar stellar patterns.
The owner's comparison involved different season, hour and view (the fixed
January zenith crop versus a September east-southeast view). Those differences
can change the visible sky; they do not disprove a mismatch. No matched
reference comparison has been performed. M1 visual acceptance and automated
checks remain recorded evidence with their stated limits, not a resolved
independent sky comparison. See the [M1 record](../milestones/m1-fixed-toulouse-sky.md#visual-closeout--2026-09-13)
and [discussion history](../development-history/2026-10-18.md).
