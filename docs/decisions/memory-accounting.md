# Current record-size accounting

Measured 2026-10-28 after promoting the animated viewer, GCC 16.2.1 on the local
Linux target. A temporary program included the public Star, CatalogueStar and
appearance headers and printed `sizeof` and `alignof`; it allocated no catalogue.
These are ABI/compiler-dependent sizes, not measured peak RSS or portable wire layout.

| Type (under `msc::`) | sizeof, bytes | alignof, bytes |
| --- | ---: | ---: |
| core::astronomy::LinearRgb8 | 3 | 1 |
| core::astronomy::Star | 64 | 8 |
| catalogue::CatalogueStar | 80 | 8 |
| render::TrialStar | 48 | 8 |

TrialStar contains double-precision ENU direction and magnitude, RGB8 and an
integer animation key. RGB remains compact in the catalogue and scene, with no
persistent expanded float colour copy. The static ScreenPoint type and renderer
have been removed. Historical Star/CatalogueStar/ScreenPoint measurements were
72/88/12 bytes on 2026-10-18 and 64/80/16 bytes on 2026-10-25.

## One-vector arithmetic, not peak memory

Each cell is `count * sizeof(T)` in binary units, assuming exactly N elements.
It excludes vector metadata, spare capacity, allocator overhead and simultaneous
allocations. Version-3 binary records use **68 bytes**, plus a **32-byte header**
per full file, independently of in-memory padding.

| Sources | Star | CatalogueStar | TrialStar | Binary records |
| ---: | ---: | ---: | ---: | ---: |
| 4,096 | 0.25 MiB | 0.3125 MiB | 0.1875 MiB | 0.265625 MiB |
| 262,144 | 16 MiB | 20 MiB | 12 MiB | 17 MiB |
| 1,048,576 | 64 MiB | 80 MiB | 48 MiB | 68 MiB |
| 134,217,728 | 8 GiB | 10 GiB | 6 GiB | 8.5 GiB |

At 134,217,728 stars, RGB8 alone occupies 384 MiB. A complete version-3 binary
would be 9,126,805,536 bytes including its header. These are arithmetic examples,
not evidence of full-catalogue loading or rendering. The acquired gzip CSV archive
is 8,575,699,250 bytes; compression/source columns differ from binary records,
so archive size does not describe runtime RAM.

## Current lifetimes and extra memory

Building reads Hipparcos first, then only the remaining accepted Gaia count.
The Gaia input vector and combined selection briefly coexist during insertion;
stable sorting can allocate workspace and erasing records need not shrink capacity.
The selected vector remains alive while completed-output verification reads a
second full selected-count vector. These allocations end with the build command.

Viewing loads the requested binary prefix into CatalogueStar records.
`make_m1_scene` requests TrialStar capacity for the full loaded count and keeps
above-horizon sources, including faint and off-viewport sources for renderer
selection and edge footprints. Catalogue and scene coexist during construction;
`prepare_view` releases the loaded catalogue before returning the scene for
rendering. The scene stays alive throughout the window's lifetime.

Renderer upload briefly creates a separate CPU instance vector containing float
ENU/magnitude, RGB8 and integer identity keys, then copies it to a GPU buffer.
That temporary vector is destroyed after upload; the scene and GPU buffer remain.
The RGBA32F light attachment adds `width*height*16` bytes, about 14.1 MiB at
1280x720, and is reallocated on resize. Frame preparation uses 24 shared
three-float frequency records (288 bytes), without per-source animation history.
Exact capacities, peak allocations, other framebuffer/driver overhead and RSS
have not been measured here. See the [viewer model](stellar-appearance-trial.md).

Framebuffer storage scales with pixel count; source records scale with catalogue
count and simultaneous representations. Neither acquisition nor this arithmetic
establishes that a 16 GB laptop can display the full catalogue. M4 still requires
measured million-source CPU and memory evidence. Out-of-core processing remains
later conditional scope.

The owner accepted compact prepared colour with source CSVs retained offline.
See [colour assumptions](stellar-colour.md),
[prepared records](catalogue-integration.md#current-prepared-catalogue--2026-10-25)
and the [roadmap](../../README.md#milestones).
