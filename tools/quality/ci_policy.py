#!/usr/bin/env python3
"""Read event JSON as data; lint actual head history, never a synthetic merge."""
import json
import os
from pathlib import Path

import check


def main():
    event = json.loads(Path(os.environ['GITHUB_EVENT_PATH']).read_text())
    if 'pull_request' in event:
        pull = event['pull_request']
        check.message(pull['title'])
        head = pull['head']['sha']
        base = check.run(['git', 'merge-base', pull['base']['sha'], head],
                         capture_output=True, text=True).stdout.strip()
    else:
        head = event['after']
        base = event['before']
        if set(base) == {'0'}:
            base = check.run(['git', 'merge-base', head, 'origin/main'],
                             capture_output=True, text=True).stdout.strip()
    check.commit_range(base, head)


if __name__ == '__main__':
    main()
