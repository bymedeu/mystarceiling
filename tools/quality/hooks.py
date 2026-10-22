#!/usr/bin/env python3
"""Tracked Git hooks: destination policy, immutable snapshots, safe installation."""
import os
from pathlib import Path
import subprocess
import sys
import tarfile
import tempfile

import check


def push_updates(stream):
    updates = [line.split() for line in stream if line.strip()]
    for fields in updates:
        if len(fields) != 4:
            raise RuntimeError('Malformed pre-push input')
        if fields[2] == 'refs/heads/main':
            raise RuntimeError('Direct pushes to destination main are blocked; use a PR')
    return [fields for fields in updates if set(fields[1]) != {'0'}]



def clean_git_env():
    names = check.run(['git', 'rev-parse', '--local-env-vars'],
                      capture_output=True, text=True).stdout.splitlines()
    return {key: value for key, value in os.environ.items() if key not in names}


def snapshot(sha, destination):
    archive = destination / 'snapshot.tar'
    with archive.open('wb') as output:
        check.run(['git', 'archive', sha], stdout=output)
    tree = destination / 'tree'
    tree.mkdir()
    with tarfile.open(archive) as source:
        source.extractall(tree, filter='data')
    env = clean_git_env()
    subprocess.run(['git', 'init', '-q', tree], check=True, env=env)
    subprocess.run(['git', '-C', tree, 'add', '--force', '.'], check=True, env=env)
    return tree


def pre_push(stream):
    updates = push_updates(stream)
    env = dict(clean_git_env(), MSC_QUALITY_TOOLS=str(check.TOOLS.resolve()),
               MSC_DEPENDENCY_ROOT=os.environ.get(
                   'MSC_DEPENDENCY_ROOT', str(check.ROOT / 'build/_deps')))
    # CI permits explicit dependency fetching; a push must always stay offline.
    env.pop('CI', None)
    for _, sha, _, remote_sha in updates:
        base = remote_sha
        if set(base) == {'0'}:
            base = check.run(['git', 'merge-base', sha, 'refs/remotes/origin/main'],
                             capture_output=True, text=True).stdout.strip()
        check.commit_range(base, sha)
        with tempfile.TemporaryDirectory(prefix='msc-push-') as directory:
            tree = snapshot(sha, Path(directory))
            subprocess.run([sys.executable, tree / 'tools/quality/check.py', 'all'],
                           cwd=tree, env=env, check=True)


def install():
    existing = check.run(['git', 'config', '--get', 'core.hooksPath'],
                         check=False, capture_output=True, text=True)
    if existing.stdout.strip():
        raise RuntimeError('Existing core.hooksPath: preserve it; request review')
    path = check.run(['git', 'rev-parse', '--git-path', 'hooks'],
                     capture_output=True, text=True).stdout.strip()
    hooks = Path(path)
    if not hooks.is_absolute():
        hooks = check.ROOT / hooks
    custom = [p.name for p in hooks.glob('*') if not p.name.endswith('.sample')]
    if custom:
        raise RuntimeError(f'Existing custom hooks: {custom}; request review')
    check.run(['git', 'config', '--local', 'core.hooksPath', '.githooks'])


def main():
    try:
        if sys.argv[1] == 'install':
            install()
        elif sys.argv[1] == 'pre-push':
            pre_push(sys.stdin)
        elif sys.argv[1] == 'commit-msg':
            check.message(Path(sys.argv[2]).read_text())
        else:
            raise RuntimeError('Expected install, pre-push or commit-msg')
    except (RuntimeError, subprocess.CalledProcessError) as error:
        print(error, file=sys.stderr)
        return 1
    return 0


if __name__ == '__main__':
    sys.exit(main())
