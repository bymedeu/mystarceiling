# M2 — Ceiling appearance and data controls

Planning note, 2026-10-19, MODE GREEN. **Estimate: 13 December 2026; scope to
discuss**, not implementation approval. The primary assistant proposed this
date from the owner’s capacity constraints, lower availability starting
January–February 2027 and September release goal; the owner did not choose
the intermediate milestone dates. See the
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
does not mandate meteors, Perseids or clouds now. G and BP-RP are measurements;
derived display appearance needs documented approximations and validation.

Building follows the Hipparcos-first membership policy and stores its selection
in brightness order; quantity caps loaded records before visibility and display
filtering, as specified in application controls. It bounds
workload size, without solving scaling or establishing that a million sources
have been processed efficiently. Nested 4096, 65536, 262144 and 1048576
sizes are measurement cases, not a requirement to show every star. M4 must still
measure the actual million-source CPU pipeline and bounded preparation.

Basic useful inspection/projection may be included where agreed. Click-to-inspect
and screenshot export are optional; camera exploration is secondary and its
extent remains open. Camera and astronomical time are independent; M3 supplies
validated observer/time behavior. Reuse appropriate coordinate math only where
justified. The M1 pattern mismatch is low priority per owner; no new comparison
or accuracy claim was made.

## Current part-1 implementation

Part 1 now includes build/info and the configured viewer. Binary schema,
photometry, astronomy and display retain the existing baseline. The next bounded
part is colour/brightness; its model and acceptance evidence remain to discuss.
Camera extent and later acceptance evidence stay for their respective discussions.

`msc view` now consumes an existing binary through the quantity-aware reader,
using complete TOML configuration, temporary CLI overrides and scoped spdlog
logging. The owner accepted removing the former positional source-building
launcher during this transition. Scene mathematics and display remain the M1
baseline. The [reader policy](../decisions/application-controls.md#catalogue-controls)
checks file structure and the selected prefix. Building must check its completed
temporary binary before publication; catalogue info reports path, stored star
count and bytes with decimal GB. Build capacity is exact; Hipparcos fills it
first, followed by only the necessary accepted Gaia records. The selected
records are stored in brightness order. Build/info and failure handling have
integration coverage. All 175 C++ tests pass in both warning-as-error and
ASan/UBSan builds; all 30 Python tests pass. Cppcheck, Lizard and formatting
checks pass. The broader
catalogue-understanding checkpoint stays open.

No RGBE, packed/batch layout, filter model, API, GUI, protocol, canonical Star or
binary-schema change is selected. See [appearance/output boundaries](../decisions/ceiling-product-output.md),
[application controls](../decisions/application-controls.md) and
[record-size accounting](../decisions/memory-accounting.md).
