# AGENTS.md — MyStarCeiling Technical Contract

## Scope

This file contains the public technical contract for this repository. Keep personal
background, career direction, coaching, assessments, collaboration preferences,
private planning and local equipment details out of this file.

Use repository artifacts as the source of truth. Treat dated status and planning
as historical evidence until checked against the working tree and recent history.
Keep volatile implementation status in the linked project records rather than
copying it into this contract.

## MyStarCeiling's Identity and Point

MyStarCeiling is a long-term C++26 astronomy, simulation, data-processing, and
visualization project. Its purpose is to:

- reconstruct a night sky for an observer, location, and time;
- generate deterministic synthetic skies;
- apply explicit artistic modifications;
- preview and render the result beautifully;
- expose reusable machine-readable scene data;
- eventually drive digital or physical star-ceiling installations;
- process real stellar catalogs efficiently and offline.

The central portfolio purpose is C++ systems and optimization evidence. A useful,
attractive sky makes that engineering demonstrable, but visual feature count does
not outrank correctness, profiling, or measured improvement. Never claim more
scientific correctness than the data and model justify.

MyStarCeiling is not a generic graphics demo, a pile of copied astronomy formulae,
a thin wrapper around an existing planetarium, or a superficial AI project.

Its portfolio point is:

> Demonstrate that I can design, build, validate, profile, and optimize a
> technically difficult C++ system using real scientific data.

It should show modern C++, architecture, algorithms, numerical computing, memory
management, concurrency when justified, testing, profiling, scalability, and
performance optimization. Strong evidence matters more than feature count or
visual polish.

## Repository inspection and evidence

Before assessing or changing a project:

1. Read the relevant README, roadmap, milestone notes, source, examples, tests,
   and recent relevant history.
2. Inspect that repository's working tree so uncommitted owner work is not
   overwritten or mistaken for committed evidence.
3. Treat inspected repository artifacts as the source of truth for implementation
   status.
4. Distinguish verified state, intended roadmap, proposed work, and unresolved
   questions.
5. Preserve unrelated changes and useful existing work.
6. Prefer the smallest useful, reviewable step.

Strong evidence includes:

- inspected working code;
- focused tests with meaningful assertions;
- mathematical derivations and explicit conventions;
- debugging notes and failure analysis;
- controlled experiments;
- benchmarks and profiles when performance is discussed;
- written explanations and justified architectural decisions.

For meaningful performance work:

1. establish correctness;
2. define a representative workload;
3. record a baseline;
4. profile;
5. form a concrete hypothesis;
6. make one focused change;
7. measure again.

Ask about time complexity, memory complexity, data access, scaling at 10× and
100×, the dominant bottleneck, and whether optimization is premature. Do not add
concurrency, SIMD, GPU execution, or distributed machinery without a measured need.

## MyStarCeiling Product Model

Keep three concepts independent.

### Reconstructed Sky

A reconstructed sky uses explicit observer and time parameters. Those may come
from input, configuration, or random selection. Randomly selecting the parameters
does not make the underlying sky synthetic.

### Synthetic Sky

A synthetic sky creates an artificial stellar distribution. It must accept a seed
so the result is reproducible, testable, exportable, and shareable.

### Modifications

Modifications are a separate layer that may apply to reconstructed or synthetic
skies. They may alter magnitude, exposure, visibility, color, twinkle, object
membership, artistic layout, or add events such as shooting stars. The source
state and each modification must remain distinguishable. Never label an artistic
change as an astronomical observation.

## MyStarCeiling Time Model

The eventual clock may support a fixed instant, real-time progression, pause,
slowdown, acceleration, safe reverse time, natural day/night cycles, permanent
night, and artistic schedules.

Astronomical time, simulation time, wall-clock time, and animation time are
different concepts. Do not hide or mix them in an ambiguous scalar.

## Canonical Scene and Consumers

The astronomy and simulation layers must not use pixels as their primary output.
Their canonical output is a snapshot or stream of structured celestial-object
records.

Potential record information includes a stable identifier and kind, catalog
provenance, celestial/world direction, projected position, known distance,
apparent magnitude and display intensity, photometric color data, visibility and
clipping, uncertainty or quality, and modification/animation state. This is a set
of design questions, not a schema to copy without justification.

Potential consumers include:

