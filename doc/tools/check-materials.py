#!/usr/bin/env python3
"""Check documentation archive moves, payload integrity and maintained local links."""
import hashlib
import json
from pathlib import Path
import re
from urllib.parse import unquote

ROOT = Path(__file__).resolve().parents[2]

def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

def main():
    moves = json.loads((ROOT / 'doc/archive/material-moves.json').read_text())['files']
    for item in moves:
        target = ROOT / item['new']
        assert target.is_file(), item['new']
        assert not (ROOT / item['old']).is_file(), 'Unarchived duplicate: ' + item['old']
        # Only Markdown links/labels may be rewritten during relocation.
        if target.suffix != '.md':
            assert digest(target) == item['sha256_before'], item['new']
    catalog = json.loads((ROOT / 'test & diagnostic/catalog.json').read_text())['files']
    for item in catalog:
        path = ROOT / item['path']
        assert path.stat().st_size == item['bytes'], item['path']
        assert digest(path) == item['sha256'], item['path']
        majors = [int(x) for v in item['versions'] for x in re.findall(r'beta[.]?(\d+)', v)]
        assert not majors or max(majors) >= 10 or item['archived'], item['path']
    sums = (ROOT / 'test & diagnostic/SHA256SUMS').read_text()
    assert sums == ''.join(x['sha256'] + '  ' + x['path'] + '\n' for x in catalog)
    files = [ROOT / 'FB2_FORK_PLAN.md', ROOT / 'AGENTS.md']
    files += list((ROOT / 'doc').rglob('*.md'))
    files += list((ROOT / 'platforms/patch-series').glob('*.md'))
    files += [ROOT / 'test & diagnostic/README.md']
    broken = []
    for path in files:
        if 'archive' in path.relative_to(ROOT).parts:
            continue  # Snapshots can reference files that existed only at their original revision.
        for url in re.findall(r'\]\(([^\n)]+)\)', path.read_text()):
            url = unquote(url.split('#', 1)[0])
            if not url or re.match(r'\w+://|mailto:', url):
                continue
            if not (path.parent / url).exists():
                broken.append(str(path.relative_to(ROOT)) + ': ' + url)
    assert not broken, '\n'.join(broken)
    print('PASS: archival destinations, original payload hashes, catalog/checksums, maintained links')

if __name__ == '__main__':
    main()
