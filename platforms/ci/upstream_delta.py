#!/usr/bin/env python3
"""Compare immutable Git trees, never the working directory. No CMake dependency."""
import argparse
import collections
import csv
import io
import json
from pathlib import Path
import subprocess


def git(*args):
    return subprocess.check_output(['git', *args])


def tree(ref):
    result = {}
    for entry in git('ls-tree', '-rz', '--full-tree', ref).split(b'\0'):
        if entry:
            meta, path = entry.split(b'\t', 1)
            mode, kind, oid = meta.decode().split()
            result[path.decode()] = (mode, kind, oid)
    return result


def disposition(path, status):
    # Ownership hints are deliberately conservative, not a semantic approval.
    if path.startswith(('doc/', 'test & diagnostic/')) or path in ('AGENTS.md', 'FB2_FORK_PLAN.md'):
        return 'documentation-evidence', 'REFERENCE', 'Keep useful fork history; do not import as runtime.'
    if path.startswith('tests/'):
        return 'tests-fixtures', 'REVIEW', 'Merge upstream tests and register targets; retain FB2/comics/RU coverage; do not weaken assertions.'
    if path.startswith('src/engine/'):
        return 'engine-fb2-comics-hybrid', 'REVIEW-PROTECTED', 'Inspect hunks: preserve approved FB2/comics; do not change base EPUB Hybrid algorithms in this alignment.'
    if path.startswith(('src/app/reader/views/reading_page_progress.',)):
        return 'progress-bar', 'REPLACE-BETA10', 'Delete fork implementation when porting 4a9576ae017684d290dd2311f4f5195e5e043274; no parallel renderer; D04.'
    if path.startswith('src/app/reader/'):
        return 'reader-consumers', 'REVIEW', 'Separate upstream behavior from approved fork UI; resolve missing consumers/dependencies together.'
    if path.startswith('platforms/kobo/'):
        return 'kobo-integration', 'REVIEW', 'Qt5 priority; preserve FB2 routing, locale, Libra2 polarity and accepted waveform; upstream owns common policy.'
    if path.startswith('platforms/android/'):
        return 'android-integration', 'REVIEW', 'Merge upstream brightness/picker/lifecycle consumers as a whole; preserve shared portability.'
    if path.startswith(('platforms/', '.github/')):
        return 'build-platform', 'REVIEW', 'Preserve Qt5 package/provenance and RU generation; import upstream dependency/test wiring contextually.'
    if path.startswith('assets/'):
        return 'assets-localization', 'REVIEW', 'Preserve approved RU/fonts/icons and notices; audit new upstream keys and licenses; wording approval required.'
    if path.startswith(('src/support/', 'src/publication/')):
        return 'locale-publication-support', 'REVIEW', 'Keep focused RU/FB2 hooks; upstream owns shared infrastructure; inspect every hunk.'
    if path.startswith('src/scenario/'):
        return 'scenario-reporting', 'REVIEW', 'Import reporting dependencies and tests together; never classify missing instrumentation as cosmetic.'
    return 'shared-runtime-other', 'REVIEW', 'Inspect every hunk; file presence or a green build is not semantic acceptance.'


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--upstream', required=True)
    parser.add_argument('--fork', required=True)
    parser.add_argument('--output', required=True)
    args = parser.parse_args()
    upstream = git('rev-parse', args.upstream + '^{commit}').decode().strip()
    fork = git('rev-parse', args.fork + '^{commit}').decode().strip()
    left, right = tree(upstream), tree(fork)
    counts, groups, rows = collections.Counter(), collections.Counter(), []
    for path in sorted(left.keys() | right.keys()):
        a, b = left.get(path), right.get(path)
        status = 'fork-only' if a is None else 'upstream-only' if b is None else 'identical' if a == b else 'different'
        counts[status] += 1
        if status == 'identical':
            continue
        group, review, action = disposition(path, status)
        groups[group] += 1
        rows.append([status, path, group, review, *(a or ('-', '-', '-')), *(b or ('-', '-', '-')), action])
    output = Path(args.output)
    output.mkdir(parents=True, exist_ok=True)
    stream = io.StringIO(newline='')
    writer = csv.writer(stream, delimiter='\t', lineterminator='\n')
    writer.writerow(['status', 'path', 'group', 'review', 'upstream_mode', 'upstream_kind', 'upstream_oid', 'fork_mode', 'fork_kind', 'fork_oid', 'beta10_action'])
    writer.writerows(rows)
    (output / 'files.tsv').write_text(stream.getvalue(), encoding='utf-8')
    summary = dict(upstream=upstream, fork=fork, counts=dict(sorted(counts.items())), groups=dict(sorted(groups.items())), nonidentical=len(rows), classification='Conservative ownership hints; REVIEW is not semantic acceptance. Includes all tracked paths, modes and gitlinks.')
    (output / 'summary.json').write_text(json.dumps(summary, indent=2) + '\n', encoding='utf-8')
    # Git patch is reproducible on demand; avoid vendoring large fixtures into a report.
    (output / 'changes.stat').write_bytes(git('diff', '--no-ext-diff', '--no-renames', '--stat', upstream, fork, '--'))
    print(json.dumps(summary, indent=2))


if __name__ == '__main__':
    main()
