"""MSC adapter contracts, live configuration and immutable style snapshots."""

import io
import os
from pathlib import Path
import subprocess
import tempfile
import unittest
from unittest.mock import patch

import check
import hooks
import styles


class SharedIntegrationTests(unittest.TestCase):
    def test_push_gate_only_runs_requested_analyzers(self):
        with patch('sys.argv', ['check.py', 'push']):
            with patch.object(check, 'tracked', return_value=[]):
                with patch.object(check, 'complexity') as complexity:
                    with patch.object(check, 'cppcheck') as cppcheck:
                        with patch.object(check, 'configure') as configure:
                            self.assertFalse(check.main())
        complexity.assert_called_once()
        cppcheck.assert_called_once()
        configure.assert_not_called()

    def test_push_checks_each_outgoing_tip_and_uses_narrow_gate(self):
        updates = 'HEAD aaa refs/heads/one bbb\nother ccc refs/heads/two ddd\n'
        with patch.object(hooks, 'clean_git_env', return_value={}):
            with patch.object(check, 'commit_range') as commits:
                with patch.object(hooks, 'snapshot', return_value=Path('/snapshot')) as snapshot:
                    with patch.object(subprocess, 'run') as execute:
                        hooks.pre_push(io.StringIO(updates))
        self.assertEqual([c.args for c in commits.call_args_list], [('bbb', 'aaa'), ('ddd', 'ccc')])
        self.assertEqual([c.args[0] for c in snapshot.call_args_list], ['aaa', 'ccc'])
        for call in execute.call_args_list:
            self.assertEqual(call.args[0][2:], ['push', '--root', Path('/snapshot')])
            self.assertNotIn('CI', call.kwargs['env'])

    def test_new_branch_uses_local_base_and_deletions_do_no_analysis(self):
        with patch.object(check, 'run') as git:
            git.return_value.stdout = 'base\n'
            with patch.object(check, 'commit_range') as commits:
                with patch.object(hooks, 'snapshot', return_value=Path('/snapshot')):
                    with patch.object(hooks, 'clean_git_env', return_value={}):
                        with patch.object(subprocess, 'run'):
                            hooks.pre_push(io.StringIO('HEAD aaa refs/heads/new 000\n'))
                            hooks.pre_push(io.StringIO('delete 000 refs/heads/old aaa\n'))
        commits.assert_called_once_with('base', 'aaa')
        self.assertIn('refs/remotes/origin/main', git.call_args.args[0])

    def older_fixture(self, root):
        def git(*args):
            return subprocess.run(
                ['git', *args], cwd=root, check=True, capture_output=True, text=True
            ).stdout.strip()

        git('init', '-q')
        git('config', 'user.name', 'Fixture')
        git('config', 'user.email', 'fixture@example.test')
        controller = root / 'tools/quality/check.py'
        controller.parent.mkdir(parents=True)
        controller.write_text('raise SystemExit("OLD UNSUPPORTED CONTROLLER")\n')
        (root / 'test.hpp').write_text('inline int one() { return 1; }\n')
        git('add', '.')
        git('commit', '-qm', 'test: old tooling')
        good = git('rev-parse', 'HEAD')
        git('update-ref', 'refs/remotes/origin/main', good)
        (root / 'test.hpp').write_text('inline int divide(int n) { return n / 0; }\n')
        git('add', '.')
        git('commit', '-qm', 'test: defective outgoing header')
        bad = git('rev-parse', 'HEAD')
        (root / 'test.hpp').write_text('inline int divide(int n) { return n; }\n')
        return good, bad

    def test_real_older_tips_use_current_controller_and_immutable_inventory(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            with patch.dict(os.environ, GIT_CONFIG_GLOBAL=os.devnull, GIT_CONFIG_NOSYSTEM='1'):
                good, bad = self.older_fixture(root)
                with patch.object(check, 'ROOT', root):
                    hooks.pre_push(io.StringIO(f'HEAD {good} refs/heads/old {"0" * 40}\n'))
                    updates = f'old {good} refs/heads/old {"0" * 40}\n'
                    updates += f'bad {bad} refs/heads/bad {good}\n'
                    with self.assertRaises(subprocess.CalledProcessError):
                        hooks.pre_push(io.StringIO(updates))
            self.assertIn('return n;', (root / 'test.hpp').read_text())

    def test_ci_resolves_default_head_once_and_downloads_both_at_revision(self):
        revision = 'a' * 40
        result = subprocess.CompletedProcess([], 0, stdout=revision + '\tHEAD\n')
        with tempfile.TemporaryDirectory() as directory:
            with patch.object(subprocess, 'run', return_value=result) as resolve:
                with patch.object(styles, 'urlopen') as download:
                    download.return_value.__enter__.return_value.read.return_value = b'style'
                    self.assertEqual(styles.fetch(Path(directory)), revision)
        resolve.assert_called_once()
        self.assertEqual(resolve.call_args.args[0][-1], 'HEAD')
        self.assertEqual(len(download.call_args_list), 2)
        for name, call in zip(styles.NAMES, download.call_args_list):
            self.assertTrue(call.args[0].endswith(f'/{revision}/clang/{name}'))
        workflow = (styles.ROOT / '.github/workflows/quality.yml').read_text()
        self.assertEqual(workflow.count('styles.py fetch'), 1)
        self.assertTrue(workflow.rstrip().endswith('python tools/quality/check.py format'))

    def test_snapshot_materializes_live_style_offline(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            source = root / 'shared'
            source.mkdir()
            for name in styles.NAMES:
                (source / name).write_text(f'# live {name}\n')
            tree = root / 'tree'
            tree.mkdir()
            for name in styles.NAMES:
                (tree / name).symlink_to('.quality-style/' + name)
            with patch.object(styles, 'shared', return_value=source):
                styles.materialize(tree)
            for name in styles.NAMES:
                self.assertEqual((tree / name).read_text(), (source / name).read_text())
            (source / '.clang-format').write_text('# changed\n')
            self.assertNotEqual((tree / '.clang-format').read_text(), '# changed\n')

    def test_fetch_refuses_local_shared_link_before_network_or_writes(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            central = root / 'central'
            central.mkdir()
            sentinel = central / '.clang-format'
            sentinel.write_text('unpublished style')
            (root / '.quality-style').symlink_to(central, target_is_directory=True)
            with patch.object(subprocess, 'run') as network:
                with patch.object(styles, 'urlopen') as download:
                    with self.assertRaisesRegex(RuntimeError, 'local style link'):
                        styles.fetch(root)
            network.assert_not_called()
            download.assert_not_called()
            self.assertEqual(sentinel.read_text(), 'unpublished style')

    def test_local_links_keep_one_style_and_preserve_conflicts(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            shared = root / 'shared'
            shared.mkdir()
            for name in styles.NAMES:
                (shared / name).write_text('')
            with patch.object(styles, 'ROOT', root):
                with patch.object(styles, 'shared', return_value=shared):
                    styles.link()
                    styles.link()
                    self.assertEqual((root / '.quality-style').resolve(), shared)
                    (root / '.quality-style').unlink()
                    (root / '.quality-style').mkdir()
                    with self.assertRaisesRegex(RuntimeError, 'preserved'):
                        styles.link()


if __name__ == '__main__':
    unittest.main()
