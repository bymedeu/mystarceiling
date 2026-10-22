#!/usr/bin/env python3
"""All tracked first-party code; no installation or network operations."""
import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[2]
TOOLS = Path(os.environ.get('MSC_QUALITY_TOOLS', ROOT / 'tools/quality'))
CPP = {'.c', '.cc', '.cpp', '.cxx', '.h', '.hh', '.hpp', '.hxx'}
GENERATED = {'vendor', 'third_party', '_deps', 'build', 'generated', 'node_modules',
             '.quality-venv'}


def run(args, **kwargs):
    kwargs.setdefault('check', True)
    return subprocess.run([str(x) for x in args], cwd=ROOT, **kwargs)


def tracked():
    names = run(['git', 'ls-files', '-z'], capture_output=True).stdout.split(b'\0')
    return [Path(os.fsdecode(n)) for n in names if n and
            not GENERATED.intersection(Path(os.fsdecode(n)).parts)]


def tool(name, version=None):
    path = shutil.which(name)
    if not path:
        raise RuntimeError(f'MISSING required analyzer/tool: {name}')
    if version:
        output = run([path, '--version'], capture_output=True, text=True)
        versions = (version,) if isinstance(version, str) else version
        if not any(v in output.stdout for v in versions):
            raise RuntimeError(f'{name}: expected {version}: {output.stdout}')
    return path


def compiler_args(build):
    cache = build / 'CMakeCache.txt'
    entries = cache.read_text().splitlines() if cache.exists() else []
    args = []
    for language, binary in [('C', 'gcc'), ('CXX', 'g++')]:
        expected = tool(binary, '16.2.')
        key = f'CMAKE_{language}_COMPILER'
        previous = [line.split('=', 1)[1] for line in entries
                    if line.startswith(f'{key}:')]
        selected = previous[0] if previous else expected
        if Path(selected).resolve() != Path(expected).resolve():
            raise RuntimeError('Compiler cache mismatch; choose a fresh --build directory')
        args.append(f'-D{key}={selected}')
    return args


def configure(build):
    args = [tool('cmake', '4.4.4'), '-S', ROOT, '-B', build,
            '-DMSC_WARNINGS_AS_ERRORS=ON', '-DMSC_ENABLE_TESTS=ON',
            f'-DPython_EXECUTABLE={sys.executable}', *compiler_args(build)]
    if os.environ.get('CI') != 'true':
        deps = Path(os.environ.get('MSC_DEPENDENCY_ROOT', ROOT / 'build/_deps'))
        for name in ('glfw', 'glad'):
            source = deps / f'{name}-src'
            if not source.is_dir():
                raise RuntimeError(f'MISSING offline dependency: {source}')
            args.append(f'-DFETCHCONTENT_SOURCE_DIR_{name.upper()}={source}')
        args.append('-DFETCHCONTENT_FULLY_DISCONNECTED=ON')
    run(args)


def format_code(files):
    binary = tool('clang-format', '23.1.1')
    failed = []
    for file in files:
        result = subprocess.run([binary, '--dry-run', '--Werror', str(file)], cwd=ROOT)
        if result.returncode:
            failed.append(str(file))
    if failed:
        raise RuntimeError(f'Formatting failures ({len(failed)}): ' + ', '.join(failed))


def complexity(files):
    binary = tool('lizard', '1.24.2')
    run([binary, '-L40', '-C10', *files])


def tidy(files, build):
    binary = tool('clang-tidy', ('23.1.1', '23.1.3'))
    configure(build)
    database = json.loads((build / 'compile_commands.json').read_text())
    units = {str((ROOT / f).resolve()) for f in files if f.suffix not in
             {'.h', '.hh', '.hpp', '.hxx'}}
    covered = {str(Path(e['directory'], e['file']).resolve()) for e in database}
    if units - covered:
        raise RuntimeError(f'Uncovered translation units: {units - covered}')
    failures = []
    for unit in sorted(units):
        result = subprocess.run([binary, unit, '-p', str(build),
                                 '--warnings-as-errors=*'], cwd=ROOT)
        if result.returncode:
            failures.append(unit)
    if failures:
        raise RuntimeError(f'clang-tidy failed in {len(failures)} units')


def cppcheck(files):
    binary = tool('cppcheck', '2.22.0')
    run([binary, '--std=c++26', '--language=c++', '--enable=warning,style,performance,portability',
         '--error-exitcode=1', '-Iinclude', '-Isrc', *files])


def build_test(build):
    configure(build)
    run(['cmake', '--build', build, '--parallel', '2'])
    run(['ctest', '--test-dir', build, '--output-on-failure'])
    run([sys.executable, '-m', 'unittest', 'discover', '-s', 'tools', '-p', 'test_*.py'])
    run([sys.executable, '-m', 'unittest', 'discover', '-s', 'tools/quality', '-p', 'test_*.py'])


def message(text):
    binary = TOOLS / 'node_modules/.bin/commitlint'
    if not binary.is_file():
        raise RuntimeError('MISSING commitlint; run npm ci --prefix tools/quality')
    run([binary, '--config', TOOLS / 'commitlint.config.cjs'], input=text, text=True)


def commit_range(base, head):
    if not base:
        raise RuntimeError('--base is required for commit validation')
    commits = run(['git', 'rev-list', '--reverse', f'{base}..{head}'],
                  capture_output=True, text=True).stdout.splitlines()
    for commit in commits:
        body = run(['git', 'show', '-s', '--format=%B', commit],
                   capture_output=True, text=True).stdout
        message(body)


def arguments():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('gate', choices=['format', 'tidy', 'complexity', 'cppcheck',
                                        'build-test', 'all', 'message', 'commits'])
    parser.add_argument('--build', type=Path, default=ROOT / 'build/quality')
    parser.add_argument('--base')
    parser.add_argument('--head', default='HEAD')
    return parser.parse_args()


def main():
    args = arguments()
    files = tracked()
    cpp = [f for f in files if f.suffix in CPP]
    gates = {'format': lambda: format_code(cpp),
             'tidy': lambda: tidy(cpp, args.build.resolve()),
             'complexity': lambda: complexity([f for f in files if f.suffix in CPP | {'.py'}]),
             'cppcheck': lambda: cppcheck(cpp),
             'build-test': lambda: build_test(args.build.resolve()),
             'message': lambda: message(sys.stdin.read()),
             'commits': lambda: commit_range(args.base, args.head)}
    failed = []
    for name in gates if args.gate == 'all' else [args.gate]:
        if args.gate == 'all' and name in {'message', 'commits'}:
            continue
        try:
            gates[name]()
            print(f'{name}: PASS', flush=True)
        except (RuntimeError, subprocess.CalledProcessError) as error:
            print(f'{name}: FAIL: {error}', file=sys.stderr, flush=True)
            failed.append(name)
    return bool(failed)


if __name__ == '__main__':
    sys.exit(main())
