# Пользовательские тестовые и диагностические данные

Каталог создан пользователем в commit `64930c0`. Новые device uploads, сырые
логи, скриншоты и opt-in книги складывать сюда, не в `doc/`. Документация и
итоги разбора остаются в `doc/` / `FB2_FORK_PLAN.md`. Только данные из этой
папки не запускают сборку; mixed code+data и workflow_dispatch работают.

- `diagnostics 61dbfaa.zip` — upload 64930c0, runtime из startup `61dbfaa`;
  содержит историю до принудительной перезагрузки и новый запуск после неё.
  Не путать с более ранним одноимённым архивом из bc07d8b.
- `61dbfaa/` — upload a9a7660: уменьшенный пользовательский EPUB и screen_001 /
  screen_004. EPUB spine явно помечает вторую страницу `rendition:page-spread-center`.
  Bitmap orientation сама по себе этого не отражает. Synthetic regression в
  `tests/model/comics.cpp`; copyrighted art не включается в сборку/CI fixtures.

Автоматические переносимые fixtures — `tests/fixtures/`; существующие curated
manual materials и checksums остаются в `tests/manual/`. В текущем `doc/` сырых
тестовых/диагностических файлов после предыдущей очистки уже нет; удалённые
оригиналы доступны в Git history bc07d8b, vendor binaries не восстанавливаем.
Архивы не перепаковывать, изображения и книги не перекодировать.
