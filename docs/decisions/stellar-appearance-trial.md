# Animated stellar viewer

The owner requested an in-executable trial on 2026-10-26 and approved replacing
its slow pulsation with the current scintillation approximation on 2026-10-27.
On 2026-10-28 they provisionally accepted the algorithm as good for now and
requested promotion to the real viewer. Later that day the owner accepted the
stronger native comparison as the default and renamed it the **×1 baseline**.
Temporary comparison selectors, guides and magnifiers are removed. Catalogue
colours were accepted separately. This is not final M2 closeout or physical
calibration. The historical filename is retained for existing links.

## Run

```sh
./msc view
```

Existing catalogue/configuration/logging options apply. No source or binary
rebuild is needed. Plain `msc view` always uses this animated scene; the static
renderer is removed. Both `--appearance-trial` and `--animation-trial` reject
before configuration or launch side effects. No appearance toggle, CLI tuning
option or saved TOML setting is added.

| Key | Action |
| --- | --- |
| Space | Freeze/resume picture mode |
| Escape | Close the window |

The title identifies animated or picture mode. Astronomical time pause allows
animation to continue; picture mode freezes it too. The fixed Toulouse/time
geometry already has paused astronomical time. No sky drift or new observer/time
controls are introduced. Window resizing remains available normally. The viewer
retains VSync and waits when needed to cap rendering at 60 fps, matching the
accepted comparison; this ceiling is not a guaranteed frame rate.

## Revised animation — 2026-10-27

Atmospheric turbulence changes the wavefront and received intensity. Wind moves
these patterns across the observer's pupil. Scintillation has a range of temporal
frequencies, and pupil/exposure averaging suppresses parts of the variation.
Colour signals are related, with atmospheric dispersion affecting their temporal
relationship away from the zenith. These mechanisms motivate the approximation;
the viewer does not reconstruct actual observing conditions.

The former cubic slow/fast noise generator and its clock/settings are removed.
The replacement sums 24 sinusoidal components with independent deterministic
phases from the full catalogue identity and catalogue salt. Band edges run from
0.5 to 256 Hz logarithmically; representative frequencies are 0.569 to 224.8 Hz.
Amplitudes include each band's width, using the surrogate power density
`S(f) = 1 / (1 + (f / 45 Hz)^(8/3))`, normalized to unit instantaneous variance.
This broad plateau and rolloff are a provisional small-pupil rendering choice,
not a spectrum fitted to measured atmospheric data. The 45 Hz knee is selected
for the trial; it is not inferred from catalogue data or a large-telescope law.

The underlying linear signal has a 0.12 channel RMS scale at the horizon before
exposure averaging and the bounded visual response. Its
amplitude factor is `0.25 + 0.75*(1-sin(elevation))^2`. RGB channels share the same
components, with adjacent-channel delays of `1.5 ms*(1-sin(elevation))` in opposite
directions around green. Thus colour is correlated with the intensity signal,
with no differential delay at zenith. The delay is an approximation of chromatic
behaviour, not wavelength-resolved propagation. Catalogue colour and magnitude
remain unchanged.

For each frame, every component is averaged analytically over the trailing
animation-time interval `[t-dt,t]`, clipped to the origin at startup. Its amplitude
is multiplied by `sinc(pi*f*dt)` and its phase evaluated at the midpoint. The
signed coefficients are retained without renormalization. Brightness and colour
share the averaged signal, rather than multiplying independently averaged signals.
Zero exposure gives the instantaneous signal.

The approved visual response is applied **after** this averaging. With `d` the
averaged linear deviation of a channel, the gain is
`1 + 0.9*tanh(10*d/0.9)`. This is the exact response selected in the native
comparison, now called ×1; the historical 10 in the formula is not a user-facing
multiplier or an extra setting. It keeps gains between 0.1 and 1.9, up to
rounding, while preserving a smooth response. The obsolete linear positivity
bound is removed; internal signal scales are finite values in [0,1].

This nonlinear step is a visual approximation, not the exposure average of a
physical amplified light signal. Linear signal averages compose across exposure
partitions; final gains generally do not. It also does not guarantee an exact
unit average for each finite source/time sample. Reduced phases are calculated
in double on the CPU before float uniform upload. Astronomical order and screen
position do not seed animation.

Picture mode preserves both time and the exposure interval. Ignoring elapsed
wall time while frozen prevents a changed exposure from altering a frozen image.
Resume preserves phase and excludes time spent frozen.

## Spatial profile and limits

