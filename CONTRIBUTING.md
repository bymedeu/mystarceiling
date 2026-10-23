# Contributing

Use a topic branch (the owner prefers `dcodex`), Conventional Commits such as
`chore: add quality checks`, and a PR with a conventional title. Review code,
tests, documentation, ownership, contracts, numerical assumptions and intent;
apply the C++ Core Guidelines in that scope. Keep code readable: do not compress
lines, split without a coherent responsibility, suppress findings, or exclude
legacy code to satisfy metrics. Nontrivial refactors still need the open
[owner understanding gate](README.md#algorithmic-ownership-gate).

Local setup (explicit downloads only; no global installs): Python **3**
(Debian-packaged Python is supported), Node **26.10.0**, npm **12.2.0**, GCC **16.2.x**, clang-tidy **23.1.1** on PATH.
Native prerequisites: GTest, spdlog/fmt, zlib, OpenGL, X11 development libraries
(Xrandr, Xinerama, Xcursor, Xi), Wayland, xkbcommon and wayland-protocols.

```sh
python3 -m venv .quality-venv
.quality-venv/bin/pip install -r tools/quality/requirements.txt
npm ci --prefix tools/quality --ignore-scripts
export PATH="$PWD/.quality-venv/bin:$PATH"
git clone https://github.com/cppcheck-opensource/cppcheck.git /tmp/msc-cppcheck
git -C /tmp/msc-cppcheck checkout --detach a436ca35ed1887bee789765122b65ed2d7a7e045
make -C /tmp/msc-cppcheck -j2 CXXFLAGS=-O2
export PATH="/tmp/msc-cppcheck:$PATH"
```

Prepare the existing pinned GLFW/GLAD sources explicitly before offline checks:

```sh
cmake -S . -B build -DPython_EXECUTABLE="$PWD/.quality-venv/bin/python"
python tools/quality/check.py all
python -m unittest discover -s tools/quality -p 'test_*.py'
python tools/quality/check.py commits --base origin/main --head HEAD
printf '%s\n' 'chore: example title' | python tools/quality/check.py message
```

`all` runs format, tidy, complexity, cppcheck, the warning-free build/tests and
sanitizer build/tests, reporting each failure. Each name is also an individual
command. Inventory comes
from Git: C/C++ headers, implementations and tests; Python participates in Lizard.
Only generated/vendor/build paths are excluded. Lizard enforces **physical function
length <=40** (including comments/blanks), **CCN <=10**; NLOC is a different metric.
Tidy builds only the generated GLAD library prerequisite before analyzing all
first-party units. Its database leaves GCC warnings-as-errors off; the build gate
requires them on. Tidy follows the unchanged include-cleaner policy, not every
Core Guideline. Cppcheck 2.22.0
requests C++26; its parser is not a full compiler or reflection validator. Unknown
syntax remains a failure. Each tracked first-party header is analyzed through a
temporary C++ file that includes its actual path, alongside all real source/test
units. This supplies include context without dropping headers or suppressing
diagnostics; a real-analyzer regression verifies included defects still fail.
GCC compilation remains essential. New cppcheck findings require owner review;
analyzer output is not proof of a defect.

`python tools/quality/check.py sanitizer-test` builds every first-party C++ target
with ASan+UBSan, debug symbols and frame pointers, then runs the C++ suite through
CTest. Findings are fatal; leak detection stays enabled. Both CTest gates reject
an empty suite. `build-test` also runs the offline Python acquisition/tooling tests.
The default CMake build is unsanitized; opt in with `-DMSC_ENABLE_SANITIZERS=ON`
(GCC or Clang required; the project's C++26/reflection requirements still apply).
Vendor targets receive neither sanitizer nor project warning-as-error flags.
The independent ordinary gate retains `-Werror`; sanitizer builds omit it because
[GCC documents sanitizer-related warning false positives](https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html).
Use separate caches: `--build` defaults to `build/quality`, while
`--sanitizer-build` defaults to `build/quality-sanitizers`; equal paths are rejected.
Each command explicitly sets sanitizers on or off. Sanitized test executables and
compile commands stay build-local, preserving normal `bin/` and editor output.
`all` retains the full suite for explicit local use. Hooks run the narrower
checks below; the ordinary and sanitizer CI gates remain required.

Shared hooks are maintained in [bymedeu/config](https://github.com/bymedeu/config/tree/main/git).
Follow its `git/README.md` for explicit pinned tool setup, global activation,
rollback and per-repository overrides. Global `core.hooksPath` makes them the
default for **all repositories**, while local overrides retain native Git behavior.
Existing native hooks are chained; cloned `.githooks` scripts are not implicitly
trusted. MSC opts into its policy and tracked declarative profile with:

```sh
python tools/quality/styles.py local
python tools/quality/hooks.py install
git config --show-origin --get core.hooksPath
```

Pre-commit uses the pre-commit framework to run clang-format on staged C/C++
(including headers/tests) and Ruff format on Python. It preserves unstaged work;
a formatting change fails the commit for review and deliberate re-staging.
Pre-push retains Conventional Commit validation and blocks every destination
`refs/heads/main`, including `HEAD:main` and deletion. Every nondeleted outgoing
tip is checked in an immutable snapshot with MSC's full tracked cppcheck/Lizard
inventory and project flags (`check.py push`), plus Ruff check from the declarative
profile. The initial Python lint policy in `ruff.toml` covers syntax, imports and
undefined names; no semantic fixes run. No builds, tidy, sanitizer runs, tool
installs or network requests occur from hooks. New branches need local
`origin/main`. Existing `all` and individual commands remain available explicitly.
Hooks are local checks, not remote protection. The catalogue understanding
checkpoint remains open.

CI runs only for PRs targeting `main`, with one sequential required check named
`quality`: real-head Conventional Commits and PR title policy, warning-free
build/tests, separate ASan+UBSan C++ build/tests, Lizard, cppcheck, then final
`clang-format --dry-run --Werror`.
Each failure fails the job and skips later steps. Build/analyzers check GitHub's
proposed merge tree; policy reads actual head/base history from the event, never
the synthetic merge commit. PR title edits rerun the job; stale PR runs cancel.
Clang-tidy is local only. The owner formats locally before committing; CI makes
no automatic source changes and has read-only contents permission.

The single GCC **16.2.0** Debian environment uses distribution-packaged
`python3`/`python3-venv`, the shared `requirements.txt`, pinned Node/actions and
analyzer versions (LLVM format **23.1.1**, Lizard **1.24.2**, cppcheck **2.22.0**).
Native packages remain distribution-managed. Jinja2 is pinned for GLAD.
The tracked `.clang-format`, `.clang-tidy` and existing `.clangd` are portable links through
ignored `.quality-style/`. Local setup links that directory to
`${XDG_CONFIG_HOME:-$HOME/.config}/clang`, so editors and local checks see the live
central files. Existing conflicting local material is preserved. No independent
MSC style copy is maintained. Push snapshots materialize the central styles (including archived `.clangd` links)
offline, without following machine-specific links from archived trees.

CI explicitly resolves the config repository's default-branch `HEAD` **once** per
run, logs that revision, and downloads both styles from that exact revision into
`.quality-style/`. Changes to config do not trigger MSC CI or update PRs. The next
normal MSC PR run sees the latest style. The pinned clang-format version remains
23.1.1; unsupported style options fail the format gate. Clang-tidy remains local.

Remote `main` policy requires PRs with zero required approving reviews and the
strict/up-to-date `quality` check from GitHub Actions, including for admins,
with no bypass, force pushes or deletion. Normal merges remain supported with
`PR_TITLE` as the default merge commit title. Conventional titles are required;
self-approval and squash-only merges are not required. Any new gate failure
remains blocking. No history rewriting.
