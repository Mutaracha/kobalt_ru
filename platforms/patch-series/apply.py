#!/usr/bin/env python3
"""Apply one ordered full stack to a pristine export. Never change a branch."""
import argparse
import hashlib
import io
import json
import os
from pathlib import Path
import re
import subprocess
import tarfile
import tempfile

HERE = Path(__file__).resolve().parent
ROOT = HERE.parent.parent


def load_series(manifest, root):
    data = json.loads(manifest.read_text())
    if data.get('schema') != 1 or data.get('policy') != 'ordered-full-stack':
        raise ValueError('Unsupported series schema/policy')
    if not re.fullmatch(r'[0-9a-f]{40}', data.get('upstream', '')):
        raise ValueError('Pin upstream to a full commit SHA')
    entries = data.get('patches', [])
    if not entries:
        raise ValueError('Empty patch series')
    ids = [p['id'] for p in entries]
    if len(set(ids)) != len(ids) or ids != sorted(ids):
        raise ValueError('Patch IDs must be unique and ordered')
    pending = False
    for patch in entries:
        if patch.get('status') == 'planned':
            pending = True
            continue
        if patch.get('status') not in ('ready', 'prepared') or pending:
            raise ValueError('Prepared/ready patches must form an ordered prefix')
        path = (root / patch['path']).resolve()
        if not path.is_relative_to(root.resolve()):
            raise ValueError('Patch path escapes repository')
        digest = hashlib.sha256(path.read_bytes()).hexdigest()
        if digest != patch.get('sha256'):
            raise ValueError(f'Patch checksum mismatch: {patch["id"]}')
    return data


def apply_to(root, manifest, destination, ready_only=False):
    data = load_series(manifest, root)
    pending = [p['id'] for p in data['patches'] if p['status'] != 'ready']
    if pending and not ready_only:
        raise ValueError('Full stack not ready: ' + ', '.join(pending))
    archive = subprocess.check_output(['git', '-C', str(root), 'archive', data['upstream']])
    with tarfile.open(fileobj=io.BytesIO(archive)) as tar:
        for member in tar.getmembers():
            if not (destination / member.name).resolve().is_relative_to(destination.resolve()):
                raise ValueError('Unsafe archive path')
        tar.extractall(destination)
    for patch in data['patches']:
        if patch['status'] not in ('ready', 'prepared'):
            break
        path = (root / patch['path']).resolve()
        # The export can live below the parent checkout's ignored build/.
        # Do not let git discover that parent: subdirectory apply otherwise
        # silently skips patch paths outside the current prefix.
        env = {k: v for k, v in os.environ.items()
               if k not in ('GIT_DIR', 'GIT_WORK_TREE', 'GIT_INDEX_FILE', 'GIT_COMMON_DIR')}
        env['GIT_CEILING_DIRECTORIES'] = str(destination.resolve().parent)
        subprocess.run(['git', 'apply', '--check', str(path)], cwd=destination, env=env, check=True)
        subprocess.run(['git', 'apply', '--whitespace=nowarn', str(path)], cwd=destination, env=env, check=True)
        print('Applied ' + patch['id'] + ' [' + patch['status'] + ']', flush=True)
    return data


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--repo', type=Path, default=ROOT)
    parser.add_argument('--manifest', type=Path, default=HERE / 'series.json')
    mode = parser.add_mutually_exclusive_group(required=True)
    mode.add_argument('--output', type=Path, help='new full-stack export directory; refused while any patch is planned')
    mode.add_argument('--verify-ready', action='store_true', help='temporary prepared/ready prefix check ONLY, not a full-stack build')
    args = parser.parse_args()
    try:
        data = load_series(args.manifest, args.repo)
        pending = [p['id'] for p in data['patches'] if p['status'] != 'ready']
        if args.output:
            if pending:
                raise ValueError('Full stack not ready: ' + ', '.join(pending))
            out = args.output.absolute()
            if out.exists() or out.is_symlink():
                raise ValueError('Refusing to overwrite output: ' + str(out))
            if out.resolve() == args.repo.resolve():
                raise ValueError('Output must not be the source repository')
            out.parent.mkdir(parents=True, exist_ok=True)
            with tempfile.TemporaryDirectory(prefix='.kobalt-series-', dir=out.parent) as tmp:
                export = Path(tmp) / 'source'
                export.mkdir()
                apply_to(args.repo, args.manifest, export)
                export.rename(out)
            print('Prepared FULL STACK: ' + str(out))
        else:
            with tempfile.TemporaryDirectory(prefix='kobalt-series-check-') as tmp:
                apply_to(args.repo, args.manifest, Path(tmp), ready_only=True)
            print('Prepared/ready prefix applies; NOT a full-stack acceptance. Pending acceptance: ' + ', '.join(pending))
    except (ValueError, OSError, KeyError, subprocess.CalledProcessError) as exc:
        parser.exit(2, str(exc) + '\n')


if __name__ == '__main__':
    main()
