# AGENTS.md — MyStarCeiling

## Project Identity

MyStarCeiling is a long-term C++ astronomy, simulation, data-processing, and
visualization project.

Its purpose is to reconstruct a night sky from an observer, a location, and a
time; generate reproducible synthetic skies; apply deliberate artistic
modifications; preview the result; and eventually drive digital or physical
star-ceiling systems.

The project should combine:

- scientifically grounded celestial positions;
- explicit and documented approximations;
- beautiful, realistic rendering;
- controllable time and animation;
- high-throughput processing of large star catalogs;
- reusable machine-readable scene output;
- disciplined modern C++ engineering.

Visual beauty is slightly more important to the final user experience, but it
should be reached through sound astronomical and physical models wherever
possible. Scientific correctness must not be claimed beyond the limits of the
data or model.

MyStarCeiling is not a generic graphics demo, a collection of copied astronomy
formulae, or a thin wrapper around an existing planetarium.

## Student Context

The student is a French EPITA computer-science student and is currently using
C++ professionally during an ESA internship.

Current strengths and interests include:

- C and pointers;
- growing familiarity with modern C++;
- templates, concepts, and generic programming;
- some STL, CMake, and GoogleTest experience;
- strong interest in performance and data processing.

Important areas for deliberate improvement include:

- ownership and lifetime design;
- RAII and the rule of zero;
- copy and move semantics;
- references, values, and views;
- class invariants and interface design;
- software architecture;
- linear algebra;
- testing strategy;
- profiling and data-oriented optimization;
- graphics programming, when the project reaches visualization.

The project should be strict about engineering quality. Finishing features is
less important than being able to justify the design and understand the code.

## Mentoring Contract

The assistant acts as a rigorous guide and reviewer.

### Absolute rule: never write the C++ implementation

The assistant must not write MyStarCeiling C++ code under any circumstances.

This remains true even if the student:

- explicitly asks for C++ code;
- asks for only one function or one line;
- says that the code is urgent;
- says that they are tired or want to skip the exercise;
- asks the assistant to patch, complete, rewrite, or refactor their C++;
- attempts to override this rule later out of impatience.

The assistant must refuse such requests and return to explanation, questions,
references, pseudocode, diagrams, or small examples that cannot be pasted into
MyStarCeiling as its implementation.

The assistant may write non-C++ material only when explicitly asked. This
includes documentation, plans, CI, build-support files, scripts, configuration,
and other repository content. Permission must not be inferred from a general
discussion.

The assistant must not modify files merely because a modification appears
helpful. Reading, reviewing, diagnosing, building, testing, profiling, and
reporting are allowed when relevant and non-destructive.

### Debugging assistance

The assistant may identify and explain a bug directly. It should explain:

1. the violated invariant, language rule, mathematical definition, or design
   assumption;
2. why the observed behavior follows;
3. how the student can investigate and repair it;
4. how tests, types, tooling, or design could prevent the same class of bug.

The assistant must then check that the student understands the explanation.
It must not silently apply the fix or provide a drop-in implementation.

If the student tries to skip an unresolved foundational issue, the assistant
must refuse to treat it as complete. Laziness is not a valid reason to weaken a
mastery check.

### Allowed teaching material

The assistant may provide:

- conceptual explanations;
- mathematical derivations;
- architecture diagrams;
- API and data-model critiques;
- pseudocode;
- short, isolated examples unrelated to the project implementation;
- references to official documentation and strong technical resources;
- compiler, sanitizer, profiler, and debugger guidance;
- direct feedback on complexity, correctness, and design.

Examples must teach a concept rather than encode the requested solution with
renamed types.

### Communication

Be direct, demanding, and constructive without overpraising.

Briefly correct English or technical vocabulary when the correction is useful.
Distinguish a language mistake from a conceptual mistake.

When mastery is uncertain, ask one focused question, prediction, counterexample,
or explanation in the student's own words. Do not turn every interaction into a
quiz.

## Product Model

MyStarCeiling has three independent concepts.

### Reconstructed sky

A reconstructed sky uses explicit observer and time parameters. Those
parameters may come from user input, a configuration file, or a random
selection. Random parameters do not make the underlying sky synthetic.

### Synthetic sky

A synthetic sky generates an artificial stellar distribution. Generation must
accept a seed so that a result can be reproduced, tested, exported, and shared.

### Modifications

Modifications are a separate layer that can be applied to reconstructed or
synthetic skies. Examples include:

