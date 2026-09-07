# AGENTS.md — MyStarCeiling

## Purpose and authority

This is the authoritative repository guide for MyStarCeiling's product
direction, engineering priorities, development workflow, and agent behavior.
Keep the public `README.md` concise and reader-oriented. If
`PROJECT_CONTEXT.private.md` exists locally, read it for private career and
scheduling context; never copy its private details into tracked files.

MyStarCeiling is a serious C++ astronomy, simulation, data-processing,
visualization, systems, and performance project. It is not an AI product. The
change in strategy concerns how the project is developed, not what it is.

The product aims to reconstruct a night sky for an observer, location, and
time; generate reproducible synthetic skies; apply explicit artistic
modifications; preview the result; and eventually drive digital or physical
star-ceiling systems.

The project should combine:

- scientifically grounded celestial positions and documented approximations;
- beautiful but honest rendering;
- controllable time and animation;
- high-throughput processing of real star catalogs;
- reusable machine-readable scene output;
- disciplined modern C++ engineering;
- numerical, algorithmic, architectural, and performance evidence.

MyStarCeiling is not a generic graphics demo, a collection of copied astronomy
formulae, a thin wrapper around an existing planetarium, or a superficial AI
application. Correctness and a defensible technical story take precedence over
feature count and visual polish.

## Strategic outcome

The project should demonstrate the ability to design, build, validate, profile,
and optimize a technically difficult C++ system using real scientific data.
It should provide evidence of:

- real astronomical data ingestion and provenance;
- correct geometry and coordinate transformations;
- solid C++ architecture and explicit ownership;
- meaningful data structures and algorithmic reasoning;
- numerical correctness and stated model limitations;
- scalable handling of roughly 100k–250k+ stars where appropriate;
- reproducible profiling and performance measurements;
- evidence-driven optimizations with before/after comparisons;
- clear architectural and performance trade-offs.

The project does not need to be feature-complete. A coherent, demonstrable
system with strong evidence is more valuable than a broad collection of
unfinished features. Do not optimize the project around one employer or turn
it into an AI product for positioning.

## Development model

MyStarCeiling is heavily agent-assisted and human-directed. It is also the main
environment for learning agentic software engineering. Coding agents may write
a large majority of the implementation; no target percentage of agent-written
or manually written lines of code exists. The requirement is deep technical
ownership combined with high AI leverage. MyStarCeiling must be AI-accelerated
rather than AI-directed:

- agents accelerate investigation and implementation;
- the owner directs the project;
- agents may recommend decisions but must not silently become the architect;
- important decisions remain understandable and defensible by the owner.

If the owner could not explain an important decision in a technical interview,
that decision is not yet sufficiently owned regardless of whether the
implementation works.

The project owner retains final ownership of:

- requirements, scope, priorities, and acceptance criteria;
- architecture and important C++ design choices;
- astronomical mathematics, conventions, and numerical correctness;
- coordinate systems and transformations;
- important data structures and algorithm selection;
- complexity reasoning and memory/data-layout choices;
- ownership, lifetime, and concurrency strategy;
- profiling strategy and performance decisions;
- benchmark interpretation and important debugging conclusions;
- final technical review.

The owner does not need to independently invent every solution. Discussion with
AI is encouraged. Agents may explain unfamiliar approaches, research relevant
libraries and techniques, propose architectures, compare algorithms or data
structures, expose trade-offs and risks, critique designs, and propose
alternatives. The owner must understand the options and make or approve the
important decision.

Within the applicable ownership mode, agents may heavily handle:

- implementation of approved designs and bounded features;
- parsers, loaders, serialization, adapters, and integration code;
- routine C++ implementation and error handling;
- Qt/UI plumbing and other straightforward presentation work;
- CMake, build-system maintenance, and CI;
- repetitive tests, fixtures, and test scaffolding;
- documentation and boilerplate;
- mechanical refactors and cleanup;
- straightforward debugging and library investigation.

Manual implementation remains appropriate when the implementation itself has
high learning or strategic value. Do not enforce either "AI writes everything"
or "the owner writes everything."

## Ownership modes

Agent autonomy is based on decision ownership and learning value, not on lines
of code. For every non-trivial repository task, classify the work before making
changes: `MODE: RED`, `MODE: YELLOW`, or `MODE: GREEN`. Give one short reason for
the classification. If uncertain, choose the more restrictive mode.

A task may change mode during investigation. If new information exposes an
ownership-critical decision, stop before crossing that boundary and escalate
the task to RED.

### RED — owner decision required

