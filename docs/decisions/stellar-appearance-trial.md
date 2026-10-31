# Animated stellar viewer

The owner requested an in-executable trial on 2026-10-26 and approved replacing
its slow pulsation with the current scintillation approximation on 2026-10-27.
On 2026-10-28 they provisionally accepted the algorithm as good for now and
requested promotion to the real viewer. Later that day the owner accepted the
stronger native comparison as the default and renamed it the **×1 baseline**.
Temporary comparison selectors, guides and magnifiers are removed. Catalogue
colours were accepted separately. The implemented scope's later engineering
closeout is recorded below; physical calibration and demonstrated owner mastery
are not claimed. The historical filename is retained for existing links.

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
retains VSync and waits when needed to cap rendering at 30 fps, as subsequently
requested by the owner on 2026-10-28; this is not a guaranteed frame rate.
The approved response remains the ×1 baseline. Longer frame intervals suppress
more high-frequency variation, so 30 fps need not look identical to 60 fps; no
compensating strength or speed change is applied. The animation clock's initial
exposure is also 1/30 second. Historical 60 fps comparisons below retain their
original sampling conditions.

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
The vertex shader evaluates the signal and then applies the later
[faint-star colour reduction](stellar-colour.md#faint-star-desaturation--2026-10-31);
instanced quads accumulate linear light in RGBA32F. A final pass compresses highlights and encodes sRGB once. The float
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
No impact on ordinary viewing was demonstrated. The retained checks below
address the missing harness; extreme-uptime precision remains a numerical limit,
not calibration or scale evidence.

Work is bounded per source but repeats across quad vertices; cosine/hash work,
covered pixels, blending and source count can dominate at scale. No million-source
performance claim is made. The owner explicitly wants optimization after
appearance review; the algorithm is now provisionally accepted. Measure identical
workloads and preserve the agreed image and animation when comparing changes. See the [scaling follow-up](../todo.md#animation-validation-and-scaling).

Zone contribution bounds and full/compact profiles remain approved design
direction, not implemented optimizations. Combined faint light, flicker peaks,
phase and offscreen contributions must survive any later selection. Moon/cloud
and photographic models remain later work.

## Retained regression checks — 2026-10-30

The developer-only `MSC_ENABLE_OPENGL_TESTS` target now retains six checks.
They use the production instance upload, shaders and draw functions, without
new viewer controls or changed appearance coefficients. See the
[run instructions](../../CONTRIBUTING.md#headless-renderer-regression-checks).

Coverage includes CPU/shader light before tone mapping for every quad vertex,
independent identity keys/elevations, 1/60- and 1/30-second exposure, longer
intervals with negative averaging coefficients, and startup clipping. Pixel
checks verify RGB8 linearity, additive overlap before compression, one sRGB
encoding, changing 30 Hz frames, the installed Space callback and exact picture
freeze/resume, magnitude cutoff, retained offscreen edge light, and floating
attachment/viewport resize inside a fixed pbuffer. That last check does not
establish native-window resize behavior.

Verification on 2026-10-30:

| Check | Result |
| --- | --- |
| Ordinary warning-as-error build and CTest | 224 passed: 218 existing plus six OpenGL; no skips |
| Existing tests with ASan/UBSan and leak detection | All 218 passed |
| Optional OpenGL assertions with ASan/UBSan | All six passed; process exit still fails leak detection as described below |
| Offline Python tests | All 35 passed |
| Formatting, cppcheck, Lizard and whitespace | Passed |

Ordinary GPU-path checks used llvmpipe (LLVM 23.1.1, 256 bits), OpenGL 4.6 Core,
Mesa 26.2.4-arch1.1; a second run passed all six assertions on softpipe, OpenGL
3.3 Core. These software-provider checks are not performance measurements or
native-display/physical calibration.

At that checkpoint, the optional leak-enabled headless runs reported two indirect 56-byte allocations
per created/drawn GLFW context after teardown (112 bytes per isolated CTest case;
672 bytes when all six cases share one process invocation). The same report
reproduced in a standalone GLFW null/EGL triangle draw with no MSC or GTest code,
on both software renderers. A direct-EGL triangle draw was leak-free; explicit
EGL thread release did not resolve the GLFW-path report. This localizes the
reproduction to the external context/driver path but does not identify its exact
allocation owner. No suppression, sanitizer disabling, vendor patch or hidden
passing status was added. Full leak-clean headless validation was unresolved
on that provider stack at this checkpoint; the later workaround below establishes
a passing result only for its explicitly changed test environment. The standard
default-OFF GPU option leaves existing checks unchanged.

## Headless leak-check workaround — 2026-10-31

Further local investigation mapped the two allocation stacks to
`libgallium-26.2.4-arch1.1.so`. Disassembly matches the two 56-byte allocator nodes
created by `u_mmInit` for a global 10 MiB executable heap. This identifies Mesa's
RTASM heap as the likely owner by comparison with the
[RTASM source](https://chromium.googlesource.com/chromium/deps/mesa/+/3a0db227ffe90888ad760c61a63226988c974e00/src/gallium/auxiliary/rtasm/rtasm_execmem.c);
the installed module had no matching debug symbols. This is source-pattern and
runtime evidence, not a symbolized source-line attribution.

The [documented Mesa override](https://docs.mesa3d.org/drivers/llvmpipe.html)
`GALLIUM_OVERRIDE_CPU_CAPS=nosse` avoids that helper path. The standalone GLFW
reproduction and all six production-renderer checks exit cleanly with this
setting. CMake now applies it through a CTest environment property only to the
optional OpenGL tests in x86 sanitizer builds. Leak detection remains enabled;
there is no LSan suppression, driver pinning or vendor patch. The underlying
allocation report with the default driver path is not fixed upstream by this
change. The ordinary test suite and native viewer retain that default path.

Verification after the cleanup:

| Check | Result |
| --- | --- |
| Ordinary warning-as-error build and CTest | All 224 passed; no skips; default Mesa path |
| ASan/UBSan/leak-enabled build and CTest | All 224 passed; no skips; portable Mesa override for the six OpenGL cases |
| Offline Python tests | All 35 passed |
| Cppcheck, Lizard, C++ formatting and whitespace | Passed |
| Configured clang-tidy include checks | All 51 first-party translation units passed; temporary database omitted only GCC's unsupported `-freflection` option |
| Ruff syntax/import/undefined-name checks | Passed for all four Python files |
| Ruff formatting audit | Existing drift in `acquire_gaia.py` and `test_acquire_gaia.py`; unchanged in this bounded cleanup |

The audit removed unused `foo` scaffolding and the empty CLI header, corrected
direct includes, and avoided a redundant shift when serializing a byte-sized
field. Existing byte/round-trip tests pass without a binary-format change. Both
`build/quality` and the older `build` viewer executables were rebuilt. The Python
formatting drift is a separate maintenance finding: applying the formatter also
makes an acquisition function exceed the physical-length rule, so a useful
refactor should accompany it rather than suppressing either check.

These results validate the tested paths; software-provider coverage and the
portable-path sanitizer workaround do not establish native-driver leak freedom,
physical colour accuracy or performance. Perceived-colour reduction was
unimplemented at this audit checkpoint; the later
[faint-star appearance update](stellar-colour.md#faint-star-desaturation--2026-10-31)
adds it without changing the catalogue or this workaround.

## Scientific references

- [Osborn et al. (2015): atmospheric scintillation, wind and exposure averaging](https://doi.org/10.1093/mnras/stv1400).
- [Dravins, Lindegren & Mezey (1993): measured short-timescale intensity fluctuations](https://doi.org/10.1017/S0252921100007454).
- [Kornilov (2011): correlation between spectral bands and atmospheric dispersion](https://doi.org/10.1111/j.1365-2966.2011.19329.x).
- [JPL (2004): point-receiver spectral rolloff and aperture effects](https://tda.jpl.nasa.gov/2000-2009/progress_report/42-158/158F.pdf). The shared wave-propagation theory motivates the rolloff; its radio observing parameters are not used as optical calibration.
