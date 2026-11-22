#!/usr/bin/env python3
"""Offline regression checks for immutable-tree delta reporting."""
import csv
import importlib.util
import json
from pathlib import Path
import subprocess
import tempfile
import unittest

SCRIPT = Path(__file__).with_name('upstream_delta.py').resolve()
spec = importlib.util.spec_from_file_location('delta', SCRIPT)
delta = importlib.util.module_from_spec(spec)
spec.loader.exec_module(delta)


class DeltaTests(unittest.TestCase):
    def test_dispositions_are_conservative(self):
        self.assertEqual(delta.disposition('src/engine/new.cpp', 'upstream-only')[1], 'REVIEW-PROTECTED')
        self.assertEqual(delta.disposition('src/new.cpp', 'different')[1], 'REVIEW')
        self.assertEqual(delta.disposition('tests/a.cpp', 'fork-only')[1], 'REVIEW')

    def test_exact_trees_and_repeatability(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            def git(*args):
                return subprocess.check_output(['git', '-C', tmp, *args])
            git('init', '-q')
            git('config', 'user.name', 'Test')
            git('config', 'user.email', 'test@example.invalid')
            for name, content in [('same', b'unchanged'), ('modified', b'old'), ('removed', b'x'), ('mode', b'x'), ('binary', b'\x00a')]:
                (root / name).write_bytes(content)
            git('add', '.')
            git('-c', 'core.hooksPath=/dev/null', 'commit', '-qm', 'upstream')
            upstream = git('rev-parse', 'HEAD').decode().strip()
            (root / 'modified').write_bytes(b'new')
            (root / 'binary').write_bytes(b'\x00b')
            (root / 'removed').unlink()
            (root / 'added with space').write_bytes(b'new')
            git('add', '-A')
            git('update-index', '--chmod=+x', 'mode')
            git('-c', 'core.hooksPath=/dev/null', 'commit', '-qm', 'fork')
            fork = git('rev-parse', 'HEAD').decode().strip()
            (root / 'modified').write_bytes(b'dirty ignored')
            args = ['python3', str(SCRIPT), '--upstream', upstream, '--fork', fork, '--output']
            subprocess.check_output(args + [str(root / 'out1')], cwd=tmp)
            subprocess.check_output(args + [str(root / 'out2')], cwd=tmp)
            for file in ('files.tsv', 'summary.json', 'changes.stat'):
                self.assertEqual((root / 'out1' / file).read_bytes(), (root / 'out2' / file).read_bytes())
            summary = json.loads((root / 'out1/summary.json').read_text())
            self.assertEqual(summary['counts'], {'identical': 1, 'different': 3, 'fork-only': 1, 'upstream-only': 1})
            with (root / 'out1/files.tsv').open() as handle:
                rows = {r['path']: r for r in csv.DictReader(handle, delimiter='\t')}
            self.assertEqual(rows['mode']['fork_mode'], '100755')
            self.assertEqual(rows['removed']['status'], 'upstream-only')
            self.assertEqual(rows['added with space']['status'], 'fork-only')
            self.assertNotEqual(rows['binary']['upstream_oid'], rows['binary']['fork_oid'])


if __name__ == '__main__':
    unittest.main()
