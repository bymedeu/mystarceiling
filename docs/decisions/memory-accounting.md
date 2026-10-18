# Current record-size accounting

Measured 2026-10-18 using current headers at MSC commit `44c2727`, GCC 16.2.1
on the local Linux target. MODE GREEN: a tiny temporary C++ program in `/tmp`
printed `sizeof` and `alignof`; no catalogue was allocated/read/rendered.

```sh
g++ -std=c++26 -freflection -I /home/amade/projects/portfolio/msc/include /tmp/msc-record-sizes.cc -o /tmp/msc-record-sizes
/tmp/msc-record-sizes
```

The program included `core/astronomy/star.hh`, `catalogue/catalogue_star.hh` and
`render/opengl_renderer.hh` under `mystarceiling/`, then printed `sizeof(T)` and
`alignof(T)` for the three types. Results are ABI/compiler dependent, not a
chosen future layout or portable wire contract.

| Type (under `msc::`) | sizeof, bytes | alignof, bytes |
| --- | ---: | ---: |
| core::astronomy::Star | 72 | 8 |
| catalogue::CatalogueStar | 88 | 8 |
| render::ScreenPoint | 12 | 4 |

## One-vector arithmetic, not peak memory

Each cell is `count × sizeof(T)` in binary units, assuming exactly N elements.
It excludes vector metadata, spare capacity, allocator overhead and simultaneous
allocations. Version-2 binary records use **74 bytes**, plus a **32-byte header**
per full file, independently of in-memory padding.

| Sources | Star | CatalogueStar | ScreenPoint | Binary records |
| ---: | ---: | ---: | ---: | ---: |
| 4,096 | 0.28125 MiB | 0.34375 MiB | 0.046875 MiB | 0.2890625 MiB |
| 262,144 | 18 MiB | 22 MiB | 3 MiB | 18.5 MiB |
| 1,048,576 | 72 MiB | 88 MiB | 12 MiB | 74 MiB |
| 134,217,728 | 9 GiB | 11 GiB | 1.5 GiB | 9.25 GiB |

The completed gzip CSV archive is **8,575,699,250 bytes** (7.986742 GiB,
8.575699250 decimal GB). Compression and CSV contents differ from binary
schema 2: the archive size is not runtime RAM. At 134,217,728 sources, a complete
version-2 binary file would be **9,932,111,904 bytes** including the header.
Completed acquisition does not establish full-size loading or rendering.

## Current lifetimes and extra memory

Inspection of `src/cli/main.cc` shows parsed Gaia and Hipparcos vectors retained
in their results. `auto combined = parsed->stars` copies Gaia records, then
inserts the supplement. `select_brightest(std::move(combined), 4096)` transfers
that vector into the by-value selector, which stable-sorts and erases the tail.
Erasing elements does not necessarily shrink capacity; stable sort may allocate
workspace. `selected` remains alive while the written binary is read into another
`stars` vector, and both remain in scope through scene construction/rendering.
The moved-from vector's state must not be counted as another full copy.

`make_m1_scene` reserves ScreenPoint capacity equal to the input count even though
visibility and magnitude filtering yield fewer live points. The renderer uploads
live point bytes into an OpenGL buffer. These source, selected, reloaded and scene
vectors overlap in lifetime. Exact capacities and allocations were not measured.
Blindly extending this demonstration round trip to the entire archive is not a
bounded-memory design. No lifecycle change is authorized here.

One-vector arithmetic excludes simultaneous vectors, sort workspace, GPU buffers,
framebuffer, driver/allocator overhead, OS and other programs. It is **size
accounting, not measured peak RSS or a performance benchmark**. A 16 GB laptop
cannot devote its whole memory to an 11 GiB catalogue vector.

An image framebuffer scales with **pixel count × bytes per pixel × buffer count**,
plus depth/MSAA/driver choices. Catalogue memory scales with **source count ×
record size and simultaneous representations**. For illustration only, one
1920×1080 RGBA8 colour buffer is 8,294,400 bytes; this does not describe actual
framebuffer allocation or catalogue memory.

## Three distinct planning claims

- Workload-size controls choose a magnitude limit or proposed optional count cap;
  they reduce selected work but do not prove efficient scaling.
- M4 requires an actually measured million-source CPU pipeline and bounded-memory
  preparation baseline: record loading/update/projection/drawing and memory on
  named hardware. The arithmetic here does not satisfy that milestone.
- Out-of-core processing keeps a bounded working set while handling data larger
  than practical RAM, with disk/decompression/I/O costs. It belongs to conditional
  large-catalogue runtime scope. No chunk, mapping, streaming, batch layout, RGBE
  encoding or schema is selected by this note.

Source data may remain reproducible on disk while a compact prepared appearance
omits unused hot-memory fields. Removing photometry from canonical Star or the
binary schema remains an unapproved RED decision. See
[appearance/output boundaries](ceiling-product-output.md) and
[roadmap](../../README.md#milestones).
