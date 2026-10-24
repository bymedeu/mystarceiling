# Contributing

Use a topic branch, Conventional Commits and a PR with a conventional title.
Review contracts, ownership, numerical assumptions and intent. Do not compress
code, suppress findings or exclude legacy code to satisfy metrics. The
[owner understanding checkpoint](README.md#algorithmic-ownership-gate) stays open.

Use GCC 16.2.x with C++26, CMake 4.4.4, clang-format 23.1.1, Lizard 1.24.2 and
cppcheck 2.22.0 (official revision `a436ca35ed1887bee789765122b65ed2d7a7e045`).
Conan 2.33.0 manages CLI11 2.7.2, toml++ 3.4.0, GTest 1.18.0,
zlib 1.3.2, spdlog 1.17.0 and its required fmt 12.1.0. The manifest and
`conan.lock` pin versions and recipe revisions. `profiles/gcc16` explicitly uses
Linux x86_64, GCC 16, GNU C++26, `libstdc++11` and Debug, for both host and build.
Use the matching GCC installation on PATH; first-party reflection retains
`-freflection`. Do not substitute libc++ for GCC's libstdc++ ABI setting.

Keep system OpenGL and window-system development prerequisites: X11 (Xrandr,
Xinerama, Xcursor, Xi), Wayland, xkbcommon and wayland-protocols, plus pkg-config.
Two pinned FetchContent exceptions preserve the renderer: GLFW 3.5.1
(`d9d6f0f1f967807ffade6598ea9a631ebaf37a56`) is newer than the
[available ConanCenter recipe](https://github.com/conan-io/conan-center-index/blob/master/recipes/glfw/config.yml);
GLAD 2.0.8 (`73db193f853e2ee079bf3ca8a64aa2eaf6459043`) keeps reproducible
GL 3.3 core, no extensions and no loader/debug wrappers. Its
[Conan recipe](https://github.com/conan-io/conan-center-index/blob/master/recipes/glad/2.x/conanfile.py)
forces debug wrappers in Debug builds. GLFW retains both X11 and Wayland.
GLAD needs Python with Jinja2 3.1.6 and MarkupSafe 3.0.3.
CLI11 handles parsing; toml++ handles strict launch configuration through its
packaged public single header. Vendor tests/examples remain off.

Install tools into a venv, then use direct Conan/CMake commands (no global pip):

```sh
python3 -m venv .quality-venv
. .quality-venv/bin/activate
pip install conan==2.33.0 cmake==4.4.4 clang-format==23.1.1 lizard==1.24.2 Jinja2==3.1.6 MarkupSafe==3.0.3 pathspec==1.1.1 Pygments==2.21.0
export CONAN_HOME="$PWD/.conan2"
conan install . -pr:h ./profiles/gcc16 -pr:b ./profiles/gcc16 -of build/conan --build=missing
cmake -S . -B build/quality -DCMAKE_TOOLCHAIN_FILE="$PWD/build/conan/conan_toolchain.cmake" -DCMAKE_BUILD_TYPE=Debug -DMSC_WARNINGS_AS_ERRORS=ON -DMSC_ENABLE_SANITIZERS=OFF -DMSC_ENABLE_TESTS=ON -DPython_EXECUTABLE="$VIRTUAL_ENV/bin/python"
cmake --build build/quality --parallel 2
ctest --test-dir build/quality --output-on-failure --no-tests=error
python -m unittest discover -s tools -p 'test_*.py'
cmake -S . -B build/quality-sanitizers -DCMAKE_TOOLCHAIN_FILE="$PWD/build/conan/conan_toolchain.cmake" -DCMAKE_BUILD_TYPE=Debug -DMSC_ENABLE_SANITIZERS=ON -DMSC_WARNINGS_AS_ERRORS=OFF -DMSC_ENABLE_TESTS=ON -DPython_EXECUTABLE="$VIRTUAL_ENV/bin/python"
cmake --build build/quality-sanitizers --parallel 2
ASAN_OPTIONS=detect_leaks=1:halt_on_error=1 UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1 ctest --test-dir build/quality-sanitizers --output-on-failure --no-tests=error
```

Conan automatically consumes `conan.lock`. Refresh deliberately with
`conan lock create . -pr:h ./profiles/gcc16 -pr:b ./profiles/gcc16 --lockfile="" --lockfile-out=conan.lock`,
then review revisions and rebuild. For an existing system-package build, clear
its old package lookup cache once with `cmake --fresh` and the same configuration
arguments. The ordinary `build` directory can use the same toolchain/settings as
`build/quality`.

Offline, retain `.conan2` and rerun the install with `--no-remote --build=never`.
Cache both pinned renderer source checkouts too; configure with
`-DFETCHCONTENT_SOURCE_DIR_GLFW=/absolute/cache/glfw-src`
`-DFETCHCONTENT_SOURCE_DIR_GLAD=/absolute/cache/glad-src`
`-DFETCHCONTENT_FULLY_DISCONNECTED=ON`. This also works with a fresh CMake cache;
Conan's binary cache alone does not contain these two source exceptions.
See the [standard Conan flow](https://docs.conan.io/2/tutorial/consuming_packages/build_simple_cmake_project.html).

Use separate ordinary and sanitized build directories. Sanitizers apply to all
first-party C++ targets; findings are fatal. The ordinary build requires no
warnings. GCC sanitizer warning false positives are handled by keeping `-Werror`
in the ordinary build only. Vendor targets receive neither project policy.

Analyze source/test files, including untracked additions, directly:

```sh
git ls-files --cached --others --exclude-standard -z -- '*.c' '*.cc' '*.cpp' '*.cxx' '*.h' '*.hh' '*.hpp' '*.hxx' '*.py' | xargs -0 -r lizard -L40 -C10
git ls-files --cached --others --exclude-standard -z -- '*.c' '*.cc' '*.cpp' '*.cxx' | xargs -0 -r cppcheck --std=c++26 --language=c++ --enable=warning,style,performance,portability --error-exitcode=1 -Iinclude -Isrc
git ls-files --cached --others --exclude-standard -z -- '*.c' '*.cc' '*.cpp' '*.cxx' '*.h' '*.hh' '*.hpp' '*.hxx' | xargs -0 -r clang-format --dry-run --Werror
```

Lizard limits physical function length (including comments/blanks) to 40 and CCN
to 10, including Python. Cppcheck analyzes source and test translation units with
project includes; included headers get their real usage context. It does not
replace GCC compilation. Clang-tidy remains local: use `clang-tidy <source.cpp>
-p build/quality --warnings-as-errors='*'` for each first-party translation unit.

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
