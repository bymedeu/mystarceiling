# Prepared stellar colour

Owner-selected M2 direction, 2026-10-25: compact integer RGB, sRGB primaries,
separate double-precision G-like magnitude, and temperature-based approximate
observed colour. Preparation computes in double precision; the catalogue stores
three unsigned bytes in both the binary and runtime `Star`. Magnitude is not
quantised into colour. There is no alpha channel or persistent float RGB copy.

## Encoding and model

`LinearRgb8` contains red, green and blue in that order. Each byte represents
its linear-light channel proportion divided by 255. The temperature appearance
fit normalises its largest channel to one; these are colour coefficients rather
than physical luminosity. The worst rounding error is half a byte step,
0.5/255 per channel in absolute linear units. This is accepted visual precision,
not a claim that every conceivable image remains perceptually indistinguishable.

Preparation uses the following fixed, reproducible approximations:

1. Gaia observed BP-RP uses the Jordi polynomial
   `log10(T) = 3.999 - 0.654*c + 0.709*c*c - 0.316*c*c*c` for `c <= 1.5`.
   For redder colours, use Gaia Sky's appearance continuation towards 3000 K at
   BP-RP=15, anchored at the polynomial's exact endpoint to avoid a discontinuity.
   The appearance input is bounded to [-0.5, 15] before polynomial evaluation.
2. Hipparcos observed B-V uses Ballesteros's expression
   `T = 4600 * (1/(0.92*c+1.7) + 1/(0.92*c+0.62))`.
   Bound the appearance input to [-0.4, 2] to avoid singular extrapolation.
   Every colour in the original 62-row supplement lay within these bounds;
   the expanded bright-range supplement uses the same documented clamping policy.
3. Convert the surrogate temperature, bounded to 1000--40000 K, with the original
   Helland photographic temperature-to-sRGB fit. Its result is display-encoded;
   decode the standard sRGB transfer curve before rounding `255 * linear`.

The bounds are appearance policies, not assertions about valid stellar physics.
Finite unusual indices retain a bounded derived colour instead of losing source
membership. Missing/nonfinite measurements remain rejected source rows. No
interstellar dereddening is applied: the surrogate follows observed colour and
is not an inferred intrinsic stellar effective temperature.

Jordi's original relation concerns unreddened colours and has a restricted
stellar regime. The red continuation is a visual heuristic, and the Helland fit
is explicitly a photographic approximation unsuitable for scientific colour
calibration. These limitations are accepted for M2. Spectra, absorption features,
precise photometric colour calibration and actual perceived night-vision colour
are not reconstructed by this model. More expensive preparation could replace
the model later without changing the RGB consumer boundary.

## Faint-star desaturation — 2026-10-31

The owner requested implementing the earlier greyward appearance direction.
The native viewer now reduces saturation for faint stars without modifying
catalogue RGB or magnitude. No catalogue rebuild or new control is required.
The initial implementation uses magnitude as a visibility proxy: full colour at
G-like magnitude 1 and brighter, a smooth transition over 1–5, then neutral grey
at 5 and fainter. The agent selected these initial internal endpoints for visual
review; they are not calibrated thresholds for human vision.

For magnitude `m`, let `t = clamp((m-1)/4, 0, 1)` and
`s = 1 - t*t*(3-2*t)`. Saturation is 1, 0.84375, 0.5, 0.15625 and 0 at
magnitudes 1, 2, 3, 4 and 5 respectively. For linear RGB light `L`, use
`Y = 0.2126*L.r + 0.7152*L.g + 0.0722*L.b` and
`result = Y + s*(L-Y)` per channel. These are the
[linear-sRGB relative-luminance coefficients](https://www.w3.org/TR/WCAG22/#dfn-relative-luminance),
not a rod-response or night-adaptation model.

Apply the mix after the existing RGB flicker gain and before the shared spatial
profile, light accumulation and display transfer. That also fades colour pulses
for faint stars while retaining their time-varying linear luminance; applying
it before RGB flicker would reintroduce colour afterwards. The weights sum to
one, so the mix preserves linear luminance. The existing maximum-channel tone
map can still change final displayed luminance as channel proportions change.
Catalogue magnitude, flicker generation, support footprint and 30 fps pacing
are unchanged.

The vertex shader uses existing inputs, adding a bounded amount of arithmetic
per quad vertex (four evaluations per source). It adds no per-star storage,
buffer transfer, fragment work, draw call or texture pass. This is an inspection
of the work added, not a measured performance claim. Later background light,
glare, working brightness and adaptation should supply a separately selected
visibility treatment; this baseline does not model those conditions, move culling
thresholds or reinterpret catalogue photometry.

Verification: all 226 C++ checks pass in ordinary and ASan/UBSan/leak-enabled
builds with no skips, including eight headless renderer cases. The documented
[test-only Mesa override](stellar-appearance-trial.md#headless-leak-check-workaround--2026-10-31)
remains in effect for the sanitized OpenGL cases. Tests cover the sampled fade
curve for red and blue sources, preserved linear luminance and hue differences,
continued achromatic faint-source flicker, CPU/shader agreement, freeze/resume
and the existing display transfer. Cppcheck, Lizard, formatting and configured
include checks pass. Independent dcodex review found no correctness defects;
native visual acceptance and performance measurements remain separate.

## References

- [Jordi et al., Gaia broad band photometry (equation 2)](https://doi.org/10.1051/0004-6361/201015441).
- [Ballesteros, New insights into black bodies (equation 14)](https://arxiv.org/html/1201.1809v2).
- [Gaia Sky source: separate BP-RP and B-V temperature conversions](https://codeberg.org/gaiasky/gaiasky-catgen/src/branch/master/src/color.rs).
- [Helland's original temperature-to-sRGB fit and limitations](https://tannerhelland.com/2012/09/18/convert-temperature-rgb-algorithm-code.html).
- [W3C colour conversion definitions](https://www.w3.org/TR/css-color-4/#color-conversion-code).
