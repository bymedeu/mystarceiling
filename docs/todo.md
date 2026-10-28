# Project todo list

Owner decisions recorded on 2026-10-25 and updated on 2026-10-28. This list
records completed bounded work and planned follow-up. Open tasks do not select
implementation tooling.

## Documentation phase between M2 and M3

- [ ] Create coherent project documentation after M2 and before beginning M3.
  Consolidate the working setup/build instructions, CLI/configuration usage,
  catalogue preparation and data conventions, core/rendering boundaries, and
  accepted approximation limits. Use the implemented M2 behaviour as the baseline.
- [ ] Agree the documentation structure and publishing/tooling approach during
  that phase. No Doxygen, MkDocs, hosting or other framework is selected yet.

This is a dedicated documentation pass, without a separate assigned date or
changes to the existing milestone estimates. Focused decision records and basic
usage instructions continue to be maintained while M2 is implemented.

## Later accuracy work

- [ ] Consider the optional [colour-conversion refinements](decisions/ceiling-product-output.md#deferred-accuracy-work)
  after M2, if their visible benefit justifies the effort. No calibration campaign
  or richer-spectra acquisition is required to finish M2.
- [ ] Revisit the [motion model and supported date range](decisions/stellar-motion.md#accepted-m2-preparation-direction--2026-10-25)
  during M3 observer/time planning. More complete astrometry remains a scoped
  future choice, not an implied requirement to implement every physical effect.

The [accepted catalogue preparation decision](decisions/ceiling-product-output.md#accepted-catalogue-preparation--2026-10-25)
is the canonical record of the current approximations and remaining design details.

## Animation validation and scaling

- [x] Promote the [animated viewer](decisions/stellar-appearance-trial.md) to plain
  `msc view`, removing the experimental flag and static alternative. The owner
  provisionally accepted the algorithm as good for now on 2026-10-28.
- [x] Adopt the owner-approved stronger native comparison as the ×1 baseline,
  with a bounded visual response and 60 fps ceiling. Remove temporary comparison
  controls and guides; see the [current model](decisions/stellar-appearance-trial.md).
- [ ] With the provisional reference recorded, profile increasingly large animated
  workloads,
  including a million-source case where feasible. Measure frame time, memory,
  visible/source counts and covered pixels; separate signal evaluation from
  rasterization/blending costs. Current arithmetic and small-catalogue tests do
  not establish million-source throughput.
- [ ] Compare any selected optimization against the accepted reference, preserving
  deterministic phase, frame averaging, colour, combined faint light and edge
  contributions. No new culling, tier or caching strategy is selected here.

- [ ] Retain a repeatable offscreen shader/core comparison, including signed
  frame-averaging weights and freeze/resume; the existing GPU evidence came from
  a temporary harness and is not covered by the checked-in CPU tests.
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