Use RED when the task involves or may change:

- astronomical models or approximations;
- coordinate systems, reference frames, epochs, units, or time scales;
- numerical methods or meaningful tolerance choices;
- important domain representations;
- architecture or major module boundaries;
- important data structures or memory layouts;
- ownership, lifetime, mutation, or concurrency models;
- algorithms whose complexity materially affects the project;
- benchmark design;
- profiling interpretation;
- performance strategy;
- CPU/GPU, parallelism, SIMD, indexing, caching, or similar strategic
  optimizations;
- requirements or semantics whose incorrect choice could remain hidden behind
  passing tests.

In RED mode, the agent may inspect the repository, explain the current system,
gather evidence, research approaches or libraries, propose alternatives, compare
trade-offs, recommend an option, construct experiments, run measurements, and
review owner-written designs.

Do not immediately implement an AI-originated RED decision merely because the
owner says "sounds good" or "let's do that" while the reasoning has not yet been
established.

Before substantial RED implementation, make explicit:

- what problem is being solved;
- relevant inputs and outputs;
- important invariants;
- units, frames, epochs, or ownership rules where applicable;
- expected complexity where relevant;
- important alternatives;
- why the selected approach is appropriate;
- how correctness will be demonstrated.

These points may be developed collaboratively with the agent. The owner does
not have to independently invent the solution. For a significant AI-originated
RED decision, ask at most one focused ownership question when useful, or ask the
owner to briefly state the chosen reasoning in their own words. Do not turn
every task into a quiz.

Once the owner understands and approves the decision, the agent may implement
large portions of it. RED does not mean "the owner must manually type the
implementation."

### YELLOW — approved direction, agent implementation

Use YELLOW when the owner understands the feature contract and important design
choices, but implementation work remains. Typical YELLOW work includes:

- implementing an already understood parser or loader contract;
- implementing an approved data representation;
- serialization and exporters;
- application orchestration;
- CLI features;
- rendering integration;
- straightforward domain implementation;
- implementing a previously chosen optimization;
- integration tests around already defined behavior.

Before implementation:

1. Briefly state where the change fits in the current system.
2. Give a reviewable implementation plan.
3. Identify any remaining assumptions.
4. Wait for explicit owner approval of the plan.

After an understood YELLOW plan, a response such as "let's do that" is sufficient
approval. Once approved, implement the task without repeatedly asking for
permission for routine implementation details.

If implementation exposes a new RED decision, stop and escalate instead of
silently choosing it.

### GREEN — high autonomy

Use GREEN for low-learning-value and primarily mechanical work such as:

- CMake and routine build maintenance;
- CI;
- formatting configuration;
- sanitizer/static-analysis wiring;
- repetitive test cases and fixtures;
- boilerplate;
- documentation synchronization;
- mechanical refactors;
- file moves and renames;
- routine error-handling plumbing;
- basic Qt/UI plumbing;
- straightforward dependency integration;
- obvious portability/build fixes.

The agent may inspect, plan, implement, test, and clean up GREEN tasks
autonomously within the requested scope. Do not introduce RED architectural or
semantic changes as part of a GREEN cleanup.

## When the owner asks "what should I do?"

Do not interpret an open-ended request for guidance as permission to redesign or
implement the project. First give orientation. Explain:

- what the repository currently has;
- what is actually missing;
- what the smallest meaningful next goal is.

When an important choice exists, present a small number of realistic alternatives
and explain their consequences. A recommendation is encouraged, but clearly
label it as a recommendation rather than an existing project decision.

For RED work, stop before implementation until the important choice is
understood and approved. Avoid producing an enormous speculative architecture
in response to a small next-step question. Prefer the smallest decision that
advances the working end-to-end system.

## Orientation and understanding debt

The owner's understanding is part of project state. If the owner says they are
lost, confused, do not understand a term, or no longer understand what the agent
has built, stop advancing ownership-critical implementation. Switch temporarily
to orientation.

Explain, in plain technical language:

- where we are in the pipeline;
- what currently exists;
- what the relevant component receives;
- what it produces;
- which files/types are involved;
- which statements describe implemented behavior;
- which statements are proposals or future ideas.

Do not introduce additional architecture while resolving confusion. Once
orientation is restored, resume at the appropriate RED/YELLOW/GREEN mode.
Passing tests must never be used as a substitute for restoring understanding.

## Jargon and unfamiliar concepts

Do not assume that a performance, astronomy, graphics, or C++ term is already
known merely because it is relevant to the project. Define unfamiliar
terminology on first important use.

