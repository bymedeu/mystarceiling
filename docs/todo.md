# Project todo list

Owner decisions recorded on 2026-10-25. These tasks describe planned follow-up;
they do not claim completed work or select implementation tooling.

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
