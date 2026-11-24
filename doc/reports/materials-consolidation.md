# Консолидация материалов на beta10

Baseline документации: `25aac587f7ede39b5f2a89712ac7380501d1ae82`.
Нового runtime/пакета нет: актуальный binary остаётся **eafb6161**, CI45456569246.
Даты в сохранённых логах — показания устройства, не дата консолидации.

## Результат

- Накопительные FB2_FORK_PLAN/fork-decisions/fork-delta сохранены в mixed-history;
  активные версии переписаны как короткая очередь, действующие решения и карта серии.
- beta9/8 и более старые отчёты/чеклисты/публичный changelog, beta7–9 diagnostics,
  соответствующие screenshots/inputs и beta5 evaluation data перенесены в архив.
  Переходная диагностика 08c66e4d содержит обе базы и оставлена целиком с пометкой.
- Закрытые исключения вынесены из плана в retired-decisions; не являются backlog.
  Штатные функции beta10 и F07 checkable screens не объявлены исключёнными.
- Для beta10 создан единый индекс результатов, актуальный device checklist;
  обновлены инструкция форка, AGENTS, серия/протокол, сноски и ссылки.
- Исторические beta10 отчёты сохранены как снимки по SHA с указателем на текущий
  журнал. Их старый pending не заменяет новое состояние. Входные diff/inventory
  TSV/JSON не перегенерировались под другую базу и сохраняют исходное provenance.
- Тестовые книги/portable fixtures/current regression tests сохранены; универсальный
  opt-in tests/manual/books не является результатом старой сборки и не перемещён.

## Проверки

`python3 doc/tools/check-materials.py` на целостном export текущей ветки — PASS: все destinations существуют,
старых дубликатов moved paths нет, payload hashes совпадают, beta9-or-earlier
startup data не остаются вне архива (кроме смешанного переходного ZIP с beta10),
каталог согласован с checksum-файлом, maintained Markdown links разрешаются.
`sha256sum -c 'test & diagnostic/SHA256SUMS'` из корня — PASS.

[Манифест переносов/снимков](../archive/material-moves.json) фиксирует old/new,
исходные hashes и причины. Markdown links и архивные labels могли обновляться;
ZIP/images/books/fonts/JSONL и другие немаркированные payloads не изменялись.
[Каталог данных](../../test%20%26%20diagnostic/catalog.json) фиксирует source/version,
размер/hash и принадлежность архиву. Проверки CI/runtime повторно не запускались:
это docs/data-only изменение, не новая аппаратная приёмка.

Среда: retained root checkout содержит старое дерево исходников и посторонние
исторические документы; проверка всех ссылок непосредственно в нём показывает
отсутствующие newer engine paths/старые untracked документы. Их не удаляли и
не подменяли исходники ради docs-проверки. На целостном export проверка проходит;
контрольные суммы данных проходят также в root checkout.