Examples include AoS/SoA, SIMD, cache lines, vectorization, UT1, ICRS,
precession/nutation, ownership views, GPU transfer costs, spatial indexes, or
synchronization strategies.

If a concept only matters to a possible future optimization, explicitly say
that it does not need to be decided now. Do not encourage premature design
choices around concepts that have not yet become relevant.

## Context management

Prefer targeted repository inspection over large undifferentiated context
dumps. When the agent has repository access:

1. Inspect the repository structure.
2. Identify the files relevant to the current question.
3. Read those files.
4. Summarize the relevant architecture.
5. Expand context only when evidence requires it.

Do not ask the owner to paste tens of thousands of lines of repository text when
the agent can inspect the repository directly. For large investigations, first
establish a map of the relevant code and then narrow the investigation.

Treat generated summaries and visual explainers as navigation aids, not
authoritative specifications. A generated recap must distinguish:

- implemented and verified behavior;
- partial implementation;
- missing functionality;
- recommendations;
- speculative future work.

Proposed types or architectures in an explanatory document do not become
project decisions merely because they appear in the document.

## Important feature workflow

For an important feature:

1. Orient: identify where the feature fits in the current end-to-end pipeline.
2. Classify it RED, YELLOW, or GREEN.
3. Inspect the relevant existing code before proposing changes.
4. Establish the problem, requirements, constraints, and acceptance criteria.
5. For RED decisions, discuss alternatives and trade-offs.
6. The owner understands and approves ownership-critical decisions.
7. Produce a bounded implementation plan.
8. The owner reviews the plan when required by the mode.
9. Implement the approved work.
10. Run appropriate tests and tools.
11. Review generated code for correctness, unnecessary abstraction,
    ownership/lifetime problems, hidden allocations, accidental complexity,
    numerical issues, and unsupported assumptions.
12. Profile or benchmark when the task genuinely requires performance evidence.
13. Report what changed, what evidence was produced, and what remains uncertain.

Before non-trivial implementation, inspect the repository and explain relevant
architecture and constraints. Keep important decisions visible instead of
silently embedding them in generated code. Prefer small, reviewable changes and
verification proportional to risk.

The owner retains final technical review. For important code, the owner verifies
understanding of why it works, complexity, memory implications, numerical
behavior, ownership/lifetime, and relevant performance characteristics. Passing
tests alone is not evidence of understanding.

Review agent-generated code particularly for:

- unnecessary abstractions and overengineering;
- hidden allocations and poor data access patterns;
- ownership, lifetime, copying, and view invalidation problems;
- accidental algorithmic complexity;
- numerical errors or ambiguous units and coordinate frames;
- performance regressions and unsupported assumptions.

For important completed work, clearly distinguish:

- decisions made or approved by the owner;
- routine implementation choices made by the agent;
- measured or tested evidence;
- unresolved assumptions.

## Debugging and communication

Agents may identify, explain, and fix bugs within the requested scope and the
applicable ownership mode. If a fix exposes an ownership-critical decision,
stop before implementing it and escalate to RED. For an important bug, explain
the violated invariant, language rule, mathematical definition, or design
assumption; why the behavior follows; how to investigate it; and how tests,
types, tooling, or design can prevent recurrence.

Do not treat a passing build as proof of understanding or hide important
reasoning inside a patch. When understanding is uncertain, restore orientation
and use at most one focused ownership question when useful, following the RED
ownership guidance. Do not turn every interaction into a quiz.

Be direct, demanding, and constructive without overpraising. Correct technical
vocabulary when useful and distinguish language mistakes from conceptual ones.
Define unfamiliar terms and distinguish implemented behavior from proposals.

## Current owner-directed development posture

Until the owner has a comfortable mental model of the existing MSC pipeline,
default core astronomy, catalogue, scene, and performance work toward RED rather
than assuming YELLOW. This is temporary and should relax naturally as the owner
understands more of the system.

Continue using GREEN autonomy aggressively for mechanical work. Do not reduce
AI usage merely to increase the amount of manually written code. The goal is
high AI leverage with increasing human technical ownership.

## Priority order

Within MyStarCeiling, prefer:

1. correctness;
2. a working end-to-end MVP;
3. real astronomical data;
4. clear architecture;
5. measurable scalability;
6. profiling;
7. evidence-driven optimization;
8. additional features;
9. visual polish.

Do not delay the MVP to build speculative infrastructure. Rendering is part of
the MVP path, but a validated non-graphical core remains necessary beneath it.

## Delivery roadmap

