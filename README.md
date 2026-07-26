<div align="center">

<img width="100%" src="docs/assets/night-sky-banner.svg" alt="MyStarCeiling — animated night-sky banner"/>

# MyStarCeiling

### Recreate the night sky. Any place. Any time.

[![Status](https://img.shields.io/badge/Status-Foundations-111827)]()
[![Language](https://img.shields.io/badge/C%2B%2B-23-00599C?logo=cplusplus)]()
[![Platform](https://img.shields.io/badge/Platform-Linux-0F172A?logo=linux)]()
[![Focus](https://img.shields.io/badge/Focus-Astronomy%20%2B%20Simulation-4338CA)]()

</div>

---

## What is MyStarCeiling?

MyStarCeiling is a C++ engine for reconstructing, generating, modifying, and
displaying night skies.

Given an observer and an instant, it aims to estimate how the sky should look
from that place and time—from yesterday to the distant past or future, within
the limits of the available observations and physical model. It can also create
seeded synthetic skies when realism is not the objective.

The long-term goal extends beyond a desktop preview. MyStarCeiling is designed
to produce structured celestial scenes that can feed different outputs:
interactive renderers, offline visualizations, installation plans, or future
optical-fibre and LED star ceilings.

---

## One Sky, Several Modes

### Reconstructed

Select a location, orientation, date, and time to reconstruct a real sky from
catalog data. The parameters can be supplied directly, loaded from a
configuration, or selected randomly.

### Synthetic

Generate an artificial but visually convincing stellar distribution. A seed
makes every generated sky reproducible.

### Modified

Apply controlled changes to either kind of sky: exaggerate a star's magnitude,
alter visibility and exposure, introduce shooting stars, or create an
impossible composition deliberately.

Real data and artistic changes remain separate, so the result always knows
which parts are reconstructed and which are invented.

---

## A Scene Before an Image

MyStarCeiling's central output is not a bitmap. It is a structured snapshot or
stream of the celestial objects currently in the scene.

Each consumer can interpret that scene differently:

```text
Catalog + observer + time
            │
            ▼
   Celestial simulation
            │
            ▼
 Structured scene snapshot
       ┌────┼───────────┬──────────────┐
       ▼    ▼           ▼              ▼
    Screen  JSON/CSV  Binary stream  Ceiling output
```

A readable table makes results inspectable and portable. A compact in-memory or
binary representation can support hundreds of thousands of stars at animation
speed. Renderers remain free to produce pixels, glow, exposure, and other
display effects without embedding those concerns in the astronomy engine.

---

## Time Is Controllable

The simulation clock is intended to support:

- a fixed astronomical instant;
- real-time playback;
- accelerated or slowed time;
- paused and, where valid, reversed time;
- the natural day/night cycle;
- permanent night;
- a configurable artistic day/night schedule.

This makes MyStarCeiling useful both as a reconstruction tool and as a living
ceiling scene.

---

## Scientific Grounding

Stellar reconstruction will be based on curated astronomical catalogs such as
Gaia and Hipparcos. Positions away from a catalog's reference epoch can be
estimated from the measurements available for each star, including proper
motion, parallax, and radial velocity.

Accuracy is part of the output, not a marketing claim. A sky reconstructed near
the reference epoch can be trusted more strongly than one projected hundreds of
thousands of years away. Models, missing measurements, and uncertainty will be
made explicit.

The target is planetarium-quality mapping validated against authoritative
astronomical routines and established tools such as Stellarium.

---

## Visual Direction

The renderer should make stars feel luminous rather than drawn:

- brightness derived from apparent magnitude;
- color informed by available photometry;
- convincing glow and exposure;
- projection onto a display, fisheye view, or rectangular ceiling;
- a dense Milky Way emerging from stellar data where the catalog and rendering
  model make that possible;
- later animation for twinkling, shooting stars, and satellites.

Scientific inputs and artistic controls remain distinct. A magnitude override
of `-20` is allowed—but it is recorded as an intentional modification.

---

## Architecture

The project is expected to grow around independent, testable layers:

```text
mystarceiling/
├── apps/          # command-line tools and future interactive preview
├── data/          # small fixtures and documented derived catalogs
├── docs/          # models, decisions, validation, and milestones
├── include/       # public C++ interfaces
├── src/           # engine implementation
├── tests/         # correctness and reference-value tests
├── benchmarks/    # measured performance experiments
└── tools/         # explicit catalog download and conversion utilities
```

The astronomy engine will remain usable without a graphical window. Projection,
rendering, serialization, and future hardware outputs consume its scene data
rather than becoming part of its core.

---

## Roadmap

| Milestone | Focus |
| --- | --- |
| M0 | Conventions, reference cases, and engineering foundations |
| M1 | Linear algebra, units, coordinate systems, and projections |
| M2 | Catalog ingestion, normalization, and provenance |
| M3 | A validated, non-graphical reconstructed sky snapshot |
| M4 | Proper motion, long-range estimates, and uncertainty |
| M5 | Projection and first interactive preview |
| M6 | Magnitude, color, glow, exposure, and visual calibration |
| M7 | Time control, synthetic skies, modifications, and events |
| M8 | Scene export, streaming, profiling, and large catalogs |
| M9 | Solar-system objects, constellations, and extended animation |

The first target is deliberately modest: turn a small, documented catalog
fixture into a machine-readable sky snapshot for one observer and one instant,
then validate the result.

---

## Engineering Principles

MyStarCeiling is built around a few durable rules:

- correctness before optimization, measurement before performance claims;
- explicit units, coordinate systems, ownership, and lifetimes;
- offline operation, with catalog updates performed explicitly;
- deterministic tests and seeded generation;
- astronomy, projection, rendering, and export kept independent;
- documented approximations instead of false precision;
- modern C++23 with portable Linux-first design.

---

<div align="center">

*"The sky is not a backdrop. It is a moment, a place, and a point of view."*

</div>
