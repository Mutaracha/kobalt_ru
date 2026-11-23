import json
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest

import audit_strings as a


class AuditTests(unittest.TestCase):
    def test_baseline(self):
        self.assertEqual(a.audit({'a': 'Hello %1'}, {'a': 'Hello %1'}, {'a': 'Привет %1'}), [])

    def test_added_key_even_when_ru_present(self):
        self.assertIn('new_english_key', a.audit({}, {'a': 'New'}, {'a': 'Новый'})[0]['reasons'])

    def test_changed_english_same_slots(self):
        self.assertIn('english_changed', a.audit({'a': 'Screen %1'}, {'a': 'Page %1'}, {'a': 'Экран %1'})[0]['reasons'])

    def test_missing_empty_removed(self):
        issues = a.audit({}, {'a': 'A', 'b': 'B'}, {'b': '', 'c': 'C'})
        self.assertIn('missing_translation', issues[0]['reasons'])
        self.assertIn('empty_translation', issues[1]['reasons'])
        self.assertEqual(issues[2]['reasons'], ['obsolete_translation'])

    def test_slot_multiplicity_and_localized_slots(self):
        self.assertNotEqual(a.slots('%1 %1 %L2'), a.slots('%1 %2'))
        self.assertEqual(a.slots('%2 %1'), a.slots('%1 %2'))

    def test_percent_contract(self):
        self.assertIn('literal_percent_suffix_review', a.audit({'a': '%1'}, {'a': '%1'}, {'a': '%1%'})[0]['reasons'])

    def test_parser_comments_escapes_and_errors(self):
        with tempfile.TemporaryDirectory() as tmp:
            p = Path(tmp) / 'a.strings'
            p.write_text('/* comment */ "a" = "line\\n\\\"quote\\\" // value"; // end\n')
            self.assertEqual(a.load(p)['a'], 'line\n"quote" // value')
            p.write_text('"a" = "a"; "a" = "b";')
            with self.assertRaisesRegex(ValueError, 'duplicate'):
                a.load(p)
            p.write_text('"a" = "a"; broken')
            with self.assertRaisesRegex(ValueError, 'invalid'):
                a.load(p)

    def test_cli_report_and_exit_code(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            en, ru, report = (root / p for p in ('en.strings', 'ru.strings', 'report.md'))
            en.write_text('"a" = "A %1";')
            ru.write_text('"a" = "А %1";')
            args = [sys.executable, str(a.HERE / 'audit_strings.py'), '--english', str(en), '--baseline', str(en), '--russian', str(ru), '--report', str(report)]
            self.assertEqual(subprocess.run(args, capture_output=True).returncode, 0)
            ru.write_text('"a" = "А %2";')
            self.assertEqual(subprocess.run(args, capture_output=True).returncode, 1)
            self.assertIn('placeholder_mismatch', json.loads(report.with_suffix('.json').read_text())['issues'][0]['reasons'])
            before = en.read_bytes()
            args[-1] = str(en)
            self.assertEqual(subprocess.run(args, capture_output=True).returncode, 2)
            self.assertEqual(en.read_bytes(), before)


if __name__ == '__main__':
    unittest.main()
