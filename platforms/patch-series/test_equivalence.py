"""Guard the consolidation comparison against missing/extra/changed files and modes."""
import json
from pathlib import Path
import tempfile
import unittest
from verify_equivalence import tree, verify


class EquivalenceTests(unittest.TestCase):
    def test_bytes_paths_and_executable_mode(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp) / 'runtime'
            root.mkdir()
            file = root / 'font.bin'
            file.write_bytes(b'original\0bytes')
            reference = Path(tmp) / 'reference.json'
            reference.write_text(json.dumps({'files': tree(root)}))
            verify(root, reference)
            file.write_bytes(b'changed!\0bytes')
            with self.assertRaises(ValueError):
                verify(root, reference)
            file.write_bytes(b'original\0bytes')
            file.chmod(file.stat().st_mode | 0o111)
            with self.assertRaises(ValueError):
                verify(root, reference)
            file.chmod(file.stat().st_mode & ~0o111)
            extra = root / 'extra'
            extra.write_bytes(b'')
            with self.assertRaises(ValueError):
                verify(root, reference)
            extra.unlink()
            file.unlink()
            with self.assertRaises(ValueError):
                verify(root, reference)

    def test_symlink_identity(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp) / 'runtime'
            root.mkdir()
            link = root / 'link'
            link.symlink_to('first-missing-target')
            reference = Path(tmp) / 'reference.json'
            reference.write_text(json.dumps({'files': tree(root)}))
            verify(root, reference)
            link.unlink()
            link.symlink_to('second-missing-target')
            with self.assertRaises(ValueError):
                verify(root, reference)
