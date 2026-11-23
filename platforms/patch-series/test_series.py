import copy
import hashlib
import json
import re
from pathlib import Path
import tempfile
import unittest
from unittest.mock import patch

from apply import apply_to, load_series, ROOT


class SeriesTests(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.root = Path(self.tmp.name)
        (self.root / 'one.patch').write_text('fixture')
        self.data = dict(schema=1, policy='ordered-full-stack', upstream='a' * 40, patches=[
            dict(id='010-ru', status='ready', path='one.patch', sha256=hashlib.sha256(b'fixture').hexdigest()),
            dict(id='020-fb2', status='planned')])
        self.manifest = self.root / 'series.json'

    def load(self, data=None):
        self.manifest.write_text(json.dumps(data or self.data))
        return load_series(self.manifest, self.root)

    def test_ready_prefix(self):
        self.assertEqual(self.load()['policy'], 'ordered-full-stack')

    def test_checksum_drift_rejected(self):
        (self.root / 'one.patch').write_text('changed')
        with self.assertRaisesRegex(ValueError, 'checksum'):
            self.load()

    def test_duplicate_or_wrong_order(self):
        for ids in [('010-ru', '010-ru'), ('020-fb2', '010-ru')]:
            data = copy.deepcopy(self.data)
            for entry, identifier in zip(data['patches'], ids):
                entry['id'] = identifier
            with self.assertRaisesRegex(ValueError, 'unique and ordered'):
                self.load(data)

    def test_no_ready_after_gap(self):
        self.data['patches'][0]['status'] = 'planned'
        self.data['patches'][1]['status'] = 'ready'
        with self.assertRaisesRegex(ValueError, 'prefix'):
            self.load()

    def test_reject_path_escape(self):
        self.data['patches'][0]['path'] = '../outside.patch'
        with self.assertRaisesRegex(ValueError, 'escapes'):
            self.load()

    def test_incomplete_stack_rejected_before_git(self):
        self.load()
        with patch('apply.subprocess.check_output') as git:
            with self.assertRaisesRegex(ValueError, 'Full stack not ready'):
                apply_to(self.root, self.manifest, self.root / 'output')
            git.assert_not_called()

    def test_prepared_is_applyable_but_not_accepted(self):
        self.data['patches'][0]['status'] = 'prepared'
        self.load()
        with patch('apply.subprocess.check_output') as git:
            with self.assertRaisesRegex(ValueError, 'Full stack not ready'):
                apply_to(self.root, self.manifest, self.root / 'output')
            git.assert_not_called()

    def test_nested_export_does_not_skip_patches(self):
        build = ROOT / 'build'
        build.mkdir(exist_ok=True)
        with tempfile.TemporaryDirectory(prefix='series-regression-', dir=build) as tmp:
            destination = Path(tmp)
            apply_to(ROOT, ROOT / 'platforms/patch-series/series.json', destination, ready_only=True)
            self.assertEqual((destination / 'assets/ru.strings').read_bytes(),
                             (ROOT / 'platforms/control-beta10/ru.strings').read_bytes())
            self.assertTrue((destination / 'src/publication/fb2_resources.cpp').is_file())
            cmake = (destination / 'platforms/desktop/CMakeLists.txt').read_text()
            # book_fixtures.h expands this macro while compiling fb2_engine.cpp.
            zip_targets = re.search(
                r'if\(check MATCHES "\^\(([^"\n]+)\)\$"\)\s*'
                r'target_compile_definitions\(kobalt-\$\{name\}-tests PRIVATE KOBALT_TEST_ZIP=',
                cmake)
            self.assertIsNotNone(zip_targets, 'Missing ZIP fixture target definitions')
            self.assertIn('fb2_engine', zip_targets.group(1).split('|'))
            self.assertIn('appearance::migrateLegacyNotes(saved);',
                          (destination / 'src/reading/opening_job.cpp').read_text())
            for qt in ('qt5', 'qt6'):
                native = (destination / f'platforms/kobo/nickel-{qt}/native_publication.cpp').read_text()
                self.assertIn('if (fb2)', native)
                self.assertIn('FB2 uses Kobalt', native)
            self.assertIn('files[QString::fromLatin1(Publication::singleFb2ResourceName)]',
                          (destination / 'tests/model/local_resources.cpp').read_text())

    def test_pin_required(self):
        self.data['upstream'] = 'main'
        with self.assertRaisesRegex(ValueError, 'full commit'):
            self.load()


if __name__ == '__main__':
    unittest.main()
