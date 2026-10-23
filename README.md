<div align="center">

<img width="100%" src="docs/assets/night-sky-banner.svg" alt="MyStarCeiling — animated night-sky banner"/>

# MyStarCeiling

### Recreate the night sky. Any place. Any time.

[![Status](https://img.shields.io/badge/Status-M1%20complete-111827)]()
[![Language](https://img.shields.io/badge/C%2B%2B-26-00599C?logo=cplusplus)]()
[![Platform](https://img.shields.io/badge/Platform-Linux-0F172A?logo=linux)]()
[![Focus](https://img.shields.io/badge/Focus-Astronomy%20%2B%20Simulation-4338CA)]()

</div>

---

## Overview

MyStarCeiling is a C++ engine for reconstructing, generating, modifying, and
rendering night skies.

Given an observer, a location, and a time, it aims to reproduce the visible sky
from astronomical catalog data. The same engine can generate deterministic
synthetic skies and apply explicit artistic modifications without confusing
them with reconstructed observations.

MyStarCeiling is intended to work offline and to remain independent of any
single output device. Its generated scenes may be displayed on screen, exported
for other software, or eventually used to drive optical-fibre and LED ceiling
installations.

The project is also a serious exploration of modern C++ applied to a
substantial astronomy, systems, and performance problem. Its implementation
emphasizes ownership, data modeling, numerical correctness, algorithms,
testing, architecture, scalability, profiling, and measured optimization.

## Licence

Original MyStarCeiling software is licensed under the [MIT License](LICENSE),
copyright (c) 2026 bymedeu, using the [official MIT text](https://opensource.org/license/mit).
Dependencies and catalogue data retain their own licences and attribution
requirements; this licence does not relicense them.

---

## Run the completed M1 slice

After configuring the project, build and test with:

```sh
cmake --build build -j4
ctest --test-dir build --output-on-failure
build/src/cli/mystarceiling_cli \
  data/gaia-dr3/gaia-dr3.csv.gz \
  data/hipparcos2/runs/20260912T191816.016104Z/hipparcos2-supplement.csv.gz \
  /tmp/msc-m1.bin
```

The Gaia argument uses the completed local archive's ordered prefix. The existing
Hipparcos preparation is tied to the previous Gaia snapshot; see the
[acquisition and supplement caveat](docs/project-state.md#acquisition-status).
The application selects 4096 combined Gaia/Hipparcos stars, calculates the fixed
Toulouse sky, and applies an owner-approved **temporary G<=4 display filter**.
It requires a working graphical session. **M1 is complete on 2026-09-13**, with
117 passing tests and owner visual review of the combined application.
**M2 is the next planning target.** Automatic visibility and future CLI/GUI
controls remain unimplemented; see the screenshot and evidence in the
[M1 record](docs/milestones/m1-fixed-toulouse-sky.md).

## Product Objectives

The project aims to:

- reconstruct skies from real stellar catalogs such as Gaia and Hipparcos;
- perform explicit, validated coordinate transformations for a declared
  observer and instant;
- estimate stellar positions across time from the best available measurements
  while exposing model limitations and uncertainty;
- produce realistic brightness, color, glow, exposure, and sky projections;
- represent each scene as structured celestial-object data rather than only as
  pixels;
- support real-time, accelerated, slowed, paused, and configurable day/night
  simulation;
- generate reproducible synthetic skies;
- apply traceable artistic modifications and procedural events;
- scale from small validated fixtures to large catalogs, with reproducible
  profiling and benchmark evidence;
- export scenes in readable and high-throughput formats;
- keep astronomy, simulation, rendering, and output hardware independent.

---

## Design Principles

See [Contributing](CONTRIBUTING.md) for local checks, branch/PR policy and the
current enforcement limitations.

Scientific inputs and artistic controls remain distinct. Reconstructed skies
should be validated against authoritative astronomical references, while
synthetic and modified skies should remain reproducible and clearly identified.

The engine's canonical output is a structured snapshot or stream of visible
celestial objects. Screen renderers, data exporters, and future ceiling systems
consume that representation independently.

MyStarCeiling targets C++26 and Linux first, with correctness, explicit
conventions, deterministic behavior, clear architecture, and evidence-driven
performance work as core engineering requirements. Optimizations follow
measurement and profiling rather than speculation.

The verified toolchain is GCC 16.2 with CMake 4.4. CMake 3.25 or newer and a
compiler with C++26 support are required; older CMake/compiler combinations
may not recognize the needed standard flag. GCC 16 and newer also enable
experimental C++26 reflection (`-freflection`). Reflection support in other
compilers is not guaranteed.

---

## Algorithmic Ownership Gate

AI-generated implementation is welcome; ownership of the algorithms, data
structures, and important decisions stays with the owner. Before non-trivial
implementation, stop until the owner can explain the problem, data structures,
data flow, correctness assumptions, time and memory complexity, alternatives,
and debugging or optimization starting points in their own words, then explicitly
approve implementation. Missing understanding requires explanation, questions,
and owner teach-back; documentation or model answers alone do not pass the gate.
These are understanding criteria, not a questionnaire to repeat: discussion and
focused follow-ups can establish them in any order.

Depth is proportional to the change; mechanical work and trivial plumbing do
not require this ceremony. Gaps in existing mechanisms are understanding debt
to close before building important functionality on them. The current catalogue
pipeline is the first checkpoint, required before the next substantial
implementation task; see [current project state](docs/project-state.md).
The enforceable rule lives in the portfolio-level
[AGENTS.md](../AGENTS.md#algorithmic-ownership-gate).

## Status

MyStarCeiling is in early development. The fixed M1 Toulouse path now loads real catalogue data, transforms it, and renders a zenith view. Owner visual acceptance is recorded; configurable observer/time controls and measured scaling remain future work.

## Milestones

Dates below are **planning estimates**, revised on 2026-10-18 from owner
capacity constraints: roughly alternating project weeks now, with significantly
lower availability starting January–February 2027. They are capacity assumptions,
not hourly estimates. Detailed scope remains **to discuss**, not implementation
approval. Reduce optional scope before correctness or reproducible evidence.

| Stage | Planning estimate | Intended evidence / scope to discuss |
| --- | --- | --- |
| M0 — Typed astronomy foundations | Existing foundations retained | Working typed path supporting M1; no new accuracy claim |
| M1 — Fixed Toulouse sky | Completed 2026-09-13 | Working 4096-source catalogue-to-screen baseline |
| M2 — Ceiling appearance and data controls | 2026-12-13 | Realistic-looking colour/brightness, modest glow, magnitude-first selection, proposed optional quantity cap, useful bounded inspection/projection |
| M3 — Validated observer and time | 2027-02-28 | Declared model/range, independent validation, controlled astronomical progression |
| M4 — Million-source CPU baseline | 2027-04-25 | Actually measured 1,048,576-source pipeline and bounded-memory preparation baseline |
| M5 — Profile-guided CPU work and output boundary | 2027-06-27 | Justified improvements, explicit consumer boundary, reproducible before/after evidence |
| Clean release gate | 2027-08-31 | Demonstrable, documented, tested portable CPU release; July–August buffer and hardening |

The clean project must be demonstrable by September 2027. July–August is reserved
for validation, documentation, usability, reproducibility and scope recovery,
without new mandatory features. A quantity control bounds selected work; it does
not eliminate measured million-source scaling work. Larger nested workloads are
measurement cases, not a requirement to display every source.

The **16 GB Linux laptop has no NVIDIA GPU** and is the core delivery target.
The RTX 3060 belongs to a **separate Windows desktop**; Linux there is only a
future possibility. CUDA must not block portable CPU delivery. No desktop access,
CUDA installation or system change is assumed.

M6–M9 retain ambitions as a **conditional optional backlog**, with no mandatory
2028 dates and no committed milestones after December 2027. At most **one**
extension may be selected for September–December 2027, according to evidence,
capacity and owner priority; selecting none is valid:

- **M6 — CUDA comparison:** only for a justified measured workload and available
  separate desktop/setup; include transfer/synchronization costs and CPU agreement.
- **M7 — Large-catalogue runtime:** measured out-of-core processing within bounded
  RAM; acquisition of 134,217,728 rows is already complete, but loading/rendering
  them simultaneously is not demonstrated. Chunking, mapping, indexing or streaming
  remain candidates requiring approval, not selected architecture.
- **M8 — Physical bodies:** Sun/Moon/planet geometry, illumination and validated
  events, with declared ephemerides/range; independent optional scope.
- **M9 — Advanced physical, procedural and photographic appearance:** deterministic
  synthetic skies, meteors/Perseids, clouds and bounded exposure experiments remain
  ambitions, not a mandatory feature count. A bounded visible-light long-exposure
  prototype could be the single extension. Full infrared/spectral realism is not
  promised by release or by current catalogue data.

Camera orientation and astronomical time remain independent. Share appropriate
coordinate mathematics where justified, without forcing one universal implementation.
M2 camera extent remains open; observer/time validation belongs to M3. See the
focused M2 plan and appearance/output decisions before discussing implementation.

See the [M2 planning note](docs/milestones/m2-ceiling-appearance.md),
[appearance/output decisions](docs/decisions/ceiling-product-output.md) and
[record-size accounting](docs/decisions/memory-accounting.md). The original
2026-09-11 schedule is retained as superseded history in
[objectives and decisions](docs/objectives-and-decisions.md#historical-milestone-schedule--approved-2026-09-11-superseded).

The detailed decision record and milestone acceptance evidence live in
[the project documentation](docs/README.md).
