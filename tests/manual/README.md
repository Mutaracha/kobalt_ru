# Ручные тестовые материалы форка

Это исходные материалы пользователя, не generated gallery и не portable fixtures.
Текущий статус проверки хранится в [`FB2_FORK_PLAN.md`](../../FB2_FORK_PLAN.md).

## Расположение и происхождение

| Папка | Происхождение и назначение |
|---|---|
| `screenshots/kobo-qt5/ab6d9a0/` | `screen_001.png` … `screen_044.png`, добавлены пользователем в commit `093dd4d0d97c6a2be57a09c022c7282aa08cd725`. Бинарная сборка **ab6d9a0052bd87b1aba25daa71c22a4f7956f5f7**, beta6, а не beta7 и не бинарник 093dd4d. Полная полученная серия меню/диалогов; карта просмотра — в плане. |
| `screenshots/kobo-qt5/2026-11-06/` | Более ранние вложения после `5b1164d`, нижние FB2-сноски/Свет/Стили; README различает старый ориентир и дефект unskinned row. Не эталон текущей сборки. |
| `screenshots/kobo-qt5/legacy-ru/` | Старые RU окна, theme/swipe regression и подтверждённый словарь. `index.md` различает исходные, ошибочные и подтверждённые состояния. Не смешивать с ab6d9a0. |
| `books/publisher/Кокаиновые+короли.epub` | Неизменённый пользовательский EPUB из commit `02953e673bcd371c7be26ad6ead4aa5894b51b9b`; EPUB 2 / InDesign, отдельная проверка `_idFootnotes > _idFootnote`. Не общественное достояние и не новая общая fixture. |

Ранее материалы лежали в `doc/screenshots/{ab6d9a0,device-2026-11-06,kobo-ru}`
и `doc/Кокаиновые+короли.epub`. При переносе PNG/EPUB сохранены byte-for-byte.
Полный список SHA-256 — [`materials.sha256`](materials.sha256). Проверка из корня:

```sh
sha256sum -c tests/manual/materials.sha256
```

Указанный на About в снимках UTC timestamp — метаданные присланной сборки,
а не дата новой проверки или сборки beta7. Снимки не редактировать; новые серии
помещать в новую папку по бинарному SHA с provenance и обновлением manifest.
Контактные листы для аудита — scratch, не замена оригиналам и не новые эталоны.

## Использование книги

`tests/model/links.cpp --publisher-notes PATH` — отдельный opt-in regression
по исходному EPUB (реальные source notes 4/5, bottom fragments/paint/off/popup).
В `.github/workflows/kobo-qt5-device.yml` путь теперь
`/work/tests/manual/books/publisher/Кокаиновые+короли.epub` внутри SDK container;
сохраняются parent-owned temporary root, headless environment и timeout.
Перенос пути сам по себе не является повторным прохождением этого CI.

Не делать коммерческую книгу обязательной зависимостью общего native suite,
не копировать её текст/шрифты в новые synthetic fixtures/runtime assets и не
включать в приложение/пакет. Portable synthetic и public-domain fixtures остаются
в `tests/fixtures/`; сгенерированные captures и build outputs — в ignored `build/`.
Наличие файла в Git не означает разрешения на переиздание книги.
