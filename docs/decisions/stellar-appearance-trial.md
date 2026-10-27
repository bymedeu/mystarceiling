# Native stellar appearance trial

2026-10-26: the owner approved a provisional visual trial **inside the MSC
executable**. Numerical agreement is not visual acceptance. The owner accepted
the earlier catalogue colours and wants animated light with unequal tapered rays,
rather than only a circular halo. The image references describe a visual target;
their colours, particles and photographic processing are not reconstructed data.

## Run

```sh
./msc view --appearance-trial
```

Existing catalogue/configuration/logging options still apply. No source data or
binary rebuild is needed. Ordinary `msc view` remains the static viewer. The
trial does not change the strict TOML schema or rewrite saved settings.

On 2026-10-27 the owner requested removal of the extra appearance CLI options
and tuning shortcuts added during the trial.
The trial now uses its existing C++ model defaults. No animation/profile
coefficients were changed as part of the control removal. There are no public
appearance-tuning parameters, including window-size, seed or initial-picture
options. The window can still be resized normally.

| Key | Action |
| --- | --- |
| Space | Toggle picture mode, freezing/resuming the animation phase |
| Escape | Close the window |

The window title identifies animated or picture mode. Time pause and picture
mode are different: astronomical time pause keeps atmospheric animation running,
while picture mode freezes both. The fixed Toulouse/time geometry already has
paused astronomical time. No new astronomical clock, observer controls or
artificial sky drift is introduced. Time spent frozen is not accumulated on resume.

## Current visual limitations

The owner likes the sky appearance but reports that animation is not apparent
and small sources look pixelated. The trial is provisional, not visually accepted.
Ordinary `msc view` has no animation; the trial must be explicitly selected.
In the trial, the zenith-facing view attenuates modulation by design, and the
highlight-compression pass reduces visible bright-core variation further. Its
animation changes intensity/colour, without moving the profile or source position.
Pixel-footprint approximation and framebuffer sampling also limit the appearance
of small stars; screen resolution alone does not explain away these limitations.

The owner's [video reference](https://www.youtube.com/shorts/m34BUDkIoDM) is a
magnified telescope view. It is a qualitative motion/colour reference, not an
approved angular scale, diffraction model or numerical calibration. Improving
animation visibility and pixel sampling remains follow-up appearance work.

An offscreen check on 2026-10-27 linked the actual compiled renderer and used the
configured 4,096-source prefix at 1280x720 on Mesa llvmpipe. Advancing animation
from 0 to 0.25 seconds changed 17,585 final RGB8 pixels, with a maximum channel
difference of 11 levels. Picture mode ignored ten seconds of elapsed time and
preserved the image exactly; resuming changed pixels again. This verifies
variation in rendered output, not perceptual strength or native-window presentation.

## Model and boundaries

The catalogue retains double-precision magnitude and compact linear RGB8. The
application prepares local ENU directions and a deterministic animation key from
both halves of the native uint64 identity and its catalogue. Projected order or
screen position does not seed animation. Above-horizon sources remain available
outside the viewport, letting their expanded light footprints reach screen edges.
Projection uses the actual framebuffer aspect ratio and the existing 90-degree
vertical field of view.

The core reference model interpolates hashed samples with cubic smoothstep.
Slow and fast components drive brightness; a separately salted fast component
drives opposing red/blue variations and a smaller green change. Horizon strength
is `zenith_fraction + (1-zenith_fraction)*(1-sin(elevation))^2`, bounded without a
horizon singularity. Strengths below one keep channel multipliers positive.
This independent-source model does not reconstruct the spatial or temporal
spectrum of real atmospheric turbulence. Colour fluctuations change instantaneous
luminance as well as hue; only the catalogue baseline remains unchanged.

Split integer/fractional noise clocks avoid converting ever-growing elapsed
seconds directly to a shader float. The shader reproduces the core's integer
hash and interpolation; CPU/GPU floating results are compared with a numerical
tolerance, not promised bit-identical across graphics implementations.

The source is a point with no separately painted dot. Its spatial profile is a
weighted narrow Gaussian, wider Gaussian and seeded unequal one-sided Gaussian
rays. Ray orientation is common to the observer's trial profile. Profile weights
redistribute light; their fractions sum to at most one. Brightness variation makes
different parts of that distribution visible without a binary blinking state.

The profiles have unit continuous integrals before finite support cropping. A
`1/12` pixel-footprint variance is added to Gaussian axis variances to reduce
subpixel aliasing; this is an approximation, not exact pixel integration.
Finite support, sampling, display compression and RGB8 limit numerical/visual
accuracy. Very bright/faint display magnitudes are bounded to [-30,31] for upload;
sources above the supported limiting magnitude of 30 remain excluded. Original
catalogue magnitudes are unchanged. Profiles are not calibrated human-eye PSFs,
measured diffraction or a full atmospheric/vision model.

## Rendering and scaling

The trial uploads compact static instances with integer RGB and identity, then
evaluates modulation in the vertex shader. Instanced quads distribute linear
light additively into an RGBA32F framebuffer. A final pass compresses highlights
with a common RGB scale and encodes sRGB once. There is no per-star halo image,
per-frame source reupload or persistent expanded float RGB catalogue.

Cost depends on source count, covered pixels and ray count. The floating colour
attachment alone uses `width*height*16` bytes: about 14.1 MiB at 1280x720, excluding
the window framebuffer, driver storage and source buffers. Resize reallocates it.
No 500k-source frame-rate or memory benchmark is claimed by this visual trial.

Software-OpenGL checks compiled/linked the actual shaders, checked additive
overlap, off-screen contribution and output encoding, and matched GPU modulation
to the double-precision core within 1.61e-7 absolute error in the sampled cases.
An 8x8 subpixel-position sweep found 6.66% peak-to-peak integrated-flux variation
for the default profile and 26.55% with rays alone. Thus the pixel approximation
does not preserve sampled flux exactly; visual review and later rendering work
must account for this. These checks do not establish native-window appearance
acceptance or display calibration.

Zone-based contribution bounds and full/compact profiles with continuous sizes
are approved design direction, not implemented optimizations. They must preserve
combined faint-source light, flicker peaks, phase and contributions from outside
the view. Thresholds, zone dimensions, tier transitions and any extra subtiers
remain to be measured against this reference. A future Moon can share light
accumulation/spreading, while retaining an extended shape and different animation
response. Clouds and photographic modes are optional later work, not dependencies.

## References

- [NRAO: scintillation and elevation](https://www.gb.nrao.edu/GBTopsdocs/primer/scintillation.htm).
- [NASA: stars that change colours](https://cosmicopia.gsfc.nasa.gov/qa_star.html).
- [Psychophysical study of the optical origin of starbursts](https://doi.org/10.1364/JOSAA.36.000B97): perceived rays depend on the observer's optics.
- [Khronos: linear framebuffer blending and sRGB conversion](https://wikis.khronos.org/opengl/Framebuffer).
