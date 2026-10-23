# Contributing

Use a topic branch, Conventional Commits and a PR with a conventional title.
Review contracts, ownership, numerical assumptions and intent. Do not compress
code, suppress findings or exclude legacy code to satisfy metrics. The
[owner understanding checkpoint](README.md#algorithmic-ownership-gate) stays open.

Use GCC 16.2.x with C++26, CMake 4.4.4, clang-format 23.1.1, Lizard 1.24.2 and
cppcheck 2.22.0 (official revision `a436ca35ed1887bee789765122b65ed2d7a7e045`).
Native prerequisites are GTest, spdlog/fmt, zlib, OpenGL, X11 development libraries
(Xrandr, Xinerama, Xcursor, Xi), Wayland, xkbcommon and wayland-protocols.
GLAD requires Python 3 with Jinja2 3.1.6 and MarkupSafe 3.0.3. CMake fetches the
pinned GLFW/GLAD sources on initial configuration; for offline work, reuse those
sources with `FETCHCONTENT_SOURCE_DIR_GLFW`, `FETCHCONTENT_SOURCE_DIR_GLAD` and
`FETCHCONTENT_FULLY_DISCONNECTED=ON`.

Run the build and offline acquisition tests directly:

```sh
cmake -S . -B build/ci -DMSC_WARNINGS_AS_ERRORS=ON -DMSC_ENABLE_SANITIZERS=OFF -DMSC_ENABLE_TESTS=ON
cmake --build build/ci --parallel 2
ctest --test-dir build/ci --output-on-failure --no-tests=error
python3 -m unittest discover -s tools -p 'test_*.py'
cmake -S . -B build/ci-sanitizers -DMSC_ENABLE_SANITIZERS=ON -DMSC_WARNINGS_AS_ERRORS=OFF -DMSC_ENABLE_TESTS=ON
cmake --build build/ci-sanitizers --parallel 2
ASAN_OPTIONS=detect_leaks=1:halt_on_error=1 UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1 ctest --test-dir build/ci-sanitizers --output-on-failure --no-tests=error
```

Use separate ordinary and sanitized build directories. Sanitizers apply to all
first-party C++ targets; findings are fatal. The ordinary build requires no
warnings. GCC sanitizer warning false positives are handled by keeping `-Werror`
in the ordinary build only. Vendor targets receive neither project policy.

Analyze the tracked source/test inventory directly:

```sh
git ls-files -z -- '*.c' '*.cc' '*.cpp' '*.cxx' '*.h' '*.hh' '*.hpp' '*.hxx' '*.py' | xargs -0 -r lizard -L40 -C10
git ls-files -z -- '*.c' '*.cc' '*.cpp' '*.cxx' | xargs -0 -r cppcheck --std=c++26 --language=c++ --enable=warning,style,performance,portability --error-exitcode=1 -Iinclude -Isrc
git ls-files -z -- '*.c' '*.cc' '*.cpp' '*.cxx' '*.h' '*.hh' '*.hpp' '*.hxx' | xargs -0 -r clang-format --dry-run --Werror
```

Lizard limits physical function length (including comments/blanks) to 40 and CCN
to 10, including Python. Cppcheck analyzes source and test translation units with
project includes; included headers get their real usage context. It does not
replace GCC compilation. Clang-tidy remains local: use `clang-tidy <source.cpp>
-p build/ci --warnings-as-errors='*'` for each first-party translation unit.

Use the [shared language hooks](https://github.com/bymedeu/config/tree/main/git)
and their documented explicit tool installation and global activation. MSC needs
no project hook adapter. Shared defaults select languages: clang-format and Ruff
format before commits, cppcheck, Lizard and Ruff check before pushes. Formatting
preserves unstaged work; review formatter changes and re-stage deliberately.
Hooks run no project build or sanitizer suite. Native hooks remain chained.
Check activation with `git config --show-origin --get core.hooksPath`.

For MSC's include context, copy the shared default YAML to a local profile outside
this repository and select it with `git config --local quality.config /path/to/msc.yaml`.
Set cppcheck's entry to the direct command above and its `files` to
`\.(c|cc|cpp|cxx)$`; headers are analyzed through their source/test consumers.
Use `ruff check --isolated --select E4,E7,E9,F` for syntax, imports and undefined
names, and `ruff format --isolated`. Keep the default Lizard and clang-format
entries. No MSC adapter or Python policy file is required.

The portable `.clang*` links use an ignored local cache. Prepare it explicitly:

```sh
ln -s "${XDG_CONFIG_HOME:-$HOME/.config}/clang" .quality-style
git config --show-origin --get core.hooksPath
```

If the cache already exists, inspect it before changing it. CI checks out the
central config default branch once, logs that exact revision and copies its Clang
files into the cache. The next PR run sees updated central styles.

PRs targeting `main` run one sequential required `quality` job: standard
Conventional Commit and PR-title actions, warning-free build/CTest and offline
Python tests, ASan/UBSan build/CTest, Lizard, cppcheck and final clang-format.
Builds and analyzers check the proposed merge tree; commit policy checks PR
commits. CI has no source write permission. Remote main protection requires PRs
and the strict/up-to-date check, including admins; no bypass or history rewriting.
