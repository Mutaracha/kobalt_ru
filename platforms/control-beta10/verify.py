#!/usr/bin/env python3
"""Verify the portable overlay against a pristine pinned upstream export."""
import argparse
import hashlib
import io
import json
from pathlib import Path
import subprocess
import tarfile
import tempfile

from audit_strings import audit, load

HERE = Path(__file__).resolve().parent
REPO = HERE.parent.parent


def sha(data):
    return hashlib.sha256(data).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--repo', type=Path, default=REPO, help='repository containing the pinned upstream object')
    args = parser.parse_args()
    manifest = json.loads((HERE / 'manifest.json').read_text())
    base = manifest['upstream']
    patch = HERE / 'ru.patch'
    # Objects must already exist; this verifier does not fetch or change branches.
    archive = subprocess.check_output(['git', '-C', str(args.repo), 'archive', base])
    with tempfile.TemporaryDirectory(prefix='kobalt-ru-verify-') as tmp:
        root = Path(tmp)
        with tarfile.open(fileobj=io.BytesIO(archive)) as tar:
            # Only the pinned, locally fetched git object is extracted here.
            # Accept Python 3.10/3.11 without tarfile's newer filter parameter.
            for member in tar.getmembers():
                if not (root / member.name).resolve().is_relative_to(root):
                    raise ValueError('Unsafe archive path')
            tar.extractall(root)
        assert (root / 'assets/en.strings').read_bytes() == (HERE / 'en.strings').read_bytes(), 'Wrong English baseline'
        for entry in manifest['files']:
            p = root / entry['path']
            actual = sha(p.read_bytes()) if p.exists() else None
            assert actual == entry['upstream_sha256'], f'Upstream hash: {entry["path"]}'
        # Apply in an independent temporary directory, not the dirty fork checkout.
        subprocess.run(['git', 'apply', '--check', str(patch)], cwd=root, check=True)
        subprocess.run(['git', 'apply', '--whitespace=nowarn', str(patch)], cwd=root, check=True)
        names = subprocess.check_output(['git', 'apply', '--numstat', '-z', str(patch)], cwd=root).decode().split('\0')
        paths = [line.split('\t', 2)[2] for line in names if line]
        assert set(paths) == {e['path'] for e in manifest['files']}, 'Patch/manifest path mismatch'
        for entry in manifest['files']:
            assert sha((root / entry['path']).read_bytes()) == entry['sha256'], f'Output hash: {entry["path"]}'
        assert (root / 'assets/ru.strings').read_bytes() == (HERE / 'ru.strings').read_bytes(), 'Canonical Russian drift'
        issues = audit(load(HERE / 'en.strings'), load(root / 'assets/en.strings'), load(root / 'assets/ru.strings'))
        assert not issues, issues
    print(f'PASS: pristine apply, {len(paths)} output hashes, canonical RU and EN/RU contracts')


if __name__ == '__main__':
    main()