The quieter earlier preview is restored: core sigma 0.4 framebuffer pixels,
halo sigma 1 with 3% light, and four unequal one-sided rays with longest sigma
1.5 and 4% light. The remaining light is in the core. These remain the current
internal viewer coefficients. A star is a point source with no separately painted opaque dot.
The stable light distribution changes intensity/colour rather than wobbling or
rotating. Profiles redistribute linear light before display encoding.

The pixel-footprint approximation adds variance `1/12` to Gaussian axes.
This reduces aliasing but is not exact pixel integration. Small stars may still
look pixelated. Finite support, pixel sampling and highlight compression limit
appearance; monitor resolution alone does not explain these issues.

The owner's [video reference](https://www.youtube.com/shorts/m34BUDkIoDM) is highly
magnified. It guides qualitative flickering, without supplying naked-eye angular
scale or a calibrated diffraction model. This finite spectral sum can exhibit
beating and does not model full turbulence, measured eye optics or spatial
correlations between nearby stars. Frame averaging attenuates high frequencies
but is not an ideal anti-alias filter. Provisional owner acceptance does not
establish calibrated visual realism.

## Rendering, verification and scaling

The catalogue retains double magnitude and RGB8. Static instances retain ENU,
float display magnitude, RGB8 and their existing integer identity key. Their
layout is unchanged; no per-star history or expanded RGB catalogue is added.
Above-horizon sources outside the viewport remain available for edge footprints.
Projection uses actual framebuffer aspect and the existing 90-degree vertical FOV.

The CPU prepares 24 shared three-float frequency records (288 bytes) per frame.
The vertex shader evaluates the signal; instanced quads accumulate linear light
in RGBA32F. A final pass compresses highlights and encodes sRGB once. The float
attachment uses `width*height*16` bytes, about 14.1 MiB at 1280x720, excluding
other buffers and driver allocations. Resize reallocates it.

Core tests compare analytic linear-signal averaging with numerical integration,
check linear exposure partitioning, variance suppression, bounded gains,
deterministic identity, the approved response and freeze/resume. Before the
stronger response was selected, an offscreen Mesa llvmpipe check of the compiled
renderer with the configured 4,096-source prefix matched CPU/GPU gains within
7.1e-7 over sampled times including 1e12 seconds. Advancing by 1/60 second changed
final pixels; picture mode ignored ten seconds and preserved them exactly.
These checks establish implementation behaviour, not native-window perception.

Promotion of the stronger baseline passed all 218 C++ tests in ordinary and
ASan/UBSan/leak-enabled builds, plus formatting, cppcheck and Lizard checks.
A capture linked to the production renderer matched all 100 frames of the
accepted native-comparison response byte-for-byte at a fixed 1/60-second step
(Mesa llvmpipe, 1280x720, a 16x16-pixel bright-star crop from the configured
4,096-source scene). Picture mode preserved the pixels after ten seconds and
animation resumed correctly. This establishes that promotion preserved the
approved response in that workload; it is not a performance measurement.

The independent dcodex review on 2026-10-28 found no blocker for native visual
review. It identified two follow-ups: forming the midpoint before phase reduction
loses precision at extreme elapsed times (about 32.55 microseconds at `t=1e12`,
`dt=1/60`), and the temporary GPU harness is not retained in the repository.
CPU/GPU agreement at that time does not establish accurate long-time averaging.
No impact on ordinary viewing was demonstrated; these remain numerical and
regression-coverage limits, not calibration or scale evidence.

Work is bounded per source but repeats across quad vertices; cosine/hash work,
covered pixels, blending and source count can dominate at scale. No million-source
performance claim is made. The owner explicitly wants optimization after
appearance review; the algorithm is now provisionally accepted. Measure identical
workloads and preserve the agreed image and animation when comparing changes. See the [scaling follow-up](../todo.md#animation-validation-and-scaling).

Zone contribution bounds and full/compact profiles remain approved design
direction, not implemented optimizations. Combined faint light, flicker peaks,
phase and offscreen contributions must survive any later selection. Moon/cloud
and photographic models remain later work.

## Scientific references

- [Osborn et al. (2015): atmospheric scintillation, wind and exposure averaging](https://doi.org/10.1093/mnras/stv1400).
- [Dravins, Lindegren & Mezey (1993): measured short-timescale intensity fluctuations](https://doi.org/10.1017/S0252921100007454).
- [Kornilov (2011): correlation between spectral bands and atmospheric dispersion](https://doi.org/10.1111/j.1365-2966.2011.19329.x).
- [JPL (2004): point-receiver spectral rolloff and aperture effects](https://tda.jpl.nasa.gov/2000-2009/progress_report/42-158/158F.pdf). The shared wave-propagation theory motivates the rolloff; its radio observing parameters are not used as optical calibration.
