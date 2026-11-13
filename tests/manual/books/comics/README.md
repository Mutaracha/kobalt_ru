# Single-page comics reproduction

Derived from the user-provided `doc/Spice and Wolf v01.kepub.epub/` material
(upload ad349b7). XHTML, stylesheet and PNG are byte-identical. Only package
metadata/spine/navigation were reduced to remove references to deleted files.
The artwork is user-supplied diagnostic evidence, not a public-domain asset.
Automated `engine.comics` uses synthetic artwork instead.

Manual Kobo Qt5 checks: mode off with margins/chrome zero; mode on; rotate;
edit margins and header/footer while on (controls remain available, graphic
viewport unchanged); apply; sleep/reopen; disable mode and check the latest
saved values return. Check the lower edge against the original Nickel image,
without cropping natural image borders or distorting its aspect ratio.

EPUB SHA256: `5476b8256ca1206b112171cd0d1b8d6ce0b496bedce21966f5e9ee7266573145`.

Исходный unpacked upload удалён из текущего дерева после разбора; доступен
в истории: commit `bc07d8b0b60f354ac6892f0496e0ac0434590062`, путь
`doc/Spice and Wolf v01.kepub.epub/`. Сохранённый manual EPUB и screenshots
не изменялись. Итоги анализа — в `FB2_FORK_PLAN.md` (корень репозитория).
