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

The project is also a practical exploration of modern C++ applied to a
substantial, performance-sensitive system. Its implementation emphasizes
ownership, data modeling, numerical correctness, testing, architecture, and
measured optimization.

---

## Product Objectives

The project aims to:

- reconstruct skies from real stellar catalogs such as Gaia and Hipparcos;
- estimate stellar positions across time from the best available measurements
  while exposing model limitations and uncertainty;
- produce realistic brightness, color, glow, exposure, and sky projections;
- represent each scene as structured celestial-object data rather than only as
  pixels;
- support real-time, accelerated, slowed, paused, and configurable day/night
  simulation;
- generate reproducible synthetic skies;
- apply traceable artistic modifications and procedural events;
- process large catalogs efficiently and export scenes in readable and
  high-throughput formats;
- keep astronomy, simulation, rendering, and output hardware independent.

---

## Design Principles

Scientific inputs and artistic controls remain distinct. Reconstructed skies
should be validated against authoritative astronomical references, while
synthetic and modified skies should remain reproducible and clearly identified.

The engine's canonical output is a structured snapshot or stream of visible
celestial objects. Screen renderers, data exporters, and future ceiling systems
consume that representation independently.

MyStarCeiling targets C++23 and Linux first, with correctness, explicit
conventions, deterministic behavior, and measured performance as core
engineering requirements.

---

## Status

MyStarCeiling is in early development. Its first implementation
target is a validated, machine-readable sky snapshot for one observer and one
instant using a small stellar catalog.
