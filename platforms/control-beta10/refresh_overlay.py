#!/usr/bin/env python3
"""Regenerate the pinned patch after editing canonical RU; never switch branches."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import tempfile

from audit_strings import HERE, audit, load


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, required=True, help='prepared upstream + overlay export, not the fork checkout')
    parser.add_argument('--repo', type=Path, default=HERE.parent.parent)
    args = parser.parse_args()
    source = args.source.resolve()
    if source == args.repo.resolve() or (source / '.git').exists():
        parser.error('source must be an isolated export, not a git checkout')
    manifest = json.loads((HERE / 'manifest.json').read_text())
    base = manifest['upstream']
    if (source / 'assets/en.strings').read_bytes() != (HERE / 'en.strings').read_bytes():
        parser.error('English baseline changed: review new/changed strings before regenerating')
    issues = audit(load(HERE / 'en.strings'), load(source / 'assets/en.strings'), load(HERE / 'ru.strings'))
    if issues:
        parser.error(f'Unresolved string contracts: {issues}')
    with tempfile.TemporaryDirectory(prefix='kobalt-ru-index-') as tmp:
        env = dict(os.environ, GIT_INDEX_FILE=str(Path(tmp) / 'index'))

        def git(*cmd):
            return subprocess.check_output(['git', '-C', str(args.repo), *cmd], env=env)

        git('read-tree', base)
        # RU is the only file copied into the source export. Other 35 files
        # must already contain the explicitly reviewed localization changes.
        (source / 'assets/ru.strings').write_bytes((HERE / 'ru.strings').read_bytes())
        for entry in manifest['files']:
            path = entry['path']
            upstream = subprocess.run(['git', '-C', str(args.repo), 'show', f'{base}:{path}'], capture_output=True)
            upstream_hash = hashlib.sha256(upstream.stdout).hexdigest() if upstream.returncode == 0 else None
            if upstream.returncode != 0:
                listing = git('ls-tree', base, '--', path)
                if listing:
                    raise RuntimeError(f'Cannot read existing upstream file: {path}')
            if upstream_hash != entry['upstream_sha256']:
                raise ValueError(f'Baseline hash mismatch: {path}')
            data = (source / path).read_bytes()
            entry['sha256'] = hashlib.sha256(data).hexdigest()
            blob = git('hash-object', '-w', str(source / path)).decode().strip()
            git('update-index', '--add', '--cacheinfo', '100644', blob, path)
        patch = git('diff', '--cached', '--binary', '--full-index', base)
    (HERE / 'ru.patch').write_bytes(patch)
    (HERE / 'manifest.json').write_text(json.dumps(manifest, indent=2) + '\n')
    print('Regenerated ru.patch and manifest.json; run verify.py before publishing.')


if __name__ == '__main__':
    main()
