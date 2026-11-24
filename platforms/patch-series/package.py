#!/usr/bin/env python3
"""Export the complete ordered series and verify its Qt5 hardware-test package."""
import argparse
import hashlib
import io
import json
import os
from pathlib import Path
import subprocess
import tarfile
import zipfile
from apply import apply_to, load_series, ROOT


def require_complete(data):
    if any(p['status'] != 'ready' for p in data['patches']):
        raise ValueError('Hardware package requires the entire series, not a prefix')
    if data['patches'][-1]['id'] != '060-build-integration':
        raise ValueError('Missing final integration patch')


def prepare(root, output):
    manifest = root / 'platforms/patch-series/series.json'
    data = load_series(manifest, root)
    require_complete(data)
    if output.exists():
        raise ValueError('Refusing to overwrite export')
    commit = subprocess.check_output(['git', '-C', str(root), 'rev-parse', 'HEAD'], text=True).strip()
    if os.environ.get('GITHUB_SHA', commit) != commit:
        raise ValueError('Checkout differs from workflow source commit')
    output.mkdir(parents=True)
    apply_to(root, manifest, output, ready_only=True)
    version = json.loads((output / 'assets/manifest.json').read_text())['version']
    provenance = dict(version=version, schema=1, policy='full-series-hardware-unverified',
                      commit=commit, upstream=data['upstream'], fork_source=data['fork_source'],
                      manifest_sha256=hashlib.sha256(manifest.read_bytes()).hexdigest(),
                      patches=data['patches'], run_url=os.environ.get('GITHUB_SERVER_URL', 'https://github.com')
                      + '/' + os.environ.get('GITHUB_REPOSITORY', 'Mutaracha/kobalt_ru')
                      + '/actions/runs/' + os.environ.get('GITHUB_RUN_ID', 'local'))
    (output / 'series-provenance.json').write_text(json.dumps(provenance, indent=2) + '\n')
    # Used by desktop's source-info.cmake too; Kobo receives the same commit explicitly.
    (output / 'source-info.json').write_text(json.dumps({'commit': commit, 'version': version}) + '\n')


def collect(source, destination, instructions):
    provenance = (source / 'series-provenance.json').read_bytes()
    data = json.loads(provenance)
    package = source / 'build/kobo-qt5/Kobalt-qt5.zip'
    with zipfile.ZipFile(package) as archive:
        if archive.read('.adds/kobalt/series-provenance.json') != provenance:
            raise ValueError('Installed package lost or changed series provenance')
        with tarfile.open(fileobj=io.BytesIO(archive.read('.kobo/KoboRoot.tgz')), mode='r:gz') as tar:
            members = tar.getmembers()
            expected = 'usr/local/Kobo/imageformats/libkobalt.so'
            if len(members) != 1 or members[0].name != expected:
                raise ValueError('Unexpected firmware installer contents')
            member = members[0]
            if not member.isfile() or member.uid != 0 or member.gid != 0 or member.mode != 0o755:
                raise ValueError('Invalid installed library ownership/mode')
            library = tar.extractfile(member).read()
            if library[:4] != b'\x7fELF' or library[4:6] != b'\x01\x01' or library[18:20] != b'\x28\x00':
                raise ValueError('Expected ELF32 little-endian ARM library')
    destination.mkdir(parents=True, exist_ok=False)
    (destination / 'Kobalt-ru-beta10-series-qt5.zip').write_bytes(package.read_bytes())
    (destination / 'series-provenance.json').write_bytes(provenance)
    (destination / 'source-commit.txt').write_text(data['commit'] + '\n')
    (destination / 'README.md').write_bytes(instructions.read_bytes())
    (destination / 'installed-library.sha256').write_text(hashlib.sha256(library).hexdigest()
                                                         + '  /usr/local/Kobo/imageformats/libkobalt.so\n')
    sums = ''.join(hashlib.sha256(p.read_bytes()).hexdigest() + '  ' + p.name + '\n'
                   for p in sorted(destination.iterdir()))
    (destination / 'SHA256SUMS').write_text(sums)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('mode', choices=['prepare', 'collect'])
    parser.add_argument('source', type=Path)
    parser.add_argument('destination', type=Path)
    args = parser.parse_args()
    if args.mode == 'prepare':
        prepare(args.source.resolve(), args.destination.resolve())
    else:
        collect(args.source.resolve(), args.destination.resolve(), ROOT / 'platforms/patch-series/DEVICE-TEST.md')
