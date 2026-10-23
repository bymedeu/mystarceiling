#!/usr/bin/env python3
"""Portable live local styles and explicit once-per-CI-run retrieval."""

import os
from pathlib import Path
import shutil
import subprocess
import sys
from urllib.request import urlopen

ROOT = Path(__file__).resolve().parents[2]
NAMES = ('.clang-format', '.clang-tidy')
REMOTE = 'https://github.com/bymedeu/config.git'


def shared():
    return Path(os.environ.get('XDG_CONFIG_HOME', Path.home() / '.config')) / 'clang'


def materialize(root, source=None):
    destination = root / '.quality-style'
    if destination.is_symlink():
        raise RuntimeError('Refusing to materialize over a local style link')
    destination.mkdir(exist_ok=True)
    for name in NAMES:
        shutil.copyfile((source or shared()) / name, destination / name)


def link():
    source = shared()
    for name in NAMES:
        if not (source / name).is_file():
            raise RuntimeError(f'MISSING shared style: {name}')
    destination = ROOT / '.quality-style'
    if destination.is_symlink() and destination.resolve() == source.resolve():
        return
    if destination.exists() or destination.is_symlink():
        raise RuntimeError('Existing .quality-style preserved; inspect before setup')
    destination.symlink_to(source, target_is_directory=True)


def fetch(root=ROOT):
    destination = root / '.quality-style'
    if destination.is_symlink():
        raise RuntimeError('Refusing to fetch over a local style link')
    revision = subprocess.run(
        ['git', 'ls-remote', REMOTE, 'HEAD'], check=True, capture_output=True, text=True
    ).stdout.split()[0]
    if len(revision) != 40 or any(c not in '0123456789abcdef' for c in revision):
        raise RuntimeError('Invalid central style revision')
    destination.mkdir(exist_ok=True)
    print(f'Central Clang configuration revision: {revision}', flush=True)
    for name in NAMES:
        url = f'https://raw.githubusercontent.com/bymedeu/config/{revision}/clang/{name}'
        with urlopen(url, timeout=60) as response:
            (destination / name).write_bytes(response.read())
    return revision


if __name__ == '__main__':
    if sys.argv[1:] == ['local']:
        link()
    elif sys.argv[1:] == ['fetch']:
        fetch()
    else:
        sys.exit('Expected local or fetch (fetch is explicit setup/CI only)')
