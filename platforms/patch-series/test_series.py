import copy
import hashlib
import json
import re
import subprocess
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
            # Later ordered patches may append new translated UI strings; the reviewed
            # 010 overlay must remain intact, rather than suppressing those additions.
            overlay = (ROOT / 'platforms/control-beta10/ru.strings').read_text()
            actual = (destination / 'assets/ru.strings').read_text()
            # 050 generalizes singular forms from literal 1 to the CLDR count
            # slot (21, 101, ...). All other reviewed 010 entries stay intact.
            plural_updates = {
                'library.count_one': '"library.count_one" = "%1 книга";',
                'annotations.summary_one': '"annotations.summary_one" = "%1 аннотация";',
                'chapter_contents.summary_one': '"chapter_contents.summary_one" = "Содержание · %1 глава";',
            }
            for line in overlay.splitlines():
                match = re.match(r'^"([^"\n]+)"\s*=', line)
                if match:
                    self.assertIn(plural_updates.get(match.group(1), line), actual)
            for language in ('en', 'ru'):
                strings = (destination / f'assets/{language}.strings').read_text()
                self.assertEqual(strings.count('"reader.comics_reading_mode" ='), 1)
                self.assertEqual(strings.count('"reader.comics_reading_mode_description" ='), 1)
            self.assertTrue((destination / 'src/publication/fb2_resources.cpp').is_file())
            # The mixed RU/EN pattern is explicitly excluded from the full port.
            self.assertFalse((destination / 'assets/hyph/Russian_EnUS.pattern').exists())
            self.assertTrue((destination / 'assets/hyph/Russian.pattern').is_file())
            cmake = (destination / 'platforms/desktop/CMakeLists.txt').read_text()
            # book_fixtures.h expands this macro while compiling fb2_engine.cpp.
            zip_targets = re.search(
                r'if\(check MATCHES "\^\(([^"\n]+)\)\$"\)\s*'
                r'target_compile_definitions\(kobalt-\$\{name\}-tests PRIVATE KOBALT_TEST_ZIP=',
                cmake)
            self.assertIsNotNone(zip_targets, 'Missing ZIP fixture target definitions')
            self.assertIn('fb2_engine', zip_targets.group(1).split('|'))
            self.assertIn('comics_layout', zip_targets.group(1).split('|'))
            column_view = (destination / 'src/engine/blaze/adapter/column_view.h').read_text()
            self.assertIn('chapterColumns()', column_view)
            self.assertIn('void rebuildComicSpreads();', column_view)
            self.assertNotIn('Position restore(', column_view)
            self.assertTrue((destination / 'src/engine/blaze/adapter/reader_comics.cpp').is_file())
            shared_buffer = (destination / 'src/engine/blaze/adapter/page_draw_buffer.h').read_text()
            self.assertIn('_rowsize = image.bytesPerLine();', shared_buffer)
            self.assertIn('OnTableDrawn()', shared_buffer)
            self.assertIn('frameImage()', (destination / 'src/reading/reader_session.cpp').read_text())
            refresh = (destination / 'platforms/kobo/nickel-qt5/refresh.cpp').read_text()
            self.assertIn('clean || graphics ? repairPolicy(dark) : ordinaryPolicy(dark)', refresh)
            controls = (destination / 'src/app/reader/window/reader_build.cpp').read_text()
            self.assertIn('rrComicsToggle', controls)
            self.assertIn('rrDarkTheme', controls)
            self.assertIn('pageNavigation::reverseSwipesKey', controls)
            self.assertNotIn('invert_page_swipes', controls)
            features = (destination / 'src/reading/features.h').read_text()
            self.assertIn('HighlightLinks, Activity, DarkMode, Comics', features)
            self.assertIn('reader_controls', cmake)


            self.assertIn('appearance::migrateLegacyNotes(saved);',
                          (destination / 'src/reading/opening_job.cpp').read_text())
            for qt in ('qt5', 'qt6'):
                native = (destination / f'platforms/kobo/nickel-{qt}/native_publication.cpp').read_text()
                self.assertIn('if (fb2)', native)
                self.assertIn('FB2 uses Kobalt', native)
            self.assertIn('files[QString::fromLatin1(Publication::singleFb2ResourceName)]',
                          (destination / 'tests/model/local_resources.cpp').read_text())

    def test_compile_workflow_shell_syntax(self):
        workflow = (ROOT / '.github/workflows/patch-series.yml').read_text()
        marker = '          set -euo pipefail\n'
        self.assertEqual(workflow.count(marker), 1)
        script = marker + workflow.split(marker, 1)[1].split('\n      - name:', 1)[0]
        script = '\n'.join(line[10:] for line in script.splitlines())
        result = subprocess.run(['bash', '-n'], input=script, text=True, capture_output=True)
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertIn('--no-tests=error', script)
        self.assertIn('build/prepared-prefix/build/kobo-qt5/Kobalt-qt5-diagnostic.zip', workflow)
        self.assertIn('name: Beta10-series-prefix-qt5', workflow)
        self.assertIn('test-only-prefix-not-full-fork', workflow)

    def test_pin_required(self):
        self.data['upstream'] = 'main'
        with self.assertRaisesRegex(ValueError, 'full commit'):
            self.load()


if __name__ == '__main__':
    unittest.main()
