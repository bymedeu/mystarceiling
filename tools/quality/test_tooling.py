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


class ToolingTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
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
            hooks.push_updates(io.StringIO('HEAD abc refs/heads/feature def\n'
                                          '(delete) 000 refs/heads/main def\n'))
        self.assertEqual(hooks.push_updates(['x 000 refs/heads/feature abc']), [])

    def test_inventory_includes_headers_tests_and_python(self):
        for file in ('include/a.hh', 'tests/a.cc', 'tools/a.py', 'vendor/a.cc'):
            path = self.root / file
            path.parent.mkdir(exist_ok=True)
            path.write_text('')
        self.git('add', '.')
        self.assertEqual(set(check.tracked()),
                         {Path('include/a.hh'), Path('tests/a.cc'), Path('tools/a.py')})

    def test_compiler_mismatch_fails_before_cmake_can_reset_offline_flags(self):
        (self.root / 'CMakeCache.txt').write_text('CMAKE_C_COMPILER:FILEPATH=/old/gcc\n')
        with patch.object(check, 'tool', return_value='/new/gcc'):
            with patch.object(check, 'run') as execute:
                with self.assertRaisesRegex(RuntimeError, 'Compiler cache mismatch'):
                    check.configure(self.root)
                execute.assert_not_called()

    def test_missing_analyzer_fails(self):
        with patch.dict(os.environ, {'PATH': ''}):
            with self.assertRaisesRegex(RuntimeError, 'MISSING.*cppcheck'):
                check.cppcheck([])

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
        self.assertEqual(self.git('config', '--get', 'core.hooksPath').stdout.strip(),
                         'custom-hooks')

    def test_installer_preserves_custom_hook(self):
        custom = self.root / '.git/hooks/pre-push'
        custom.write_text('# owner hook\n')
        with self.assertRaisesRegex(RuntimeError, 'Existing custom hooks'):
            hooks.install()
        self.assertEqual(custom.read_text(), '# owner hook\n')
        self.assertNotEqual(check.run(['git', 'config', '--get', 'core.hooksPath'],
                                      check=False, capture_output=True).returncode, 0)

    def test_installer_in_empty_fixture(self):
        hooks.install()
        self.assertEqual(self.git('config', '--get', 'core.hooksPath').stdout.strip(),
                         '.githooks')

    def test_snapshot_uses_outgoing_commit(self):
        (self.root / 'tracked.py').write_text('# committed\n')
        self.git('add', '.')
        self.git('-c', 'user.name=Fixture', '-c', 'user.email=fixture@example.test',
                 'commit', '-qm', 'test: snapshot')
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
        shutil.copytree(source / 'tools/quality', self.root / 'tools/quality',
                        ignore=shutil.ignore_patterns('node_modules', '__pycache__'))
        binaries = self.root / '.quality-venv/bin'
        binaries.mkdir(parents=True)
        (binaries / 'python').symlink_to(sys.executable)
        hooks.install()
        env = dict(os.environ, MSC_QUALITY_TOOLS=str(check.TOOLS))
        result = subprocess.run([self.root / '.githooks/pre-push'], cwd=self.root,
                                input='HEAD abc refs/heads/main def\n', text=True,
                                capture_output=True, env=env)
        self.assertNotEqual(result.returncode, 0)
        self.assertIn('destination main', result.stderr)
        msg = self.root / 'message'
        for text, expected in [('chore: fixture', 0), ('malformed', 1)]:
            msg.write_text(text)
            result = subprocess.run([self.root / '.githooks/commit-msg', msg],
                                    cwd=self.root, capture_output=True, env=env)
            self.assertEqual(result.returncode, expected)

    def commit(self, message):
        self.git('-c', 'user.name=Fixture', '-c', 'user.email=fixture@example.test',
                 'commit', '--allow-empty', '-qm', message)
        return self.git('rev-parse', 'HEAD').stdout.strip()

    def test_pr_policy_uses_real_head_and_validates_title(self):
        base = self.commit('historical nonconventional baseline')
        head = self.commit('chore: introduced real head')
        self.commit('Merge synthetic invalid title')
        event = {'pull_request': {'title': 'chore: reviewed title',
                                 'base': {'sha': base}, 'head': {'sha': head}}}
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
