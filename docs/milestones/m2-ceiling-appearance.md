# M2 — Ceiling appearance and data controls

Planning note, 2026-10-18, MODE GREEN. **Estimate: 13 December 2026; scope to
discuss**, not implementation approval. The owner supplied this capacity-based
baseline with lower availability starting January–February 2027. See the
[roadmap](../../README.md#milestones). The
[owner-understanding checkpoint](../project-state.md#required-understanding-checkpoint)
remains open before substantial implementation.

## Proposed bounded scope

Build on the working first 4096-source M1 path and the recorded 119 passing C++
tests after the source-independent Star refactor. Aim for realistic-looking
colour, useful brightness controls, modest glow and subtle effects. “Animated”
does not mandate meteors, Perseids or clouds now. G and BP-RP are measurements;
derived display appearance needs documented approximations and validation.

Selection remains catalogue magnitude-first. An optional quantity cap is the
owner's proposal; exact behavior/order with a limiting magnitude remains open.
It bounds workload size, without solving scaling or establishing that a million
sources have been processed efficiently. Nested 4096, 65536, 262144 and 1048576
sizes are measurement cases, not a requirement to show every star. M4 must still
measure the actual million-source CPU pipeline and bounded preparation.

Basic useful inspection/projection may be included where agreed. Click-to-inspect
and screenshot export are optional; camera exploration is secondary and its
extent remains open. Camera and astronomical time are independent; M3 supplies
validated observer/time behavior. Reuse appropriate coordinate math only where
justified. The M1 pattern mismatch is low priority per owner; no new comparison
or accuracy claim was made.

## Discussion before implementation

Agree the smallest appearance/control slice, acceptance evidence, camera extent,
and measurement cases after the owner explains the current catalogue pipeline.
No RGBE, packed/batch layout, filter model, API, GUI, protocol, canonical Star or
binary-schema change is selected. See [appearance/output boundaries](../decisions/ceiling-product-output.md),
[application controls](../decisions/application-controls.md) and
[record-size accounting](../decisions/memory-accounting.md).
