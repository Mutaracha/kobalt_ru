#!/usr/bin/env python3
"""Audit actual cmap coverage, not OS/2 script claims. Requires fonttools.
Usage: python audit-cyrillic.py FONT_ROOT [FONT_ROOT ...] > evidence.json
"""
import hashlib,json,sys
from pathlib import Path
from fontTools.ttLib import TTFont, TTCollection
RUSSIAN='АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя'
result=[]
for root in sys.argv[1:]:
 for p in sorted(Path(root).rglob('*')):
  if p.suffix.lower() not in ('.ttf','.otf','.ttc'): continue
  faces=TTCollection(p,lazy=False).fonts if p.suffix.lower()=='.ttc' else [TTFont(p,lazy=False)]
  for index,font in enumerate(faces):
   cmap=font.getBestCmap() or {}
   result.append(dict(path=str(p),face=index,family=font['name'].getDebugName(1),
                      style=font['name'].getDebugName(2),sha256=hashlib.sha256(p.read_bytes()).hexdigest(),
                      russian_66=sum(ord(c) in cmap for c in RUSSIAN),
                      missing_russian=''.join(c for c in RUSSIAN if ord(c) not in cmap),
                      cyrillic_0400_052f=sum(c in cmap for c in range(0x400,0x530))))
   font.close()
print(json.dumps(dict(criterion='All 66 Russian letters including Ёё; not a claim of all Cyrillic languages',fonts=result),ensure_ascii=False,indent=2))
