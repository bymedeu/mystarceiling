# Current record-size accounting

Measured 2026-10-25 with the RGB8/J2016 implementation, GCC 16.2.1 on the local
Linux target. A temporary program included the public Star, CatalogueStar and
ScreenPoint headers and printed `sizeof` and `alignof`; it allocated no catalogue.
These are ABI/compiler-dependent sizes, not a measured peak RSS or portable wire layout.

| Type (under `msc::`) | sizeof, bytes | alignof, bytes |
| --- | ---: | ---: |
| core::astronomy::LinearRgb8 | 3 | 1 |
| core::astronomy::Star | 64 | 8 |
| catalogue::CatalogueStar | 80 | 8 |
| render::ScreenPoint | 16 | 4 |

The earlier 2026-10-18 Star/CatalogueStar/ScreenPoint sizes were 72/88/12 bytes.
RGB now occupies three bytes without a persistent float colour copy. ScreenPoint
contains two float coordinates, one float linear intensity and RGB8, padded to
16 bytes; intensity is at offset 8 and colour at offset 12. OpenGL consumes the
RGB bytes as normalized unsigned-byte attributes.

## One-vector arithmetic, not peak memory

Each cell is `count * sizeof(T)` in binary units, assuming exactly N elements.
It excludes vector metadata, spare capacity, allocator overhead and simultaneous
allocations. Version-3 binary records use **68 bytes**, plus a **32-byte header**
per full file, independently of in-memory padding.

| Sources | Star | CatalogueStar | ScreenPoint | Binary records |
| ---: | ---: | ---: | ---: | ---: |
| 4,096 | 0.25 MiB | 0.3125 MiB | 0.0625 MiB | 0.265625 MiB |
| 262,144 | 16 MiB | 20 MiB | 4 MiB | 17 MiB |
| 1,048,576 | 64 MiB | 80 MiB | 16 MiB | 68 MiB |
| 134,217,728 | 8 GiB | 10 GiB | 2 GiB | 8.5 GiB |

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

Viewing independently loads the requested binary prefix. Scene construction
reserves ScreenPoint capacity equal to the loaded count, even when visibility
and magnitude filtering leave fewer points. The catalogue and scene coexist;
the renderer additionally uploads the live point bytes into a GPU buffer.
No persistent expanded float RGB array is introduced. Exact vector capacities,
peak allocations, GPU/driver overhead and RSS have not been measured here.

A framebuffer scales with pixel count, bytes per pixel and buffer count;
source records scale with catalogue count and simultaneous representations.
Neither acquisition nor record-size arithmetic establishes that a 16 GB laptop
can display the full catalogue. M4 still requires measured million-source CPU
and memory evidence. Out-of-core processing remains later conditional scope.

The owner accepted compact prepared colour with source CSVs retained offline.
See [colour assumptions](stellar-colour.md),
[prepared records](catalogue-integration.md#current-prepared-catalogue--2026-10-25)
and the [roadmap](../../README.md#milestones).
