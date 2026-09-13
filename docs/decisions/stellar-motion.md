# Stellar proper-motion propagation

Status: core operation and bounded Hipparcos M1 scene integration implemented
and verified on 2026-09-13.

## Selected model

Estimate a new ICRS direction from a catalogue direction, its reference epoch,
and the two published tangent-plane proper-motion components. The model assumes
zero radial velocity and uses linear displacement in three-dimensional tangent
space followed by normalization:

\[
\begin{aligned}
\mathbf{r} &= (\cos\delta\cos\alpha,\;\cos\delta\sin\alpha,\;\sin\delta),\\
\mathbf{e}_{\alpha} &= (-\sin\alpha,\;\cos\alpha,\;0),\\
\mathbf{e}_{\delta} &= (-\sin\delta\cos\alpha,\;
-\sin\delta\sin\alpha,\;\cos\delta),\\
\mathbf{r}' &= \operatorname{normalize}\left(
\mathbf{r} + \Delta t
(\mu_{\alpha *}\mathbf{e}_{\alpha}+
\mu_{\delta}\mathbf{e}_{\delta})\right).
\end{aligned}
\]

`mu_alpha_star` is the right-ascension rate already multiplied by
`cos(declination)`. The implementation converts milliarcseconds per Julian year
to radians per Julian year and never divides by `cos(declination)`. This avoids
the right-ascension coordinate singularity at the celestial poles.

The result is converted back to right ascension and declination. Right ascension
is canonicalized to `[0, 2 pi)`, and declination is recovered with `atan2` rather
than `asin` to avoid domain errors caused by rounding. Exact zero elapsed time or
zero motion preserves the input direction.

## Architectural boundary

`ReferenceEpoch`, scalar `ProperMotion`, and the named
`ProperMotionComponents` value belong to the source-independent core. Catalogue
parsers populate them, while the propagation operation consumes them without
depending on Gaia, Hipparcos, identifiers, photometry, parallax, observer state,
projection, or rendering.

The two components are grouped and named so the propagation boundary does not
accept two indistinguishable positional arguments. The original core-only change left the binary schema unchanged. Combined M1
integration subsequently introduced version/schema 2 for provenance and optional
colour; the two motion values retain their binary64 order.

## Errors and logging

The operation returns `std::expected` and distinguishes a non-finite epoch
difference, a non-finite displacement, and an invalid calculated direction. It
does not log. M1 reports an aggregate failure count and skips failed records.
Future bulk propagation should report timings
at the orchestration boundary and sample individual failures only when detailed
diagnostics are enabled. Per-star logging would distort the performance of the
hot loop.

## Verified behavior

Focused tests cover:

- exact identity for zero time or zero motion;
- forward and backward motion;
- the Gaia/Hipparcos `pmRA * cos(dec)` convention at high declination;
- both celestial poles and a near-pole direction;
- right-ascension wrapping across zero;
- distinct non-finite epoch and displacement failures;
- overflow-resistant normalization of very large finite displacements;
- agreement with an independently rearranged spherical calculation of the same
  selected model;
- preservation of named components through Gaia parsing and binary round trips.

The complete suite passed 106 of 106 tests on 2026-09-13.

## Limits

- The caller must supply reference and target epochs expressed as compatible
  Julian epochs. The type does not encode a time scale.
- This operation does not convert a UTC civil timestamp into a Julian epoch.
- Parallax is unnecessary for this zero-radial-motion barycentric direction
  estimate, but remains relevant to observer-dependent annual parallax.
- The model omits radial velocity, perspective acceleration, binaries, Galactic
  dynamics, precession-nutation, aberration, refraction, and other
  observed-place effects.
- Passing numerical tests does not establish accuracy over arbitrary historical
  or future intervals. A supported date range and tolerance remain an M3
  decision.
- M1 leaves Gaia J2016 directions unchanged and propagates Hipparcos J1991.25
  directions to J2016 before local transformation. Source measurements remain
  unchanged. This is the approved catalogue epoch target, not a claim that
  `2016-01-01 00:00:00 UTC` equals the precise J2016 Julian epoch.

## Reference behavior

- [ERFA `pmpx`](https://github.com/liberfa/erfa/blob/master/src/pmpx.c) documents
  a standard position/proper-motion path that also accounts for parallax,
  radial velocity, and observer position. Direct ERFA comparison for MSC's
  simplified boundary remains future validation; the current test does not
  claim full ERFA parity.
