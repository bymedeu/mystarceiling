# Project todo list

Owner decisions recorded on 2026-10-25 and updated on 2026-10-28. This list
records completed bounded work and planned follow-up. Open tasks do not select
implementation tooling.

## M4 Project documentation

- [ ] Complete M4 project documentation after M3 benchmarking and before M5
  observer/time/camera work.
  Consolidate the working setup/build instructions, CLI/configuration usage,
  catalogue preparation and data conventions, core/rendering boundaries, and
  accepted approximation limits. Use the implemented M2 behaviour as the baseline.
- [ ] Agree the documentation structure and publishing/tooling approach during
  that phase. No Doxygen, MkDocs, hosting or other framework is selected yet.

This is M4, a dedicated documentation pass without a separately assigned date.
The revised scope has not been used to recalculate the existing dated estimates.
Focused decision records and basic
usage instructions continue to be maintained while M2 is implemented.

## Later accuracy work

- [ ] Consider the optional [colour-conversion refinements](decisions/ceiling-product-output.md#deferred-accuracy-work)
  after M2, if their visible benefit justifies the effort. No calibration campaign
  or richer-spectra acquisition is required to finish M2.
- [ ] Revisit the [motion model and supported date range](decisions/stellar-motion.md#accepted-m2-preparation-direction--2026-10-25)
  during M5 observer/time planning. More complete astrometry remains a scoped
  future choice, not an implied requirement to implement every physical effect.

The [accepted catalogue preparation decision](decisions/ceiling-product-output.md#accepted-catalogue-preparation--2026-10-25)
is the canonical record of the current approximations and remaining design details.

## Animation validation and scaling

- [x] Promote the [animated viewer](decisions/stellar-appearance-trial.md) to plain
  `msc view`, removing the experimental flag and static alternative. The owner
  provisionally accepted the algorithm as good for now on 2026-10-28.
- [x] Adopt the owner-approved stronger native comparison as the ×1 baseline,
  with a bounded visual response and 30 fps ceiling. Remove temporary comparison
  controls and guides; see the [current model](decisions/stellar-appearance-trial.md).
- [ ] Build the accepted separate developer executable in [M3](milestones/m3-benchmarking.md),
  with operation, pipeline, memory and rendering measurements, scaling curves and
  reproducible comparisons. Measure loaded/submitted/visible counts separately.
- [ ] Use and extend that suite in M6 for high usage and measured optimization.
  Workload size is not fixed at 1M; former M7 (superseded numbering) is folded
  into current M6. Preserve bounded memory and the accepted appearance when
  increasing workload.
- [ ] Compare any selected optimization against the accepted reference, preserving
  deterministic phase, frame averaging, colour, combined faint light and edge
  contributions. No new culling, tier or caching strategy is selected here.

- [x] Retain the offscreen shader/core and display regression checks for M2,
  including signed frame-averaging weights, changing frames and freeze/resume.
  Run the opt-in [headless renderer checks](../CONTRIBUTING.md#headless-renderer-regression-checks);
  unsupported context providers are reported as skips, not validation passes.
- [ ] Resolve the optional headless GLFW/Mesa leak-check limitation before
  claiming leak-clean OpenGL validation on that provider stack; retain the
  [standalone reproduction evidence](decisions/stellar-appearance-trial.md#retained-regression-checks--2026-10-30).
- [ ] Revisit extreme-uptime phase/midpoint precision before claiming long-time
  averaging accuracy. The independent review found midpoint rounding at 1e12
  animation seconds; that is separate from astronomical epoch/time handling.

## Later customization

- [ ] Discuss the owner's suggested JSON-based appearance/behaviour customization
  after the current visual behaviour is settled. This is a possible later option;
  no schema, parser, persistence mechanism or implementation is selected. Do not
  add appearance-tuning keys to the current configuration as part of the viewer
  promotion.

## Bright-source coverage

- [x] Repair the observed-minimum-G supplementation gap under the owner's bounded
  2026-10-28 authorization: inclusive estimated-G<=6 candidates, published-match
  evidence, fallbacks resolved against actual selected output, fresh retained raw
  source and verified runtime rebuild. See the
  [policy and measured result](hipparcos2-datasets.md#verified-result-run-20261028t194241299001z).
- [ ] Revisit completeness/component identity and photometric disagreements only
  within a separately agreed accuracy scope. Current fit limits, probabilistic
  matches and normal capacity truncation remain explicit limitations. This repair
  does not close the catalogue-understanding checkpoint or M2.

## Camera and perceived colour

- [ ] Implement camera movement and zoom during M5, with declared projection and
  orientation; keep camera motion independent of astronomical time.
- [ ] Discuss the proposed [perceived-colour reduction](decisions/ceiling-product-output.md#proposed-perceived-colour)
  toward grey as visibility decreases. Preserve original catalogue RGB. No
  visibility algorithm, threshold or implementation is selected or approved.

## Optional extensions

M8–M13 remain conditional in the [canonical roadmap](../README.md#milestones).
M8 covers a justified CUDA comparison; M9 covers planets, satellites and ordinary
meteors/shooting stars. M10 covers deterministic synthetic JSON and a separate
behavior/settings JSON contract, M11 long exposure, M12 strongly affecting
illumination, and M13 clouds. JSON schemas, data providers, illumination models
and optional scheduling remain to discuss; these are not delivery promises.