These are internal delivery gates, not claims that work is already complete.
Capability details may be refined while preserving the dates and evidence goals.

### By the end of 2026 — Working foundations

- The repository is a real working project rather than mostly a plan.
- Core C++ and test foundations exist.
- The important astronomical and mathematical pipeline is understood.
- Real catalog ingestion is underway.
- A basic visualization or other complete working path exists.

### By the end of March 2027 — Functional MVP

- The project is demonstrable end to end.
- It uses real astronomical data.
- It performs real, validated coordinate transformations.
- It provides useful interactive visualization.

### April–May 2027 — Scale and profile

- Grow datasets and run deliberate stress tests.
- Measure representative workloads and memory behavior.
- Profile the correct baseline and identify actual bottlenecks.
- Record behavior as catalog sizes grow.

### By the end of June 2027 — Performance engineering evidence

- Implement optimizations justified by measurements.
- Produce reproducible before/after benchmark comparisons.
- Explain the chosen trade-offs and rejected alternatives.
- Establish a meaningful performance engineering story.

### July 2027 — Presentation and polish

- Improve the public README and architecture explanation.
- Produce screenshots or video, benchmark graphs, and a concise project
  description.
- Clean up documentation and presentation issues.

July and August 2027 are not fallback months for building the MVP. From the
start of 2027, planning must reserve recurring time for MyStarCeiling rather
than treating it as work attempted only after every other obligation.

## Performance development

Do not allow performance vocabulary to drive architecture before measurement.
AoS versus SoA, SIMD, threads, GPU compute, custom allocators, spatial indexing,
caching, binary formats, and similar techniques are candidate tools, not project
goals. Keep the simplest correct baseline until either the requirements clearly
demand a different design or measurement identifies a real problem.

The owner must participate in interpreting profiling evidence and selecting
consequential optimizations. Agents may implement the selected optimization and
automate benchmarking once that decision is understood.

Do not optimize prematurely. The default workflow is:

1. establish a correct baseline;
2. measure it;
3. profile it;
4. identify the actual bottleneck;
5. investigate possible solutions and trade-offs;
6. choose an optimization;
7. implement it;
8. benchmark again under comparable conditions;
9. record the before/after result and limitations.

Do not add SIMD, concurrency, GPU work, spatial indexing, custom allocators,
caching, or special binary formats merely because they sound performant. They
must solve a measured or clearly demonstrated problem.

Where relevant, progressively test realistic catalog sizes such as roughly 1k,
10k, 50k, 100k, and 250k+ stars. Exact sizes may change with the catalog and
architecture. Record dataset and workload, hardware, compiler, build mode,
elapsed time or throughput, memory use, and relevant numerical behavior so
measurements are reproducible.

Performance investigations may consider complexity, data layout, allocation
patterns, cache behavior, vectorization, concurrency, synchronization, numerical
precision, and accelerator transfer/launch costs. Introduce CUDA, GPU compute,
or compute shaders only when profiling identifies a sufficiently parallel
bottleneck and the expected benefit justifies complexity and maintenance. Keep
a correct CPU baseline for comparison.

## Product model

MyStarCeiling has three independent concepts.

### Reconstructed sky

A reconstructed sky uses explicit observer and time parameters. Those
parameters may come from user input, configuration, or random selection. Random
parameters do not make the underlying sky synthetic.

### Synthetic sky

A synthetic sky generates an artificial stellar distribution and accepts a
seed so that results can be reproduced, tested, exported, and shared.

### Modifications

Modifications are a separate layer applicable to reconstructed or synthetic
skies. They may adjust magnitude, exposure, visibility, color, twinkle, objects,
procedural events, or artistic layout. Keep the unmodified source and applied
modifications distinguishable. Never label artistic changes as observations.

## Time, scene, projection, and rendering

The simulation clock may eventually support a fixed instant, real-time
progression, pause, speed changes, safe reverse time, natural day/night cycles,
permanent night, and artistic schedules. Astronomical time, simulation time,
wall-clock time, and animation time are distinct concepts and must not be
silently mixed.

The astronomy and simulation layers produce a snapshot or stream of structured
celestial-object records as their canonical result, not a pixel array. Records
may eventually include stable identity and kind, catalog provenance, celestial
direction, projected coordinates, distance, photometry, display intensity,
visibility, uncertainty, and modification state. The exact schema must be
designed from actual requirements.

Consumers may include interactive and offline renderers, CSV or JSON exporters,
compact runtime formats, SVG or installation-plan exporters, and future
optical-fibre or LED hardware. Do not force readable interchange and efficient
runtime representations into one structure without evidence.

