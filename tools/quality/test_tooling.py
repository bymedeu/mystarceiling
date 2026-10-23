"""Offline regression tests in temporary Git repositories."""

import io
import json
import os
from pathlib import Path
import subprocess
import shutil
import sys
import tempfile
import unittest
from unittest.mock import patch

import check
import ci_policy
import hooks
import styles


class ToolingTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        environment = patch.dict(os.environ, GIT_CONFIG_GLOBAL=os.devnull, GIT_CONFIG_NOSYSTEM='1')
        environment.start()
        self.addCleanup(environment.stop)
        subprocess.run(['git', 'init', '-q', self.root], check=True)
        self.scope = patch.object(check, 'ROOT', self.root)
        self.scope.start()
        self.addCleanup(self.scope.stop)

    def git(self, *args):
        return check.run(['git', *args], capture_output=True, text=True)

    def test_destination_main_blocked_before_any_gate(self):
        self.git('symbolic-ref', 'HEAD', 'refs/heads/feature')
        for local in ('HEAD', 'refs/heads/feature'):
            with self.assertRaisesRegex(RuntimeError, 'destination main'):
                hooks.pre_push(io.StringIO(f'{local} abc refs/heads/main def\n'))

    def test_all_destinations_checked_including_delete(self):
        with self.assertRaisesRegex(RuntimeError, 'destination main'):
            hooks.push_updates(
                io.StringIO('HEAD abc refs/heads/feature def\n(delete) 000 refs/heads/main def\n')
            )
        self.assertEqual(hooks.push_updates(['x 000 refs/heads/feature abc']), [])

    def test_inventory_includes_headers_tests_and_python(self):
        for file in ('include/a.hh', 'tests/a.cc', 'tools/a.py', 'vendor/a.cc'):
            path = self.root / file
            path.parent.mkdir(exist_ok=True)
            path.write_text('')
        self.git('add', '.')
        self.assertEqual(
            set(check.tracked()),
            {Path('include/a.hh'), Path('tests/a.cc'), Path('tools/a.py')},
        )

    def test_compiler_mismatch_fails_before_cmake_can_reset_offline_flags(self):
        (self.root / 'CMakeCache.txt').write_text('CMAKE_C_COMPILER:FILEPATH=/old/gcc\n')
        with patch.object(check, 'tool', return_value='/new/gcc'):
            with patch.object(check, 'run') as execute:
                with self.assertRaisesRegex(RuntimeError, 'Compiler cache mismatch'):
                    check.configure(self.root)
                execute.assert_not_called()

    def test_werror_remains_required_for_build_configuration(self):
        with patch.dict(os.environ, {'CI': 'true'}):
            with patch.object(check, 'tool', return_value='cmake'):
                with patch.object(check, 'compiler_args', return_value=[]):
                    with patch.object(check, 'run') as configure:
                        check.configure(self.root)
                        check.configure(self.root, warnings_as_errors=False)
        self.assertIn('-DMSC_WARNINGS_AS_ERRORS=ON', configure.call_args_list[0].args[0])
        self.assertIn('-DMSC_WARNINGS_AS_ERRORS=OFF', configure.call_args_list[1].args[0])
        self.assertIn('-DMSC_ENABLE_SANITIZERS=OFF', configure.call_args_list[0].args[0])

    def test_sanitizer_configuration_explicitly_enables_instrumentation(self):
        with patch.dict(os.environ, {'CI': 'true'}):
            with patch.object(check, 'tool', return_value='cmake'):
                with patch.object(check, 'compiler_args', return_value=[]):
                    with patch.object(check, 'run') as configure:
                        check.configure(self.root, warnings_as_errors=False, sanitizers=True)
        self.assertIn('-DMSC_ENABLE_SANITIZERS=ON', configure.call_args.args[0])
        self.assertIn('-DMSC_WARNINGS_AS_ERRORS=OFF', configure.call_args.args[0])

    def configure_project(self, sanitizers):
        source = Path(__file__).resolve().parents[2]
        deps = Path(os.environ.get('MSC_DEPENDENCY_ROOT', source / 'build/_deps'))
        if not deps.is_dir():
            deps = source / 'build/quality/_deps'
        with patch.dict(os.environ, CI='false', MSC_DEPENDENCY_ROOT=str(deps)):
            with patch.object(check, 'ROOT', source):
                check.configure(self.root, warnings_as_errors=False, sanitizers=sanitizers)
        return json.loads((self.root / 'compile_commands.json').read_text())

    def test_real_cmake_limits_sanitizers_to_all_first_party_units(self):
        source = Path(__file__).resolve().parents[2]
        database = self.configure_project(sanitizers=True)
        first_party = [
            entry
            for entry in database
            if Path(entry['file']).is_relative_to(source / 'src')
            or Path(entry['file']).is_relative_to(source / 'tests')
        ]
        vendors = [entry for entry in database if entry not in first_party]
        self.assertTrue(first_party)
        self.assertTrue(vendors)
        targets = set()
        for entry in first_party:
            command = entry['command']
            targets.add(command.split('CMakeFiles/')[1].split('.dir')[0])
            for flag in (
                '-fsanitize=address,undefined',
                '-fno-sanitize-recover=all',
                '-fno-omit-frame-pointer',
                '-g',
            ):
                self.assertIn(flag, command.split())
        self.assertEqual(
            targets,
            {
                'mystarceiling_core',
                'mystarceiling_catalogue',
                'mystarceiling_m1',
                'mystarceiling_cli',
                'mystarceiling_renderer_gl',
                'mystarceiling_tests',
            },
        )
        self.assert_vendor_and_link_sanitizers(vendors, source)

    def assert_vendor_and_link_sanitizers(self, vendors, source):
        for entry in vendors:
            self.assertNotIn('-fsanitize', entry['command'])
            self.assertNotIn('-Werror', entry['command'])
        for directory, target in (
            ('src/cli', 'mystarceiling_cli'),
            ('tests', 'mystarceiling_tests'),
        ):
            link = self.root / directory / 'CMakeFiles' / f'{target}.dir/link.txt'
            self.assertIn('-fsanitize=address,undefined', link.read_text())
        test_link = link.read_text()
        self.assertNotIn(str(source / 'bin'), test_link)

    def test_ordinary_configure_clears_previous_sanitizer_option(self):
        self.configure_project(sanitizers=True)
        database = self.configure_project(sanitizers=False)
        for entry in database:
            self.assertNotIn('-fsanitize', entry['command'])
        self.assertIn('MSC_ENABLE_SANITIZERS:BOOL=OFF', (self.root / 'CMakeCache.txt').read_text())

    def test_sanitizer_runner_fails_on_build_or_test_failure(self):
        for failure_at in (0, 1):
            calls = []

            def execute(command, **kwargs):
                calls.append(command)
                self.assertIn('detect_leaks=1', kwargs['env']['ASAN_OPTIONS'])
                self.assertIn('halt_on_error=1', kwargs['env']['ASAN_OPTIONS'])
                self.assertIn('halt_on_error=1', kwargs['env']['UBSAN_OPTIONS'])
                if len(calls) == failure_at + 1:
                    raise subprocess.CalledProcessError(1, command)

            with patch.object(check, 'configure'):
                with patch.object(check, 'run', side_effect=execute):
                    with self.assertRaises(subprocess.CalledProcessError):
                        check.sanitizer_test(self.root)
            self.assertEqual(len(calls), failure_at + 1)
            if failure_at == 1:
                self.assertEqual(calls[-1][0], 'ctest')
                self.assertIn('--no-tests=error', calls[-1])

    def test_ordinary_runner_requires_tests_and_keeps_python_suites(self):
        with patch.object(check, 'configure'):
            with patch.object(check, 'run') as execute:
                check.build_test(self.root)
        commands = [call.args[0] for call in execute.call_args_list]
        self.assertIn('--no-tests=error', commands[1])
        self.assertEqual([command[5] for command in commands[2:]], ['tools', 'tools/quality'])

    def test_ctest_rejects_empty_suite(self):
        result = check.run(
            ['ctest', '--test-dir', self.root, '--no-tests=error'],
            check=False,
            capture_output=True,
            text=True,
        )
        self.assertNotEqual(result.returncode, 0)
        self.assertIn('No tests were found', result.stderr)

    def test_build_directory_collision_rejected_before_running_gates(self):
        arguments = [
            'check.py',
            'all',
            '--build',
            str(self.root),
            '--sanitizer-build',
            str(self.root / 'nested/..'),
        ]
        with patch.object(sys, 'argv', arguments):
            with patch.object(check, 'tracked') as inventory:
                with self.assertRaises(SystemExit) as failure:
                    check.main()
                self.assertEqual(failure.exception.code, 2)
                inventory.assert_not_called()

    def test_tidy_generates_glad_before_fresh_analysis(self):
        build = self.root / 'fresh-build'
        header = build / 'src/render/gladsources/msc_glad/include/glad/gl.h'
        unit = self.root / 'src/render/opengl_renderer.cc'

        def configure_fixture(_, warnings_as_errors):
            self.assertFalse(warnings_as_errors)
            build.mkdir()
            database = [{'directory': str(self.root), 'file': str(unit)}]
            (build / 'compile_commands.json').write_text(json.dumps(database))

        def generate(args):
            self.assertEqual(
                args,
                ['cmake', '--build', build, '--target', 'msc_glad', '--parallel', '2'],
            )
            self.assertFalse(header.exists())
            header.parent.mkdir(parents=True)
            header.write_text('// generated fixture\n')

        def analyze(args, **kwargs):
            self.assertTrue(header.is_file())
            self.assertEqual(args[1], str(unit))
            return subprocess.CompletedProcess(args, 0)

        with patch.object(check, 'configure', side_effect=configure_fixture):
            with patch.object(check, 'tool', return_value='clang-tidy'):
                with patch.object(check, 'run', side_effect=generate):
                    with patch.object(check.subprocess, 'run', side_effect=analyze) as tidy:
                        check.tidy([Path('src/render/opengl_renderer.cc')], build)
                        tidy.assert_called_once()

    def test_workflow_uses_merge_quality_and_real_head_policy(self):
        workflow = (
            Path(__file__).resolve().parents[2] / '.github/workflows/quality.yml'
        ).read_text()
        self.assertNotIn('          ref:', workflow)
        self.assertIn('fetch-depth: 0', workflow)
        self.assertIn('branches: [main]', workflow)
        self.assertNotIn('  push:', workflow)
        self.assertIn('    name: quality', workflow)
        self.assertNotIn('matrix', workflow)
        self.assertNotIn('continue-on-error', workflow)
        self.assertNotIn('check.py tidy', workflow)
        self.assertIn('python3 python3-venv', workflow)
        self.assertIn('-r tools/quality/requirements.txt', workflow)
        self.assertNotIn('requirements-ci.txt', workflow)
        self.assertIn('edited', workflow)
        self.assertIn('cancel-in-progress: true', workflow)
        commands = [
            'ci_policy.py',
            'check.py build-test',
            'check.py sanitizer-test',
            'check.py complexity',
            'check.py cppcheck',
            'check.py format',
        ]
        positions = [workflow.index(command) for command in commands]
        self.assertEqual(positions, sorted(positions))
        self.assertNotIn('\n        if:', workflow)

    def test_missing_analyzer_fails(self):
        with patch.dict(os.environ, {'PATH': ''}):
            with self.assertRaisesRegex(RuntimeError, 'MISSING.*cppcheck'):
                check.cppcheck([])

    def cppcheck_fixture_headers(self):
        return [
            Path(name)
            for name in ('include/one.hh', 'include/two.h', 'src/nested/three.hpp', 'src/four.hxx')
        ]

    def test_cppcheck_analyzes_every_header_and_real_unit(self):
        headers = self.cppcheck_fixture_headers()
        units = [Path('src/actual.cc'), Path('tests/actual.cpp')]
        for file in headers + units:
            path = self.root / file
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_text(f'inline int {file.stem}() {{ return 1; }}\n')
        self.git('add', '.')
        self.assertEqual(set(check.tracked()), set(headers + units))
        requested = check.tracked()
        binary = check.tool('cppcheck', '2.22.0')
        execute = check.run
        wrappers = []

        def inspect_and_analyze(command):
            self.assertEqual(
                command[:7],
                [
                    binary,
                    '--std=c++26',
                    '--language=c++',
                    '--enable=warning,style,performance,portability',
                    '--error-exitcode=1',
                    '-Iinclude',
                    '-Isrc',
                ],
            )
            wrappers.extend(file for file in command[7:] if file not in units)
            includes = {file.read_text() for file in wrappers}
            self.assertEqual(includes, {f'#include "{self.root / header}"\n' for header in headers})
            self.assertEqual(set(command[7:]) - set(wrappers), set(units))
            return execute(command, capture_output=True, text=True)

        with patch.object(check, 'tool', return_value=binary):
            with patch.object(check, 'run', side_effect=inspect_and_analyze) as analyze:
                check.cppcheck(requested)
                analyze.assert_called_once()
        self.assertEqual(len(wrappers), len(headers))
        self.assertTrue(all(not file.exists() for file in wrappers))

    def test_real_cppcheck_rejects_defect_in_included_header(self):
        header = self.root / 'defect.hh'
        header.write_text('inline int divide(int numerator) { return numerator / 0; }\n')
        binary = check.tool('cppcheck', '2.22.0')
        execute = check.run

        def analyze(command):
            return execute(command, capture_output=True, text=True)

        with patch.object(check, 'tool', return_value=binary):
            with patch.object(check, 'run', side_effect=analyze):
                with self.assertRaises(subprocess.CalledProcessError) as failure:
                    check.cppcheck([header])
        self.assertIn('zerodiv', failure.exception.stderr)
        self.assertIn(str(header), failure.exception.stderr)
        self.assertEqual(failure.exception.returncode, 1)
        wrapper = Path(failure.exception.cmd[-1])
        self.assertFalse(wrapper.exists())

    def test_conventional_library_rejects_malformed_message(self):
        check.message('chore: validate tooling\n')
        with self.assertRaises(subprocess.CalledProcessError):
            check.message('arbitrary message without a type\n')
        with self.assertRaises(subprocess.CalledProcessError):
            check.message('Merge pull request #1 from feature\n')

    def test_installer_preserves_configuration(self):
        self.git('config', 'core.hooksPath', 'custom-hooks')
        with self.assertRaisesRegex(RuntimeError, 'Existing core.hooksPath'):
            hooks.install()
        self.assertEqual(
            self.git('config', '--get', 'core.hooksPath').stdout.strip(), 'custom-hooks'
        )

    def test_installer_preserves_custom_hook_and_uses_shared_profile(self):
        custom = self.root / '.git/hooks/pre-push'
        custom.write_text('# owner hook\n')
        with patch.object(styles, 'link'):
            hooks.install()
        self.assertEqual(custom.read_text(), '# owner hook\n')
        self.assertEqual(
            self.git('config', '--get', 'quality.adapter').stdout.strip(),
            'tools/quality/hooks.py',
        )
        self.assertEqual(
            self.git('config', '--get', 'quality.config').stdout.strip(),
            'tools/quality/pre-commit.yaml',
        )
        self.assertNotEqual(
            check.run(
                ['git', 'config', '--get', 'core.hooksPath'],
                check=False,
                capture_output=True,
            ).returncode,
            0,
        )

    def test_snapshot_uses_outgoing_commit(self):
        (self.root / 'tracked.py').write_text('# committed\n')
        self.git('add', '.')
        self.git(
            '-c',
            'user.name=Fixture',
            '-c',
            'user.email=fixture@example.test',
            'commit',
            '-qm',
            'test: snapshot',
        )
        sha = self.git('rev-parse', 'HEAD').stdout.strip()
        (self.root / 'tracked.py').write_text('# later commit\n')
        self.git('add', '.')
        self.commit('test: later head')
        (self.root / 'tracked.py').write_text('# dirty\n')
        destination = self.root / 'archive'
        destination.mkdir()
        with patch.dict(os.environ, {'GIT_DIR': str(self.root / '.git')}):
            tree = hooks.snapshot(sha, destination)
        self.assertTrue((tree / '.git/index').is_file())
        self.assertEqual((self.root / 'tracked.py').read_text(), '# dirty\n')
        self.assertEqual((tree / 'tracked.py').read_text(), '# committed\n')
        self.assertFalse((tree / 'untracked.py').exists())

    def test_real_hook_entrypoints(self):
        source = Path(__file__).resolve().parents[2]
        shutil.copytree(source / '.githooks', self.root / '.githooks')
        shutil.copytree(
            source / 'tools/quality',
            self.root / 'tools/quality',
            ignore=shutil.ignore_patterns('node_modules', '__pycache__'),
        )
        binaries = self.root / '.quality-venv/bin'
        binaries.mkdir(parents=True)
        (binaries / 'python').symlink_to(sys.executable)
        env = dict(os.environ, MSC_QUALITY_TOOLS=str(check.TOOLS))
        result = subprocess.run(
            [self.root / '.githooks/pre-push'],
            cwd=self.root,
            input='HEAD abc refs/heads/main def\n',
            text=True,
            capture_output=True,
            env=env,
        )
        self.assertNotEqual(result.returncode, 0)
        self.assertIn('destination main', result.stderr)
        msg = self.root / 'message'
        for text, expected in [('chore: fixture', 0), ('malformed', 1)]:
            msg.write_text(text)
            result = subprocess.run(
                [self.root / '.githooks/commit-msg', msg],
                cwd=self.root,
                capture_output=True,
                env=env,
            )
            self.assertEqual(result.returncode, expected)

    def commit(self, message):
        self.git(
            '-c',
            'user.name=Fixture',
            '-c',
            'user.email=fixture@example.test',
            'commit',
            '--allow-empty',
            '-qm',
            message,
        )
        return self.git('rev-parse', 'HEAD').stdout.strip()

    def test_pr_policy_uses_real_head_and_validates_title(self):
        base = self.commit('historical nonconventional baseline')
        head = self.commit('chore: introduced real head')
        self.commit('Merge synthetic invalid title')
        event = {
            'pull_request': {
                'title': 'chore: reviewed title',
                'base': {'sha': base},
                'head': {'sha': head},
            }
        }
        path = self.root / 'event.json'
        path.write_text(json.dumps(event))
        with patch.dict(os.environ, {'GITHUB_EVENT_PATH': str(path)}):
            ci_policy.main()
            event['pull_request']['title'] = 'bad title'
            path.write_text(json.dumps(event))
            with self.assertRaises(subprocess.CalledProcessError):
                ci_policy.main()


if __name__ == '__main__':
    unittest.main()
