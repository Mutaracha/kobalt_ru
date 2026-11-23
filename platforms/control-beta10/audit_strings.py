#!/usr/bin/env python3
"""Report translation drift; never modify translations or silently approve new EN."""
import argparse
from collections import Counter
import json
from pathlib import Path
import re

HERE = Path(__file__).resolve().parent
ENTRY = re.compile(r'\s*"((?:[^"\\]|\\.)*)"\s*=\s*("(?:[^"\\]|\\.)*")\s*;')
COMMENT = re.compile(r'/\*.*?\*/|//[^\n]*', re.S)


def load(path):
    text = Path(path).read_text(encoding='utf-8-sig')
    result = {}
    pos = 0
    while pos < len(text):
        if text[pos].isspace():
            pos += 1
            continue
        comment = COMMENT.match(text, pos)
        if comment:
            pos = comment.end()
            continue
        match = ENTRY.match(text, pos)
        if not match:
            raise ValueError(f'{path}:{text.count(chr(10), 0, pos) + 1}: invalid .strings syntax')
        key = json.loads('"' + match[1] + '"')
        if key in result:
            raise ValueError(f'{path}: duplicate key {key}')
        result[key] = json.loads(match[2])
        pos = match.end()
    return result


def slots(value):
    return Counter(re.findall(r'%(?:L?[1-9][0-9]?|n)', value))


def audit(baseline, english, russian):
    issues = []
    for key, en in sorted(english.items()):
        ru = russian.get(key)
        reasons = []
        if key not in baseline:
            reasons.append('new_english_key')
        elif baseline[key] != en:
            reasons.append('english_changed')
        if ru is None:
            reasons.append('missing_translation')
        elif not ru.strip():
            reasons.append('empty_translation')
        elif slots(en) != slots(ru):
            reasons.append('placeholder_mismatch')
        # beta10 supplies the percent sign in these arguments. Even identical
        # slot counts must not let an inherited "%1%" silently pass.
        if ru is not None and re.search(r'%[1-9][0-9]?%', ru) and not re.search(r'%[1-9][0-9]?%', en):
            reasons.append('literal_percent_suffix_review')
        if reasons:
            issues.append(dict(key=key, reasons=reasons, old_en=baseline.get(key), en=en, ru=ru))
    for key in sorted(russian.keys() - english.keys()):
        issues.append(dict(key=key, reasons=['obsolete_translation'], old_en=baseline.get(key), en=None, ru=russian[key]))
    return issues


def cell(value):
    return (value or '').replace('|', '\\|').replace('\n', '<br>')


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--english', type=Path, required=True, help='assets/en.strings from the target upstream')
    parser.add_argument('--russian', type=Path, default=HERE / 'ru.strings')
    parser.add_argument('--baseline', type=Path, default=HERE / 'en.strings', help='last manually reviewed upstream English')
    parser.add_argument('--report', type=Path, required=True, help='Markdown output, with sibling JSON')
    args = parser.parse_args(argv)
    # Never overwrite any input (including via symlinks).
    inputs = {p.resolve() for p in (args.english, args.russian, args.baseline)}
    outputs = [args.report.resolve(), args.report.with_suffix('.json').resolve()]
    if outputs[0] == outputs[1] or any(p in inputs for p in outputs):
        parser.error('report must be a separate Markdown path, not an input or JSON path')
    try:
        baseline, english, russian = map(load, (args.baseline, args.english, args.russian))
    except (ValueError, OSError) as exc:
        parser.error(str(exc))
    issues = audit(baseline, english, russian)
    data = dict(baseline=str(args.baseline), english=str(args.english), russian=str(args.russian),
                english_keys=len(english), russian_keys=len(russian), issues=issues)
    lines = ['# Проверка русского перевода', '',
             f'EN: {len(english)}; RU: {len(russian)}; строк для проверки: {len(issues)}.', '',
             'Нулевой отчёт означает отсутствие дрейфа ключей/EN/аргументов, не визуальную приёмку.', '',
             '| Ключ | Причина | Прежний EN | Текущий EN | RU |', '|---|---|---|---|---|']
    lines += ['| ' + ' | '.join(map(cell, (e['key'], ', '.join(e['reasons']), e['old_en'], e['en'], e['ru']))) + ' |' for e in issues]
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text('\n'.join(lines) + '\n', encoding='utf-8')
    args.report.with_suffix('.json').write_text(json.dumps(data, ensure_ascii=False, indent=2) + '\n', encoding='utf-8')
    print(f'EN={len(english)} RU={len(russian)} review={len(issues)}; {args.report}')
    return int(bool(issues))


if __name__ == '__main__':
    raise SystemExit(main())
