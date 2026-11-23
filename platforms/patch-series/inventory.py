#!/usr/bin/env python3
"""Reproducible old-fork delta inventory; stage hints are NOT hunk-level approval."""
import argparse
import csv
import json
from pathlib import Path
import subprocess

OLD = 'be47258850ffdc3295257475afdf8b8f6cb53176'


def tree(root, ref):
    out = {}
    for entry in subprocess.check_output(['git', '-C', str(root), 'ls-tree', '-rz', '--full-tree', ref]).split(b'\0'):
        if entry:
            meta, path = entry.split(b'\t', 1)
            out[path.decode()] = tuple(meta.decode().split())
    return out


def hints(path):
    if path.startswith(('doc/', 'test & diagnostic/')) or path in ('README.md', 'AGENTS.md', 'FB2_FORK_PLAN.md'):
        return 'reference', 'REFERENCE-ONLY'
    if path == 'platforms/kobo/common/page_button_mapping.h':
        return 'removed', 'DROP: user verified upstream invert_landscape replaces Libra2 mapping'
    stages = []
    if any(x in path for x in ('fb2', 'publication/', 'reader_source', 'reader_fork_styles', 'reader_styles', 'footnote', 'reader_links', 'reader_alignment', 'reader_beautifier', 'opening_job', 'native_publication', 'book_menu')):
        stages.append('020-fb2')
    if any(x in path for x in ('comic', 'column_view', 'page_draw_buffer', 'figure_layout', 'alternate_layout')):
        stages.append('030-comics')
    if path.endswith('/refresh.cpp') and '/nickel-qt5/' in path:
        stages.append('030-comics')
    if any(x in path for x in ('navigation', 'fork_reader_actions', 'reader_build', 'reader_refresh', 'reader_settings', 'reading_page', 'reader_p.h', 'page_navigation', 'features.h')):
        stages.append('040-reader-controls')
    if path.startswith(('src/support/', 'src/ui/', 'src/app/about/', 'assets/')) or any(x in path for x in ('sleep_', 'debug_language', 'annotations', 'text_preset')):
        stages.append('050-fork-presentation')
    if path.startswith(('.github/', 'platforms/ci/')) or path in ('.clang-format-ignore', '.gitignore'):
        stages.append('060-build-integration')
    if not stages:
        stages = ['020-fb2..060-build-integration:shared-review']
    return ','.join(stages), 'REVIEW-HUNKS: do not copy whole files'


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--repo', type=Path, default=Path(__file__).resolve().parents[2])
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    config = json.loads((args.repo / 'platforms/patch-series/series.json').read_text())
    ru = json.loads((args.repo / 'platforms/control-beta10/manifest.json').read_text())
    ru_paths = {e['path'] for e in ru['files']}
    before, fork, target = [tree(args.repo, ref) for ref in (OLD, config['fork_source'], config['upstream'])]
    counts = {}
    args.output.mkdir(parents=True, exist_ok=True)
    with (args.output / 'inventory.tsv').open('w') as stream:
        writer = csv.writer(stream, delimiter='\t', lineterminator='\n')
        writer.writerow(['path', 'stage_hints_NOT_approval', 'decision', 'also_touched_by_010_ru', 'upstream_changed_since_beta9', 'old_base_oid', 'fork_oid', 'beta10_oid'])
        for path in sorted(before.keys() | fork.keys()):
            a, b, c = before.get(path), fork.get(path), target.get(path)
            if a == b:
                continue
            stage, decision = hints(path)
            if b == c:
                decision = 'TAKE-UPSTREAM: exact tree entry already matches'
            counts[decision.split(':')[0]] = counts.get(decision.split(':')[0], 0) + 1
            oid = lambda value: value[2] if value else '-'
            writer.writerow([path, stage, decision, path in ru_paths, a != c, oid(a), oid(b), oid(c)])
    summary = dict(old_upstream=OLD, fork_source=config['fork_source'], target_upstream=config['upstream'], counts=counts,
                   warning='Path-level candidate hints only. Shared files require hunk-level review; this is not a completed port.')
    (args.output / 'summary.json').write_text(json.dumps(summary, indent=2) + '\n')
    print(json.dumps(summary, indent=2))


if __name__ == '__main__':
    main()
