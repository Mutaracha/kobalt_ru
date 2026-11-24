import io
import json
from pathlib import Path
import tarfile
import tempfile
import unittest
import zipfile
from package import collect, require_complete


class PackageTests(unittest.TestCase):
    def test_incomplete_rejected(self):
        for patches in ([{'id': '050', 'status': 'ready'}],
                        [{'id': '060-build-integration', 'status': 'planned'}],
                        [{'id': '060-build-integration', 'status': 'prepared'}]):
            with self.assertRaises(ValueError):
                require_complete({'patches': patches})

    def fixture(self, root, wrong_provenance=False, wrong_mode=False):
        provenance = json.dumps({'commit': 'a' * 40}).encode()
        (root / 'series-provenance.json').write_bytes(provenance)
        target = root / 'build/kobo-qt5'
        target.mkdir(parents=True)
        elf = bytearray(64)
        elf[:6] = b'\x7fELF\x01\x01'
        elf[18:20] = b'\x28\x00'
        buf = io.BytesIO()
        with tarfile.open(fileobj=buf, mode='w:gz') as tar:
            member = tarfile.TarInfo('usr/local/Kobo/imageformats/libkobalt.so')
            member.mode = 0o644 if wrong_mode else 0o755
            member.size = len(elf)
            tar.addfile(member, io.BytesIO(elf))
        with zipfile.ZipFile(target / 'Kobalt-qt5.zip', 'w') as z:
            z.writestr('.kobo/KoboRoot.tgz', buf.getvalue())
            z.writestr('.adds/kobalt/series-provenance.json', b'{}' if wrong_provenance else provenance)
        instructions = root / 'instructions.md'
        instructions.write_text('test instructions')
        return instructions

    def test_collect_and_checksums(self):
        import hashlib
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            instructions = self.fixture(root)
            destination = root / 'artifact'
            collect(root, destination, instructions)
            for line in (destination / 'SHA256SUMS').read_text().splitlines():
                digest, filename = line.split('  ')
                self.assertEqual(digest, hashlib.sha256((destination / filename).read_bytes()).hexdigest())
            self.assertEqual((destination / 'source-commit.txt').read_text().strip(), 'a' * 40)

    def test_bad_installer_rejected(self):
        for flag in ('wrong_provenance', 'wrong_mode'):
            with self.subTest(flag=flag), tempfile.TemporaryDirectory() as tmp:
                root = Path(tmp)
                instructions = self.fixture(root, **{flag: True})
                with self.assertRaises(ValueError):
                    collect(root, root / 'artifact', instructions)


if __name__ == '__main__':
    unittest.main()
