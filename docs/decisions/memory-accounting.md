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

Every cell below is `count × sizeof(T)` bytes, followed by MiB/GiB (binary units).
It assumes exactly N elements; it excludes vector metadata, spare capacity,
allocator overhead and every other simultaneous allocation. The binary column
uses the current version-2 **74-byte** serialized record, not in-memory padding;
add its **32-byte header** for the full uncompressed file size.

| Source count | Star bytes (MiB / GiB) | CatalogueStar bytes (MiB / GiB) | ScreenPoint bytes (MiB / GiB) | Binary record bytes (MiB / GiB) |
| ---: | ---: | ---: | ---: | ---: |
| 4,096 | 294,912 (0.28125 MiB / 0.000274658 GiB) | 360,448 (0.34375 MiB / 0.000335693 GiB) | 49,152 (0.046875 MiB / 4.57764e-05 GiB) | 303,104 (0.289062 MiB / 0.000282288 GiB) |
| 262,144 | 18,874,368 (18 MiB / 0.0175781 GiB) | 23,068,672 (22 MiB / 0.0214844 GiB) | 3,145,728 (3 MiB / 0.00292969 GiB) | 19,398,656 (18.5 MiB / 0.0180664 GiB) |
| 1,048,576 | 75,497,472 (72 MiB / 0.0703125 GiB) | 92,274,688 (88 MiB / 0.0859375 GiB) | 12,582,912 (12 MiB / 0.0117188 GiB) | 77,594,624 (74 MiB / 0.0722656 GiB) |
| 134,217,728 | 9,663,676,416 (9216 MiB / 9 GiB) | 11,811,160,064 (11264 MiB / 11 GiB) | 1,610,612,736 (1536 MiB / 1.5 GiB) | 9,932,111,872 (9472 MiB / 9.25 GiB) |

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
