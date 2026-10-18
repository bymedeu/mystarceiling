# MSC documentation

This directory records project objectives, decisions, and supporting evidence.
It is plain Markdown for now; Doxygen and MkDocs integration are deferred.

## Start here

- [Current project state](project-state.md): concise cross-session hand-off,
  verified progress, approved work, and open decisions.
- [Star ceiling and output boundaries](decisions/ceiling-product-output.md):
  current owner priorities, colour/cache proposal, camera/time distinction,
  device flexibility and unselected implementation choices.
- [Catalogue integration](decisions/catalogue-integration.md): approved
  Gaia/Hipparcos boundary, Hp/G choices, and Stellarium-derived lessons.
- [Gaia datasets](gaia-datasets.md): current acquisition scope, field review,
  benchmark counts, and storage constraints.
- [Hipparcos-2 supplement](hipparcos2-datasets.md): separate provisional Hp-selected
  catalogue, reproducible acquisition, and official Gaia cross-match evidence.
- [Stellar-motion propagation](decisions/stellar-motion.md): selected vector
  model, core boundary, validation, and accuracy limits.
- [Application controls](decisions/application-controls.md): settled CLI/GUI
  behavior, open place resolution, catalogue controls, and milestone boundaries.
- [Development discussion history](development-history/README.md): concise
  owner decisions, reasons, alternatives, progress, and AI contributions.
- [Objectives and decisions](objectives-and-decisions.md): current direction,
  project-wide direction and dated historical record.
- [Milestone records](milestones/README.md): focused decisions, acceptance
  evidence, and remaining questions for each active or completed milestone.
- [Milestones in the public README](../README.md#milestones): dated objectives,
  introduced capabilities, engineering problems, and portfolio gates.
- Portfolio-level `AGENTS.md`: ownership, approval, documentation, and delivery
  rules. The project keeps durable technical decisions here so a standalone
  checkout remains understandable.
- [Public overview](../README.md): concise introduction to the project.

## Keeping records useful

Update the current decision summary when the owner resolves or revises a choice,
and append a dated history entry explaining the change. Clearly distinguish
accepted decisions, assistant recommendations, open questions, and verified
implementation. Record evidence and limitations alongside measured results.

Put milestone-specific decisions in that milestone's record instead of extending
the historical project-wide file indefinitely. Create focused design notes,
benchmark reports, or learning notes only when there is substantive material.
Future notes should link back here and avoid duplicating the project contract.
Keep private career and scheduling context in the existing ignored private file.