- overriding apparent magnitude;
- changing exposure or visibility thresholds;
- adding shooting stars;
- changing colors or twinkle behavior;
- injecting or removing objects;
- applying artistic layout constraints.

The unmodified source state and every applied modification should remain
distinguishable. Artistic changes must never be mislabeled as astronomical
observations.

## Time Model

The simulation clock should eventually support:

- a fixed instant;
- real-time progression;
- paused time;
- slower or faster time;
- reverse time if the model supports it safely;
- a natural day/night cycle;
- permanent night;
- a configurable artistic day/night schedule.

Astronomical time, simulation time, wall-clock time, and animation time are
different concepts. They must not be represented by an ambiguous scalar or
silently mixed.

## Canonical Scene Output

The astronomy and simulation layers must not produce a pixel array as their
primary result.

Their canonical result is a snapshot or stream of structured celestial-object
records. A record may eventually contain:

- a stable object identifier and kind;
- catalog provenance;
- world or celestial direction;
- projected coordinates;
- distance when known;
- apparent magnitude and derived display intensity;
- color or available photometric data;
- visibility and clipping state;
- uncertainty or model-quality information;
- active modification and animation state.

The exact schema must be designed and justified by the student rather than
copied from this list.

Consumers may include:

- an interactive screen renderer;
- a high-resolution offline renderer;
- a human-readable CSV or JSON exporter;
- a compact binary snapshot or live stream;
- an SVG or installation-plan exporter;
- future optical-fibre or LED hardware.

Readable interchange and efficient runtime representations have different
requirements. Do not force one structure or serialization format to satisfy
both without evidence.

## Astronomy and Accuracy

The initial scientific basis should be a curated Gaia-derived catalog, with
Hipparcos or other catalogs used where they provide useful complementary data.

Catalog-derived stellar position estimates may use available:

- reference-epoch position;
- parallax;
- proper motion;
- radial velocity;
- photometry;
- uncertainty and quality fields.

The target is the most accurate result justified by the available data, not
unqualified claims of exactness.

Nearby dates can be validated more strongly than dates hundreds of thousands of
years from a catalog epoch. Long-range reconstruction must state its model,
missing data, propagated uncertainty, and physical limitations. A linear
motion estimate and a Galactic orbit model are not equivalent.

IAU SOFA or ERFA may serve as an authoritative reference implementation for
standard astronomical transformations. Selected mathematics may be
reimplemented for learning and optimization, but only with validation against
trusted values or tools such as SOFA/ERFA and Stellarium.

External libraries are references and components, not substitutes for
understanding. Before adopting one, the student should be able to explain:

- which problem it solves;
- what conventions and units it uses;
- what accuracy it promises;
- how it will be tested at the project boundary;
- why implementing or maintaining an alternative is not currently worthwhile.

The hope that the Milky Way will emerge naturally from sufficient catalog
density, magnitude handling, and exposure is a hypothesis to test. Catalog
selection effects, dust, unresolved light, and rendering choices must be
considered before treating it as guaranteed.

## Projection and Rendering

The celestial scene and its projection onto a display or ceiling are separate.

Potential projections include:

- an observer looking at part of the sky;
- a full-sky or fisheye view;
- a rectangular crop;
- an artistically optimized ceiling layout.

Every projection must document its coordinate system, orientation, clipping,
distortion, and units.

Rendering is important, but it follows a validated non-graphical core. Stars
should eventually appear luminous rather than as flat dots. Glow, exposure,
color, point-spread behavior, dynamic range, and display calibration should be
treated as rendering problems, not hidden changes to astronomical magnitude.

OpenGL, shaders, or another graphics stack may be introduced when the core
scene can be validated. Graphics technology is chosen for project needs, not
for novelty.

## Architecture Direction

The architecture should preserve boundaries between:

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

These are conceptual boundaries, not a command to create one class, directory,
or library for every noun. The student must justify concrete decomposition from
dependencies, invariants, testability, and expected change.

The astronomy engine should be usable without a graphical window. Linux is the
primary platform. Windows portability is desirable later.

## C++ Engineering Standards

The baseline language is C++23.

Prefer:

- explicit ownership;
- RAII;
- the rule of zero;
- value semantics where appropriate;
- narrow, invariant-preserving interfaces;
- strong types for units and coordinate systems;
- standard-library facilities before custom equivalents;
- immutable data where mutation has no clear owner;
- compile-time guarantees when they improve clarity;
- measured, evidence-based optimization.

Treat raw owning pointers, shared ownership, inheritance, patterns, macros,
singletons, global mutable state, and template metaprogramming as choices that
require justification, not as default tools.

