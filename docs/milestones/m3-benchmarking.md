# M3 — Benchmarking and regression evidence

Owner requirements, 2026-10-28. Milestone placement and a separate developer
executable are accepted. Measurement mechanisms and implementation slices below
are proposed for discussion, not implemented or a selected optimization strategy.
M4 documentation follows this milestone, before M5 observer/time/camera work.

## Objective and completion evidence

Measure operation costs, pipeline behavior, memory and rendering across workloads;
retain enough context to compare changes against the accepted image/animation.
The suite must answer what changed, at what workload and with what uncertainty.
It cannot promise a measurable timing difference for every source-line change.
Compiler transformations and measurement noise may hide or eliminate a difference.
Correctness tests, benchmarks and profiling answer different questions and should
be used together. Full coverage of every possible interaction is not an achievable
acceptance criterion; use a maintained matrix of representative and stress cases.

M6 applies and extends this suite for high usage, including the historical
large-catalogue milestone's (formerly M7) memory and scaling concerns.
No mandatory 1M count, streaming strategy or workload
ceiling is selected. M3 does not implement all M6 optimizations in advance.

## Proposed measurement layers

| Layer | Cases and evidence |
| --- | --- |
| Operations | Source-specific colour/epoch conversion, binary decoding, coordinate conversion/projection, shared 24-band preparation, per-source CPU reference scintillation; batch inputs and consume outputs |
| Pipeline stages | Parse/prepare, resolve/select/sort, write/verify, load prefix, construct scene, construct/upload instances, first completed frame |
| GPU passes | Source pass and final display pass separately; use GPU timing rather than interpreting CPU submission time as completed GPU work |
| Controlled experiments | Isolate animation evaluation, profile coverage/blending and display cost using explicit benchmark-only variants of production code; quantify effects in the full renderer too |
| Application | Startup, steady-state throughput, presentation intervals, memory and image/animation checks |

The CPU scintillation reference is not the actual GPU workload. Isolated GPU
signal tests and actual star-pass measurements are both needed to attribute costs.
CPU reference and GPU shader arithmetic may use different precision and operation
ordering; compare their results with justified tolerances, not assumed bitwise
identity, and keep their timings separately labelled. Picture freeze alone does
not eliminate shader signal evaluation; it is a correctness case, not an
animation-off performance control.

## Workload dimensions

- Sweep source counts, beginning with approximately 1k, 4k and 65k where available,
  then increasing by controlled factors within explicit memory/time budgets.
  One million is an example, not a gate. Use true catalogue subsets for real-data
  results; label generated stress fixtures separately.
- Track decoded, above-horizon, submitted, magnitude-eligible and viewport/footprint
  candidate counts separately. A million loaded sources is not a million visible
  flickering sources. Quantify estimated footprint coverage and overlap; exact
  fragment counters, if used, are distinct instrumented cases with their own cost.
- Vary distribution: dispersed, clustered/overlapping and outside-view sources;
  vary brightness so heavy shader processing and heavy fragment/blending cases
  can be distinguished. Repeat dimensions deliberately rather than every possible
  Cartesian combination on every run.
- Vary resolution independently of count. Compare animation sampling at 30 and
  60 Hz while preserving the accepted response coefficients and deterministic keys.
  Keep exposure step independent of benchmark execution speed.
- Measure real source build and binary load separately. Identify OS-cache state
  honestly: a fresh process is not proof of a cold filesystem cache.

## Proposed method and output

Use an optimized, identified build for timing; keep sanitizer/correctness runs
separate. Use monotonic CPU timings and asynchronous OpenGL timer results with
GPU/backend identity, supported counter checks and explicit unavailable results.
Avoid forcing completion after every draw merely to simplify timing. Measure
CPU submission, GPU execution and presentation waits separately; they overlap
and cannot generally be added as sequential frame costs.

