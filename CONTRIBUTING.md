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

`all` runs format, tidy, complexity, cppcheck, and the warning-free build/tests,
reporting each failure. Each name is also an individual command. Inventory comes
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

Hooks are tracked but **not activated**. The approved quality cleanup passes the
warning-free build (119 C++ tests), 30 offline acquisition tests, 17 tooling tests,
Lizard, cppcheck, pinned formatting and local tidy. The catalogue understanding
checkpoint remains open. Hook activation still requires owner review. Later,
`python tools/quality/hooks.py install` installs the tracked hook path only when
no custom hooks/configuration exist. It refuses to
overwrite them. The push hook blocks every destination `refs/heads/main`, including
`HEAD:main` and deletion; checks every outgoing nondeleted tip in a temporary
snapshot; and never downloads dependencies. New branches need a local
`origin/main`. Set `MSC_DEPENDENCY_ROOT` to an absolute prepared `_deps` directory
if different from `build/_deps`. Hooks are bypassable; they are not remote protection.

CI runs only for PRs targeting `main`, with one sequential required check named
`quality`: real-head Conventional Commits and PR title policy, warning-free
build/tests, Lizard, cppcheck, then final `clang-format --dry-run --Werror`.
Each failure fails the job and skips later steps. Build/analyzers check GitHub's
proposed merge tree; policy reads actual head/base history from the event, never
the synthetic merge commit. PR title edits rerun the job; stale PR runs cancel.
Clang-tidy is local only. The owner formats locally before committing; CI makes
no automatic source changes and has read-only contents permission.

The single GCC **16.2.0** Debian environment uses distribution-packaged
`python3`/`python3-venv`, the shared `requirements.txt`, pinned Node/actions and
analyzer versions (LLVM format **23.1.1**, Lizard **1.24.2**, cppcheck **2.22.0**).
Native packages remain distribution-managed. Jinja2 is pinned for GLAD.
The tracked `.clang-format` and `.clang-tidy` are real copies from
[bymedeu/config at ace2ad8](https://github.com/bymedeu/config/tree/ace2ad8e0d57e2cf6ea4beeb69a168d94df14469/clang);
updates are deliberate, with no automatic cross-repository feed.

Remote `main` policy requires PRs with zero required approving reviews and the
strict/up-to-date `quality` check from GitHub Actions, including for admins,
with no bypass, force pushes or deletion. Normal merges remain supported with
`PR_TITLE` as the default merge commit title. Conventional titles are required;
self-approval and squash-only merges are not required. Any new gate failure
remains blocking. No history rewriting.