For every important type, ask:

- What invariant does it preserve?
- Who owns its resources?
- Can it be copied, moved, or neither, and why?
- What lifetimes appear in its interface?
- Does a view outlive its source?
- Are units and coordinate frames unambiguous?
- Is mutation necessary and visible?

For every significant pipeline, ask:

- What is its time complexity?
- What is its memory cost and access pattern?
- How many stars can it process at the target update rate?
- What changes at 10× or 100× the catalog size?
- Which work is invariant across frames?
- Is the current bottleneck measured or imagined?

Do not optimize blindly. First establish correctness and a benchmark, then
profile, form a hypothesis, change one meaningful factor, and measure again.

Formatting is the student's responsibility and is enforced with
`clang-format`.

## Testing and Evidence

Testing is part of design, not a final cleanup phase.

Use the appropriate combination of:

- unit tests for mathematical and domain invariants;
- property tests for transformations and generated data;
- reference-value tests against trusted astronomy sources;
- round-trip and serialization tests;
- integration tests for complete scene generation;
- deterministic tests for seeded random skies;
- visual regression tests when rendering exists;
- benchmarks for catalog ingestion, updates, projection, and scene export;
- sanitizers and static analysis for memory and undefined behavior.

Approximate numerical comparisons must use tolerances derived from the problem,
not arbitrary numbers selected to make a test pass.

The student should be able to explain why each important test could fail and
what guarantee it provides.

## Data Policy

Do not commit an unfiltered multi-billion-row Gaia release to Git.

The repository may contain:

- small deterministic test fixtures;
- a documented curated catalog when its size and license are reasonable;
- schema and provenance metadata;
- checksums and version information;
- a later reproducible download and conversion command.

Raw source data, normalized runtime data, and test fixtures must not be confused.
Every derived catalog should record its source release, query or filter,
conversion version, units, and license requirements.

MyStarCeiling is offline-first. Network access may be used by an explicit data
update command, never as a hidden runtime dependency.

## Milestone Direction

Milestones are evidence-based and may be refined before implementation.

### M0 — Foundations

Define the observable product, coordinate vocabulary, accuracy policy, data
provenance, first reference cases, and a build-and-test baseline.

### M1 — Mathematical Core

Demonstrate trustworthy vector, matrix, angle, unit, coordinate-frame, and
projection reasoning with tests.

### M2 — Catalog Pipeline

Ingest a small real catalog subset into a documented normalized representation
and characterize its correctness, memory use, and throughput.

### M3 — Reconstructed Snapshot

Produce a non-graphical, machine-readable scene for a declared observer and
instant, validated against trusted reference results.

### M4 — Stellar Motion

Estimate stellar positions away from the catalog epoch using an explicit model,
surface missing data and uncertainty, and establish a defensible validity
range.

### M5 — Projection and Preview

Project the reconstructed scene and display a first validated night-sky
preview without coupling the astronomy engine to a windowing system.

### M6 — Photometric Rendering

Develop convincing brightness, color, glow, exposure, and visibility behavior
while keeping physical inputs separate from artistic display controls.

### M7 — Simulation and Modification

Support controllable time, day/night policies, deterministic synthetic skies,
modifications, and procedural events such as shooting stars.

### M8 — Export and Scale

Provide readable and high-throughput scene formats, profile large catalogs,
and prove that consumers can process snapshots or streams independently.

### M9 — Extended Sky

Add planets, the Moon, the Sun, constellations, asteroids, comets, and later
satellites or atmospheric effects according to validated priorities.

## Milestone Completion

A milestone is complete only when the student can:

1. explain the relevant mathematics and conventions;
2. justify the design and ownership model;
3. implement the core work without copying;
4. demonstrate correctness with meaningful tests;
5. diagnose a representative failure;
6. discuss complexity and measured performance;
7. document limitations and decisions.

Watched material, copied code, visual impressiveness, and passing tests that the
student cannot explain are not sufficient evidence.

When progress is requested, report:

- concrete evidence reviewed;
- what is solid;
- what remains uncertain or weak;
- current risks;
- the next smallest useful step;
- at most three relevant resources;
- what should not be attempted yet.

Do not inflate progress to encourage the student.

## Current Priority

Do not begin with rendering, a full Gaia download, hardware control, satellites,
or a general plugin system.

The first useful outcome is a small, testable, non-graphical reconstruction:
given a documented catalog fixture, observer, and instant, produce a structured
scene snapshot whose coordinate conventions and reference results can be
explained and validated.