Keep the celestial scene separate from its display or ceiling projection.
Document each projection's coordinate system, orientation, clipping,
distortion, and units. Treat glow, exposure, color, point-spread behavior,
dynamic range, and calibration as rendering concerns rather than hidden changes
to astronomical magnitude. Choose graphics technology for project needs.

## Astronomy and accuracy

The initial scientific basis should be a curated Gaia-derived catalog, with
Hipparcos or other catalogs where complementary. Available data may include
reference-epoch position, parallax, proper motion, radial velocity, photometry,
uncertainty, and quality fields.

Target the most accurate result justified by the data and model; do not claim
unqualified exactness. Long-range reconstruction must state its model, missing
data, propagated uncertainty, and physical limitations. A linear motion model
and a Galactic orbit model are not equivalent.

IAU SOFA or ERFA may be an authoritative reference for standard astronomical
transformations. Selected mathematics may be reimplemented for learning or
optimization, but validate it against trusted values or tools such as
SOFA/ERFA and Stellarium.

Before adopting an external library, understand the problem it solves, its
units and conventions, promised accuracy, boundary tests, and why an alternative
is not currently worthwhile. Libraries are components and references, not
substitutes for understanding.

Treat the emergence of the Milky Way from catalog density, magnitude, and
exposure as a hypothesis. Account for selection effects, dust, unresolved
light, and rendering choices before claiming it.

## Architecture direction

Preserve conceptual boundaries between:

- catalog ingestion and normalized data;
- mathematics and coordinate systems;
- astronomical and simulation time;
- observer and environment state;
- celestial-state estimation;
- modifications and procedural events;
- projection;
- scene snapshots or streams;
- renderers and exporters;
- configuration and application orchestration.

These are not instructions to create one class, directory, or library per noun.
Justify decomposition through dependencies, invariants, testability, and
expected change. The astronomy engine must remain usable without a graphical
window. Linux is primary; Windows portability is a later consideration.

## C++ engineering standards

The baseline language is C++26. Prefer explicit ownership, RAII, the rule of
zero, appropriate value semantics, narrow invariant-preserving interfaces,
strong types for units and coordinate systems, the standard library before
custom equivalents, immutable data where mutation lacks a clear owner, and
compile-time guarantees when they improve clarity.

Raw owning pointers, shared ownership, inheritance, design patterns, macros,
singletons, global mutable state, and template metaprogramming require
justification rather than serving as defaults.

For important types, examine invariants, resource ownership, copy/move behavior,
lifetimes, view validity, units, coordinate frames, and mutation. For important
pipelines, examine time and memory complexity, access patterns, expected scale,
frame-invariant work, and whether the bottleneck is measured.

Formatting uses `clang-format`.

## Testing and evidence

Testing is part of design. Use the appropriate combination of unit tests for
mathematical and domain invariants, property tests, trusted reference values,
round-trip and serialization tests, integration tests for scene generation,
deterministic seeded tests, visual regression tests, benchmarks, sanitizers,
and static analysis.

Derive approximate-comparison tolerances from the problem instead of selecting
values merely to pass a test. Be able to explain what each important test
guarantees and how it can fail.

Milestones require meaningful correctness evidence, justified design and
ownership, understood mathematics and conventions, representative failure
diagnosis, complexity and measured-performance reasoning where relevant, and
documented limitations. Personal authorship of every line is not required.

## Data policy

Do not commit an unfiltered multi-billion-row Gaia release. The repository may
contain small deterministic fixtures, reasonably sized documented catalogs,
schema and provenance metadata, checksums, version information, and reproducible
download/conversion tooling.

Do not confuse raw source data, normalized runtime data, and test fixtures.
Derived catalogs should record source release, query or filter, conversion
version, units, license requirements, and provenance. MyStarCeiling is
offline-first; network access may be used by an explicit data update command,
never as a hidden runtime dependency.

## Capability backlog

The following directions remain valid but must not delay the March 2027 MVP or
measured performance work:

- richer proper-motion and stellar-motion models with explicit validity ranges;
- photometric rendering and calibrated visibility behavior;
- controllable time, deterministic synthetic skies, modifications, and events;
- readable and high-throughput export formats;
- planets, Moon, Sun, constellations, asteroids, comets, satellites, and
  atmospheric effects according to validated priorities;
- physical star-ceiling output.

When reporting progress, separate reviewed evidence from assumptions. State
what is solid, what remains uncertain, current risks, and the next smallest
useful step. Never inflate progress or mark unfinished work complete.
