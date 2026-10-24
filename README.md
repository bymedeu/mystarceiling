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

## Run the prepared-catalogue viewer

Configure with the pinned Conan 2/CMake flow in
[CONTRIBUTING.md](CONTRIBUTING.md), including native prerequisites, sanitizer
commands and offline cache use. The same toolchain can configure `build` or
`build/quality`. After configuring `build`, build and test with:

```sh
cmake --build build -j4
ctest --test-dir build --output-on-failure
build/src/cli/msc --help
build/src/cli/msc view --catalogue /path/to/catalogue.bin --quantity 4096
```

Optional local shortcuts can be created at the repository root after building
the corresponding executables:

```sh
ln -s build/quality/src/cli/msc msc
ln -s bin/mystarceiling_tests msc_tests
./msc view
```

These relative links are ignored by Git and are not created automatically on
fresh clones. The viewer link uses the `build/quality` build.

`msc view` loads only the requested prefix of an existing brightness-ordered
binary catalogue. It prepares the unchanged fixed Toulouse M1 scene with the
**temporary G<=4 display filter** and opens a graphical window. Launching does
not read raw sources or rebuild the binary. Use the separate catalogue commands:

```sh
build/src/cli/msc catalogue build
build/src/cli/msc catalogue info
# Temporary overrides; an existing destination requires --force.
build/src/cli/msc catalogue build --gaia /path/to/gaia.csv.gz \
  --hipparcos /path/to/hipparcos-supplement.csv.gz \
  --catalogue /path/to/catalogue.bin --capacity 4096
```

A build reads usable Hipparcos records first, retaining its brightest records
if it fills capacity, then loads only the remaining accepted count from the
ordered Gaia input. It sorts the selected records by brightness before storage.
Capacity is exact and at least 64; insufficient input fails. This membership
policy gives Hipparcos priority. It does not select the global brightest N from
both complete sources. A completed temporary binary is checked before publication;
failed builds preserve the existing catalogue. Source files cannot be overwritten.
See the [build contract](docs/decisions/application-controls.md#catalogue-controls).

`catalogue info` reads the header and checks total file length, without decoding
records or reading either source. Example output:

```text
Catalogue: /path/to/catalogue.bin
Records: 4096
File size: 303136 bytes (0.000303 GB)
```

GB uses 1,000,000,000 bytes, with three decimal places for values of at least
0.001 GB; smaller sizes retain leading zeros and three significant digits.
The former positional source-building launcher has been removed.
M1 remains complete; M2 part 1 is implemented and the broader
[owner-understanding checkpoint](docs/project-state.md#required-understanding-checkpoint)
remains open.

Root options `--config`, `--log-file` and `--log-level` also work after subcommands.
`build` accepts `--gaia`, `--hipparcos`, `--catalogue`, `--capacity` and `--force`;
`info` accepts `--catalogue`. Each command also accepts `--path` as an alias
for `--catalogue`; building creates missing destination parent directories.
Without overrides, commands use TOML settings.
`view` accepts `--catalogue` and `--quantity` (integer, `64 <= N <= catalogue count`).
Bare `msc`, `--help`, `view --help` and `--version` have no configuration, logging
or catalogue side effects.

Configuration defaults to `$XDG_CONFIG_HOME/mystarceiling/config.toml`, falling
back to `$HOME/.config/mystarceiling/config.toml`. The default catalogue is
`$XDG_DATA_HOME/mystarceiling/catalogue.bin`, falling back to
`$HOME/.local/share/mystarceiling/catalogue.bin`. Unset, empty or relative XDG
values use the HOME fallback; that fallback requires an absolute HOME.
`--config` chooses a different file. A missing file is created with concrete
initial paths, quantity 4096, log destination `/tmp/mystarceiling.log` and level
`warning`. Existing files are preserved, and CLI overrides are never saved.

The complete current TOML schema is:

```toml
[catalogue]
path = "/absolute/path/to/catalogue.bin"
capacity = 4096
[sources]
gaia = "/absolute/path/to/gaia.csv.gz"
hipparcos = "/absolute/path/to/hipparcos-supplement.csv.gz"
[view]
quantity = 4096
[logging]
file = "/tmp/mystarceiling.log"
level = "warning"
```

All fields are required; wrong types, unknown keys/tables and invalid logging
levels fail. Existing configurations must include the new capacity and source
fields; incomplete files fail without being rewritten. Generated source defaults
are `sources/gaia-dr3.csv.gz` and `sources/hipparcos2-supplement.csv.gz` beneath
the same data root as the default binary. Only a command consuming a source
opens it; a build filled by Hipparcos does not open Gaia. Relative TOML paths
use the config directory; relative CLI paths use the working directory. TOML
strings do not expand environment variables. CLI overrides apply before the
reader checks the effective quantity against the opened catalogue's count.

Logging affects spdlog messages only; command output and errors stay on the
terminal. Levels are `trace`, `debug`, `info`, `warning`, `error`, `critical`,
`off`. A nonempty log file is created or truncated once per normal launch;
`--log-file ''` uses the standard spdlog stderr sink. Failure to open the log
stops the launch. A log destination cannot overwrite the catalogue, a source
or the configuration.

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
The public technical contract lives in [AGENTS.md](AGENTS.md).

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
| M2 — Ceiling appearance and data controls | 2026-12-13 | Realistic-looking colour/brightness, modest glow, Hipparcos-first catalogue preparation, quantity control, useful bounded inspection/projection |
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
