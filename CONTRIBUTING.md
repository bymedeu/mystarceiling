# Contributing

Use a topic branch (the owner prefers `dcodex`), Conventional Commits such as
`chore: add quality checks`, and a PR with a conventional title. Review code,
tests, documentation, ownership, contracts, numerical assumptions and intent;
apply the C++ Core Guidelines in that scope. Keep code readable: do not compress
lines, split without a coherent responsibility, suppress findings, or exclude
legacy code to satisfy metrics. Nontrivial refactors still need the open
[owner understanding gate](README.md#algorithmic-ownership-gate).

Local setup (explicit downloads only; no global installs): Python **3.14.7**,
Node **26.10.0**, npm **12.2.0**, GCC **16.2.x**, clang-tidy **23.1.1** on PATH.
Native prerequisites: GTest, spdlog/fmt, zlib, OpenGL, X11 development libraries
(Xrandr, Xinerama, Xcursor, Xi), Wayland, xkbcommon and wayland-protocols.

```sh
python3.14 -m venv .quality-venv
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
syntax remains a failure. GCC compilation remains essential. New cppcheck
findings require owner review; analyzer output is not proof of a defect.

Hooks are tracked but **not activated**: the baseline has 24 formatting files,
18 over-limit functions and 10 tidy errors. Do not activate until all gates are
ready and reviewed. Later, `python tools/quality/hooks.py install` installs the
tracked hook path only when no custom hooks/configuration exist. It refuses to
overwrite them. The push hook blocks every destination `refs/heads/main`, including
`HEAD:main` and deletion; checks every outgoing nondeleted tip in a temporary
snapshot; and never downloads dependencies. New branches need a local
`origin/main`. Set `MSC_DEPENDENCY_ROOT` to an absolute prepared `_deps` directory
if different from `build/_deps`. Hooks are bypassable; they are not remote protection.

CI uses the same gates, GCC **16.2.0** and LLVM format **23.1.1**. The tidy wheel
available on PyPI is **23.1.3**, explicitly pinned separately; local 23.1.1 remains
supported. Python/Node/tool packages and actions are pinned; distro native packages
remain distribution-managed. Debian gate containers build checksum-pinned
CPython 3.14.7 from official source because setup-python has no Debian 13 build;
the Ubuntu commit-policy job uses setup-python. Jinja2 is pinned for GLAD.
Existing CMake output placement is retained. Hosted analyzer/build results
remain unverified until the container setup succeeds.
Toolchain references:
[official GCC image](https://hub.docker.com/_/gcc/),
[LLVM packages](https://apt.llvm.org/),
[cppcheck dialects](https://cppcheck.sourceforge.io/manual.html).
PR quality/build jobs validate GitHub's proposed merge tree. Commit policy reads
real head/base history and event JSON, including PR title edits; it does not lint
the synthetic merge commit. Push jobs use distinct `*-push` names (history:
`commit-history-push`). Stale runs are cancelled per PR/ref; title edits rerun
all required PR jobs.

Remote policy is a **reviewed proposal, unapplied**. Once gates pass, require PRs
and `quality-format`, `quality-tidy`, `quality-complexity`, `quality-cppcheck`,
`build-test`, `commit-policy`; enforce admins with no bypass, force pushes or main
deletion. Keep normal merges and set their default title to `PR_TITLE`; require
conventional titles without demanding self-approval or squash-only merges.
Do not enable required checks while the legacy gates fail. No history rewriting.
