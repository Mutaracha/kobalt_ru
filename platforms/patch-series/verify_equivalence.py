#!/usr/bin/env python3
"""Verify a pristine series export against the accepted pre-consolidation tree."""
import argparse
import hashlib
import json
from pathlib import Path


def tree(root):
    files = {}
    for path in sorted(root.rglob('*')):
        if path.is_symlink():
            import os
            data = os.readlink(path).encode()
            mode = '120000'
        elif path.is_file():
            data = path.read_bytes()
            mode = '100755' if path.stat().st_mode & 0o111 else '100644'
        else:
            continue
        files[path.relative_to(root).as_posix()] = dict(
            mode=mode, size=len(data), sha256=hashlib.sha256(data).hexdigest())
    return files


def verify(root, reference):
    expected = json.loads(reference.read_text())['files']
    actual = tree(root)
    changes = [path for path in sorted(expected.keys() | actual.keys())
               if expected.get(path) != actual.get(path)]
    if changes:
        raise ValueError('Consolidated runtime differs from accepted reference: '
                         + ', '.join(changes[:30]))
    print(f'PASS: {len(actual)} runtime files identical (path, bytes, executable/link mode)')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('export', type=Path)
    parser.add_argument('--reference', type=Path,
                        default=Path(__file__).parent / 'equivalence/reference-tree.json')
    args = parser.parse_args()
    verify(args.export, args.reference)


if __name__ == '__main__':
    main()
