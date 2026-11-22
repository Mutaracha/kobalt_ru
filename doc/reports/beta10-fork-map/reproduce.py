#!/usr/bin/env python3
"""Documentation-only Git inventory; run from the repository root. No build dependency."""
import csv
import json
import subprocess
import sys
from pathlib import Path

UPSTREAM = 'be47258850ffdc3295257475afdf8b8f6cb53176'
FORK = 'ac21d58295c941c3f197718321398c03da33526e'

def tree(ref):
    result = {}
    for entry in subprocess.check_output(['git', 'ls-tree', '-rz', '--full-tree', ref]).split(b'\0'):
        if entry:
            meta, path = entry.split(b'\t', 1)
            result[path.decode()] = tuple(meta.decode().split())
    return result

def category(path):
    if path.startswith(('doc/', 'test & diagnostic/')) or path in ('AGENTS.md', 'FB2_FORK_PLAN.md', 'README.md'):
        return 'reference'
    if path.startswith('tests/'):
        return 'tests'
    if path.startswith('assets/'):
        return 'assets-ru-fonts-icons-notices'
    if path.startswith(('src/support/', 'src/app/about/')):
        return 'locale-about-language'
    if path.startswith(('src/publication/', 'src/engine/')):
        return 'fb2-comics-engine-integration'
    if path.startswith('src/ui/'):
        return 'shared-ui-presentation'
    if path.startswith(('src/app/reader/views/sleep_',)):
        return 'sleep-presentation'
    if path.startswith(('src/app/', 'src/reading/')):
        return 'reader-hooks-settings-navigation'
    if path.startswith('platforms/android/'):
        return 'android-integration'
    if path.startswith('platforms/kobo/'):
        return 'kobo-integration'
    return 'build-tools-platform'

def main():
    output = Path(sys.argv[1] if len(sys.argv) > 1 else 'doc/reports/beta10-fork-map')
    output.mkdir(parents=True, exist_ok=True)
    upstream, fork = tree(UPSTREAM), tree(FORK)
    counts = {}
    different, identical, reference = [], [], []
    with (output / 'all-files.tsv').open('w', newline='') as stream:
        writer = csv.writer(stream, delimiter='\t', lineterminator='\n')
        writer.writerow(['path', 'status', 'category', 'beta10_action', 'upstream_mode', 'upstream_kind', 'upstream_oid', 'fork_mode', 'fork_kind', 'fork_oid'])
        for path in sorted(upstream.keys() | fork.keys()):
            a, b = upstream.get(path), fork.get(path)
            status = 'identical' if a == b else 'fork-only' if a is None else 'upstream-only' if b is None else 'different'
            counts[status] = counts.get(status, 0) + 1
            group = category(path)
            if status == 'identical':
                action = 'TAKE-UPSTREAM: no fork merge; follow beta10 rename/delete/API changes'
                identical.append(path)
            elif group == 'reference':
                action = 'REFERENCE: retain fork evidence; reconcile live guides separately'
                reference.append((path, status))
            elif status == 'fork-only':
                action = 'PORT-FORK-ONLY: check beta10 replacement and API; never overwrite blindly'
                different.append((path, status, group))
            else:
                action = 'MERGE-HUNKS: start with beta10; port approved fork concern only'
                different.append((path, status, group))
            writer.writerow([path, status, group, action, *(a or ('-',)*3), *(b or ('-',)*3)])
    expected = set(subprocess.check_output(['git', 'diff', '--no-renames', '--name-only', '-z', UPSTREAM, FORK]).decode().rstrip('\0').split('\0'))
    assert expected == {p for p, _ in reference} | {p for p, _, _ in different}
    assert len(upstream.keys() | fork.keys()) == len(identical) + len(different) + len(reference)
    (output / 'take-upstream.txt').write_text(''.join(p + '\n' for p in identical))
    (output / 'reference-files.tsv').write_text('path\tstatus\n' + ''.join(p+'\t'+s+'\n' for p,s in reference))
    summary = dict(upstream=UPSTREAM, fork=FORK, counts=counts, runtime_build_assets_tests=len(different), reference_nonidentical=len(reference), total_paths=len(upstream.keys() | fork.keys()))
    (output / 'summary.json').write_text(json.dumps(summary, indent=2) + '\n')
    text = ['# Полный список отличающихся runtime/build/assets/tests файлов\n',
            'Группировка — навигация, не разрешение копировать файл целиком. Один файл может обслуживать несколько доработок. Назначение и проверки — в [карте](README.md).\n']
    for group in sorted({g for _,_,g in different}):
        text += ['\n## ' + group + '\n', '| Файл | Статус |\n|---|---|\n']
        text += ['| `'+p+'` | '+s+' |\n' for p,s,g in different if g == group]
    (output / 'modified-files.md').write_text(''.join(text))
    print(json.dumps(summary, indent=2))

if __name__ == '__main__':
    main()
