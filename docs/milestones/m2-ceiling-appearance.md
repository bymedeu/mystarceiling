# M2 — Ceiling appearance and data controls

Planning note, 2026-10-19. **Estimate: 13 December 2026; scope to discuss**, not
implementation approval. The primary assistant proposed this date; the owner
did not choose the intermediate milestone dates. The estimate has not been
recalculated for the 2026-10-28 roadmap revision and renumbering. See the
[roadmap](../../README.md#milestones). The
[owner-understanding checkpoint](../project-state.md#required-understanding-checkpoint)
remains open before substantial implementation.

## Approved sequence

Discuss, then implement/review each part in order:

1. Catalogue/launch workflow, configuration and quantity.
2. Colour/brightness.
3. Glow/subtle animation.
4. Validation/closeout.

Scope approval is not blanket implementation approval. Do not design every later
detail now. Part 1 follows the approved requirements, assistant recommendations
and open choices in [application controls](../decisions/application-controls.md#catalogue-controls)
and [launch configuration](../decisions/application-controls.md#launch-configuration).
Path-prediction/generation speed modes are future ideas, not M2 requirements.

## Bounded scope

Build on the working first 4096-source M1 path and the recorded 119 passing C++
tests after the source-independent Star refactor. Aim for realistic-looking
colour, useful brightness controls, modest glow and subtle effects. “Animated”
does not mandate meteors, Perseids or clouds now. The owner accepts approximate
source-photometry-to-RGB conversion and the current simplified motion model.
Compact RGB8 and separate double-precision magnitude are the selected
preparation direction. Check implementation correctness, record model limits,
and defer a colour-calibration campaign and astrometric accuracy upgrades; see
[accepted preparation and deferred accuracy work](../decisions/ceiling-product-output.md#accepted-catalogue-preparation--2026-10-25).

Building follows the Hipparcos-first membership policy and stores its selection
in brightness order; quantity caps loaded records before visibility and display
filtering, as specified in application controls. It bounds
workload size, without solving scaling or establishing that a million sources
have been processed efficiently. Nested 4096, 65536, 262144 and 1048576
sizes are examples, not mandatory gates or a requirement to show every star.
M3 establishes the benchmarking suite; M6 applies it to high-usage workloads and
bounded-memory preparation/runtime, including the historical large-catalogue
milestone (formerly M7).

Owner revision, 2026-10-28: camera movement and zoom belong to M5 alongside
validated observer/time controls. Click-to-inspect and screenshot export remain
optional and unimplemented. Camera orientation and astronomical time are
independent. The current constellation crop does not authorize changing geometry
within M2. M3 benchmarking follows M2, then M4 documentation before M5.

## Current part-1 implementation

Part 1 includes build/info and the configured viewer. Its existing catalogue
controls remain unchanged by the initial part-2 colour path below. Camera work
is assigned to M5; later acceptance evidence stays for its own discussion.

`msc view` now consumes an existing binary through the quantity-aware reader,
using complete TOML configuration, temporary CLI overrides and scoped spdlog
logging. The owner accepted removing the former positional source-building
launcher during this transition. At that stage scene mathematics and display
remained the M1 baseline; the current animated viewer is recorded below. The
[reader policy](../decisions/application-controls.md#catalogue-controls)
checks file structure and the selected prefix. Building must check its completed
temporary binary before publication; catalogue info reports path, stored star
count and bytes with decimal GB. Build capacity is exact; Hipparcos fills it
first, followed by only the necessary accepted Gaia records. The selected
records are stored in brightness order. Build/info and failure handling have
integration coverage. All 176 C++ tests pass in both warning-as-error and
ASan/UBSan builds; all 30 Python tests pass. Cppcheck, Lizard and formatting
checks pass. The broader
catalogue-understanding checkpoint stays open.

## Initial part-2 colour implementation — 2026-10-25

Both parsers now prepare temperature-based linear-sRGB colour in three uint8
channels, keep magnitude as double, and move source positions to J2016 during
building. Binary version 3 uses 68-byte records with mandatory RGB. The renderer
uploads compact RGB bytes directly, scales linear light, and performs one sRGB
output encoding. At that stage the prior white-star display curve, fixed
geometry, cutoff and point size remained; the animated viewer below supersedes
the static display. See the [colour model](../decisions/stellar-colour.md) and
[prepared catalogue contract](../decisions/catalogue-integration.md#current-prepared-catalogue--2026-10-25).

All 189 C++ tests pass in ordinary and ASan/UBSan/leak builds. An independent
software-OpenGL check compiled/linked the actual shaders and verified pixel
values and detection of double encoding; this does not validate the physical
display/window. A real 4096-record build retained 62 Hipparcos and 4034 Gaia stars,
exact source magnitude values, complete RGB and J2016 positions. No full archive
benchmark, new cross-match or owner visual acceptance is claimed.

This does not complete all colour/brightness controls or M2: appearance review,
remaining controls and glow/subtle animation still need their respective scope
and acceptance discussions. See [appearance/output boundaries](../decisions/ceiling-product-output.md),
[application controls](../decisions/application-controls.md) and
[updated record sizes](../decisions/memory-accounting.md).

## Animated viewer — promoted 2026-10-28

The native viewer began as an opt-in trial on 2026-10-26. On 2026-10-27 the owner
rejected slow pulsation and approved frame-averaged spectral variation with
correlated colour and the quieter profile. Extra tuning options and shortcuts
were removed at their request.

On 2026-10-28 the owner provisionally accepted the algorithm as good for now and
requested that plain `msc view` always show it. The experimental flag and static
renderer are removed. Existing coefficients, limiting magnitude 6, RGB8 and
fixed Toulouse/time geometry are preserved. Space freezes/resumes the picture,
Escape closes it, and resize remains available. No configuration migration or
catalogue rebuild is needed. Later that day the owner accepted the stronger
native comparison as the default: it is now the ×1 baseline, with a bounded
response after signal averaging and a 30 fps ceiling. Temporary comparison
controls and guides are removed.

This is provisional acceptance, not M2 closeout or a million-source performance
result. See the [viewer record](../decisions/stellar-appearance-trial.md) for
model details, verification and limits, and the [follow-up list](../todo.md#animation-validation-and-scaling)
for later measurement and review. Proposed zone/full/compact strategies remain
unimplemented.