- an interactive screen renderer;
- a high-resolution offline renderer;
- CSV or JSON export;
- compact binary snapshots or live streams;
- SVG or installation plans;
- future optical-fibre or LED hardware.

Readable interchange and efficient runtime representations have different needs.
Do not force one data structure or serialization format to serve both without
evidence.

## Astronomy and Accuracy

Begin from a curated Gaia-derived catalog, with Hipparcos or other catalogs where
they add useful data. Position estimates may use reference-epoch position,
parallax, proper motion, radial velocity, photometry, and uncertainty/quality
fields.

Aim for the most accurate result justified by available data, not unqualified
exactness. Nearby dates can be validated more strongly than times hundreds of
thousands of years from the catalog epoch. Long-range reconstruction must state
the model, missing data, propagated uncertainty, and physical limitations. A
linear-motion estimate is not a Galactic orbit model.

IAU SOFA or ERFA can provide authoritative reference behavior for standard
astronomical transformations. Selected mathematics may be reimplemented for
learning and optimization, but validate it against trusted values or tools such as
SOFA/ERFA and Stellarium.

Before adopting an external library, explain:

- the exact problem it solves;
- conventions and units;
- promised accuracy;
- tests at the project boundary;
- why an in-house alternative is not presently worthwhile.

The Milky Way emerging from catalog density, magnitude handling, and exposure is a
hypothesis to test, not a guarantee. Account for catalog selection effects, dust,
unresolved light, and rendering choices.

## Projection and Rendering

Keep the celestial scene distinct from its projection onto a display or ceiling.
Possible views include an observer's partial sky, a full-sky/fisheye view, a
rectangular crop, and an artistically optimized ceiling layout. Every projection
must document coordinates, orientation, clipping, distortion, and units.

Rendering follows a validated non-graphical core. Glow, exposure, color,
point-spread behavior, dynamic range, and display calibration are rendering
problems, not secret changes to astronomical magnitude. Introduce OpenGL, shaders,
or another graphics stack when the scene can be validated, and choose technology
for project needs rather than novelty.

## Architecture Direction

Preserve conceptual boundaries among:

- catalog ingestion and normalized data;
- mathematics, units, and coordinate systems;
- astronomical and simulation time;
- observer and environment state;
- celestial-state estimation;
- modifications and procedural events;
- projection;
- snapshots or streams;
- renderers and exporters;
- configuration and application orchestration.

These boundaries do not require one class, directory, or library per noun. Derive
the concrete decomposition from dependencies, invariants, testability, and likely
change. The astronomy engine must work without a graphical window. Linux is the
primary platform; Windows portability is a later preference.

## C++ Engineering Standards

The baseline is C++26. Prefer:

- explicit ownership;
- RAII and the rule of zero;
- value semantics where appropriate;
- narrow invariant-preserving interfaces;
- strong types for units and coordinate frames;
- standard-library facilities before custom equivalents;
- immutable data when mutation has no clear owner;
- compile-time guarantees when they improve clarity;
- measured optimization.

Raw owning pointers, shared ownership, inheritance, design patterns, macros,
singletons, global mutable state, and template metaprogramming require specific
justification; they are not defaults.

For every important type, ask:

- What invariant does it preserve?
- Who owns each resource?
- Is it copyable, movable, both, or neither, and why?
- Which lifetimes appear in the interface?
- Can a view outlive its source?
- Are units and coordinate frames unambiguous?
- Is mutation necessary and visibly owned?

For each pipeline, ask about time and memory complexity, access patterns, target
throughput, behavior at 10× and 100× catalog sizes, frame-invariant work, and
whether bottlenecks are measured or imagined.

Formatting is enforced with `clang-format`.

### Scalability and Acceleration

Record enough context to reproduce performance results: dataset size, hardware,
compiler, build configuration, workload, loading time, memory consumption,
bottlenecks, and numerical precision.

Consider data layout, allocations, cache behavior, vectorization, concurrency,
synchronization, contention, numeric representation, and—only when relevant—CPU/
GPU transfer, launch, and synchronization costs.

Introduce CUDA, GPU compute, or compute shaders only after profiling reveals a
real parallel bottleneck and the likely benefit justifies transfer costs,
complexity, and maintenance. Retain a correct CPU baseline for comparison and
validation.

## MyStarCeiling Testing and Evidence

Testing is part of design. Use the appropriate combination of:

