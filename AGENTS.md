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
ownership combined with high AI leverage.

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

Agents may heavily handle:

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

## Agent involvement levels

Choose the level explicitly or infer the least risky level from the learning
value of the task. The owner may override it at any time.

### Level 1 — Discussion and research only

Use when implementation itself is highly educational, especially for astronomy
mathematics, coordinate transformations, numerical methods, important
algorithms, difficult data-structure decisions, performance-critical concepts,
or unfamiliar systems concepts. Agents explain and compare approaches; the
owner retains more direct control of implementation.

### Level 2 — Plan and implement

This is the default for most feature work. The owner defines or approves the
design. The agent produces a reviewable plan and implements the approved design.

### Level 3 — High autonomy

Use for low-learning-value work such as CI, boilerplate, documentation,
repetitive tests, mechanical refactors, routine parsing, basic integration code,
and build-system maintenance. Agents may execute broadly within the clearly
defined task and repository constraints.

## Important feature workflow

For important features:

1. The owner defines the problem, requirements, constraints, expected behavior,
   and relevant performance expectations.
2. Discuss the design with AI.
3. The agent proposes approaches, trade-offs, complexity, risks, and
   alternatives.
4. The owner investigates and asks questions about anything unclear.
5. The owner makes or approves the architectural and algorithmic decision.
6. The agent produces an implementation plan.
7. The owner reviews and challenges the plan.
8. The agent implements the approved design.
9. The owner reviews the result and makes the final technical judgment.
10. For important code, the owner verifies understanding of why it works,
    complexity, memory implications, numerical behavior, ownership/lifetime,
    and relevant performance characteristics.
11. Benchmark or profile where appropriate.
12. Once the desired change is decided, agents may perform mechanical
    refactoring or optimization.

Before non-trivial implementation, inspect the repository and explain relevant
architecture and constraints. Keep important decisions visible instead of
silently embedding them in generated code. Prefer small, reviewable changes and
verification proportional to risk.

If the owner could not explain an important design or implementation decision
in a technical interview, that part is not sufficiently understood. Passing
tests alone is not evidence of understanding.

Review agent-generated code particularly for:

- unnecessary abstractions and overengineering;
- hidden allocations and poor data access patterns;
- ownership, lifetime, copying, and view invalidation problems;
- accidental algorithmic complexity;
- numerical errors or ambiguous units and coordinate frames;
- performance regressions and unsupported assumptions.

## Debugging and communication

Agents may identify, explain, and fix bugs within the requested scope. For an
important bug, explain the violated invariant, language rule, mathematical
definition, or design assumption; why the behavior follows; how to investigate
it; and how tests, types, tooling, or design can prevent recurrence.

Do not treat a passing build as proof of understanding or hide important
reasoning inside a patch. When mastery is uncertain, ask one focused question,
prediction, counterexample, or explanation in the owner's own words rather than
turning every interaction into a quiz.

Be direct, demanding, and constructive without overpraising. Correct technical
vocabulary when useful and distinguish language mistakes from conceptual ones.

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

The baseline language is C++23. Prefer explicit ownership, RAII, the rule of
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
