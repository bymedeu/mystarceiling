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

The owner's [proposed perceived-colour treatment](ceiling-product-output.md#proposed-perceived-colour)
would reduce saturation toward grey as visibility decreases while preserving
original catalogue RGB. No visibility algorithm, threshold or implementation is
selected.

## References

- [Jordi et al., Gaia broad band photometry (equation 2)](https://doi.org/10.1051/0004-6361/201015441).
- [Ballesteros, New insights into black bodies (equation 14)](https://arxiv.org/html/1201.1809v2).
- [Gaia Sky source: separate BP-RP and B-V temperature conversions](https://codeberg.org/gaiasky/gaiasky-catgen/src/branch/master/src/color.rs).
- [Helland's original temperature-to-sRGB fit and limitations](https://tannerhelland.com/2012/09/18/convert-temperature-rgb-algorithm-code.html).
- [W3C colour conversion definitions](https://www.w3.org/TR/css-color-4/#color-conversion-code).
