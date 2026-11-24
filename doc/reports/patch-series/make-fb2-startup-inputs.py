#!/usr/bin/env python3
"""Make independent cold inputs without overwriting user books or clearing caches."""
import argparse
import hashlib
import json
from pathlib import Path
import xml.etree.ElementTree as ET
from zipfile import ZipFile, ZIP_DEFLATED

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('output', type=Path, help='new output ZIP (must not exist)')
args = parser.parse_args()
root = Path(__file__).resolve().parents[3]
books = root / 'test & diagnostic'
paths = [books / '1. Виртуальный свет.fb2',
         books / 'Sorokin_Norma-Tridcataya-lyubov-Mariny-Goluboe-salo-Den-oprichnika-Saharnyy-Kreml.3nbKGQ.593957.fb2']
records = []
args.output.parent.mkdir(parents=True, exist_ok=True)
with ZipFile(args.output, 'x', ZIP_DEFLATED) as archive:
    for index, path in enumerate(paths, 1):
        original = path.read_bytes()
        ET.fromstring(original)
        for mode in ('HYBRID', 'ACCURATE'):
            name = f'034-{index}-{mode}.fb2'
            data = original + (f'\n<!-- Kobalt FB2 startup experiment 034 cold input '
                               f'{index} {mode} run A -->\n').encode()
            ET.fromstring(data)
            archive.writestr(name, data)
            records.append(dict(file=name, sha256=hashlib.sha256(data).hexdigest(),
                                original=path.name))
    archive.writestr('inputs.json', json.dumps(records, ensure_ascii=False, indent=2) + '\n')
    archive.writestr('README.txt',
        'Это тестовые книги, НЕ установочный пакет.\n'
        'Распакуйте FB2 в обычную папку книг; оригиналы не заменять.\n'
        'Для HYBRID выберите Hybrid, для ACCURATE — Accurate в Settings.\n'
        'Каждую копию впервые открывайте только в соответствующем режиме.\n'
        'Внутренние названия книг прежние; различайте файлы по суффиксам.\n'
        'Изменение содержимого: только уникальный XML-комментарий после корня.\n'
        'После 3–5 перелистываний Hybrid дайте фону 30–60 секунд, затем close/reopen.\n'
        'Приложите diagnostics ZIP и порядок открытий.\n')
print(args.output)