Each case gets warm-up and repeated samples. Batch short operations so timer
resolution and call overhead do not dominate; record actual batch iteration counts
and elapsed batch timings before deriving per-operation estimates. Repeated
fresh-process runs are needed where process startup, peak memory or run-to-run
variation matters; within-process repeats alone do not establish that variation.
Report sample counts, median, tail percentiles and variability; timer noise,
scheduling, cache state, power and thermal changes can hide small differences.
Do not treat tiny differences as improvements without stable repeated evidence.
Keep those conditions and competing activity in the run context. The initial
sample schedule remains to select and validate.

Record process peak resident memory, allocations/capacities where attributable,
known CPU/GPU buffer bytes, and transfer volume. Driver-managed/shared GPU memory
is not completely measured by CPU RSS; avoid double-counting shared memory or
calling differences between RSS peaks per-stage allocation costs.

### One result artifact per invocation

Owner output requirement: each benchmark invocation, including a suite invocation,
produces **exactly one machine-parseable result artifact** containing all cases,
repeats, raw timing samples and metadata. Child-process repeats contribute to that
same artifact. The recommended encoding is one structured JSON document; its schema
and library remain to select. Terminal progress or a summary may go to stderr.
CSV, Markdown reports and per-case sidecars are not required outputs.

The proposed artifact contents are:

- Invocation and case IDs, effective configuration and its identity, dataset
  version/checksum or fixture identity, seed where applicable, scene, resolution,
  exposure step and frame limit. Record commit identity, the dirty-state flag and
  a reproducible fingerprint of build inputs, including relevant changed/untracked
  inputs; a commit ID alone does not identify an uncommitted build.
- Build type, compiler/toolchain and relevant flags; CPU/GPU, hardware, OS,
  driver/backend identity and run conditions. Associate each sample with its case,
  repeat and process run so within-process and process-level variation remain
  distinguishable.
- Actual decoded, above-horizon, submitted, magnitude-eligible and candidate
  counts; actual warm-up, sample and batch iteration counts. Keep requested counts
  separate from observed counts. Include raw timings and derived summaries with
  explicit units, plus attributable memory and transfer measurements.
- Handled errors and skipped/unsupported cases in the same artifact, including
  invocation failures before any case completes when the destination is writable.
  Use nullable unavailable metrics
  with reasons, rather than zeros or silently missing evidence. A failed run must
  remain distinguishable from a successful measurement. Failures preventing
  artifact creation must exit unsuccessfully and explain the failure on stderr.
- Optional image/reference comparison metrics, tolerances and outcomes in this
  same artifact. Do not embed huge images or millions of per-particle values by
  default. Debug images and profiler traces are opt-in diagnostic extras outside
  the one-required-result contract.

Write the artifact after timed regions; result formatting and file I/O must not
pollute operation, pipeline or rendering timings. Measure serialization only as
an explicitly separate case if it is of interest. Keep bulky raw results outside
tracked source; retain a small reproducible baseline. Comparison tools may derive
views from the single artifact without making additional reports mandatory.
A comparison reports absolute and relative changes and uncertainty; automatic
performance gates need a stable runner and justified thresholds, not arbitrary
fixed percentages on shared CI.

Before/after correctness checks include source membership, deterministic sampled
CPU/GPU gains, final pixel samples/captures with explicit tolerances, and picture
freeze/resume. Exact cross-driver image identity is not assumed. Preserve normal
viewer behavior and keep benchmark controls in the developer executable.

## Proposed implementation slices

1. Standalone CPU developer executable and single-artifact case/result metadata:
   shared-band preparation, batched reference gains, prepared-prefix loading and
   existing scene construction. Start with identified approximately 1k/4k prefixes,
   observable varied outputs, repeated timings, counts and fresh-process RSS.
   Broader conversion/build cases follow without extracting internals just for
   timing.
2. GPU startup/pass timings and retained offscreen CPU/shader/image checks.
3. Workload sweeps and controlled GPU isolation experiments, memory accounting
   and before/after comparison results; document sampling noise and interpretation.

Each slice requires its own reviewable implementation plan and atomic commits.
No new benchmark framework, timer-query buffering architecture, JSON library,
profiler dependency or optimization is selected by this document alone.