- unit tests for mathematical and domain invariants;
- property tests for transformations and generated data;
- reference-value tests against authoritative astronomy sources;
- round-trip and serialization tests;
- integration tests for complete scene generation;
- deterministic tests for seeded skies;
- visual regression tests after rendering exists;
- benchmarks for ingestion, updates, projection, and export;
- sanitizers and static analysis for memory and undefined behavior.

Numerical tolerances must come from the problem and model, not arbitrary values
chosen to make a test pass. Explain what each important test guarantees and
how it could fail.

## MyStarCeiling Data Policy

Do not commit an unfiltered multi-billion-row Gaia release.

The repository may contain small deterministic fixtures, a documented curated
catalog when size and licensing permit it, provenance/schema metadata, checksums
and versions, and a reproducible download/conversion command.

Keep raw source data, normalized runtime data, and test fixtures distinct. Every
derived catalog should record its source release, query/filter, conversion version,
units, and license requirements.

The product is offline-first. Network access may be used by an explicit data
update command, never as a hidden runtime dependency.

## Quality and Git workflow

Use topic branches, Conventional Commits and pull requests. Do not push directly
to `main`, bypass quality checks or rewrite history.
[CONTRIBUTING.md](CONTRIBUTING.md) is the canonical quality-rules reference; inspect
it and the current CI configuration before relying on a historical description
of checks or enforcement.

Preserve readability: do not game metrics, add blanket suppressions or exclude
legacy first-party code to bypass checks. Exclude generated/build artifacts and
vendor dependencies where the canonical rules specify.

Make one atomic Git commit for each logical change when committing authorized
work. Stage only relevant files or hunks, inspect the staged diff, run appropriate
verification and preserve unrelated working-tree changes. Each commit should
express one reviewable reason.

## Documentation discipline

Plan M2.5 for benchmarking, followed by M2.6 project documentation before M3,
as revised by the owner on 2026-10-28. Keep the actionable record in [the project todo list](docs/todo.md#documentation-phase-between-m2-and-m3).
Documentation tooling and publishing format remain to be selected during that
phase; continue maintaining focused decisions and usage notes during M2.

Documentation consumes scarce reader attention. Record durable useful decisions,
contracts, reproducibility metadata, correctness/performance evidence and material
limitations; do not create exhaustive chat history or operational transcripts.
Public technical information belongs in focused files under `docs/`. Keep
personal direction and planning outside the public repository. Keep one canonical
record per subject and link to it instead of copying facts across notes. Distinguish verified state,
approved but unimplemented work, recommendations and open questions.

Keep `docs/development-history/YYYY-MM-DD.md` short and nonduplicative: record
only material changes with links to focused records, the decision's status and
relevant owner rationale. Attribute owner decisions versus agent proposals
accurately; do not invent motivations or dates selected by the owner. Routine
checks, workflow chatter and intermediate observations need no separate entry.

Acquisition records retain release/source, exact canonical query/selection/order
and continuation rule, script version and reproducible command, limits, final
count/size/date/checksum and verification scope. Aggregate repeated failures by
cause and recovery outcome. Do not require every executed retry/poll/curl command,
job or response body in human Markdown. Detailed per-request debugging belongs
in optional ignored runtime traces, with credentials excluded. Do not archive
noise into another public document. Existing verbose logger code is separate
maintenance scope; a documentation cleanup does not authorize changing it.

## C++ file conventions

For C++ repository work:

- every source and test file directly includes what it uses;
- public project headers under `include/` are included with angle brackets;
- implementation-private headers use quotes;
- avoid unnecessary includes in public headers;
- when a custom type from another namespace is used repeatedly in one file,
  prefer a narrow local `using` declaration to repeated full qualification;
- avoid namespace-wide `using` directives;
- mirror the production module hierarchy under `tests/`, and associate tests
  with the implementation they exercise while retaining integration tests for
  cross-module paths.

## Canonical project records

Read [README.md](README.md), [the documentation index](docs/README.md),
[current project state](docs/project-state.md), the relevant milestone and focused
decision records, source, tests and recent history before proposing changes.
Keep status, acquisition evidence, measured record sizes and unresolved technical
choices in their canonical records. Recorded acquisition does not demonstrate
simultaneous loading or rendering, and record-size arithmetic is not peak RSS.

Do not expand observer/time controls, GPU work, hardware control, satellites,
procedural systems or large-catalogue runtime architecture before their
prerequisites and evidence have been established.
