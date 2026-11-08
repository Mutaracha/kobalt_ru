# Разовая сборка Kobo Qt6

Qt5 остаётся основной целью разработки и автоматического CI этого форка.
Qt6 запускается только явно: workflow `Kobo qt5 device build`, ручной запуск
на текущей ветке с `build_qt6_once=true`. При таком запуске обычные Qt5/native
jobs пропускаются; Qt6 job выполняет offline checks, штатные adapter checks,
сборку и проверку пакета через `platforms/ci/build.sh qt6`.

Артефакт — `Kobalt-qt6-once`: `Kobalt-qt6.zip` и `build-qt6.json` с SHA кода,
SDK и SHA-256 ZIP. Установщик внутри — `.kobo/Kobo.tgz`, не Qt5 `KoboRoot.tgz`.
Пакет предназначен только для совместимой Qt6-прошивки Kobo, не для 4.x/Qt5.
Перед установкой закрыть Kobalt и сохранить backup всей `.adds/kobalt/`.
Сборка/проверки адаптеров не заменяют проверку на физическом устройстве.

Результат конкретного запуска и ссылка на пакет записываются в
`FB2_FORK_PLAN.md`. Регулярные push по-прежнему не запускают Qt6.

Если workflow_dispatch недоступен подключению, явно разрешённый разовый push
с маркером `[qt6-once]` в commit message запускает тот же Qt6-only job. Обычные
push без этого маркера остаются Qt5-only. Не использовать маркер для docs-only
обновлений результата. Первый dispatch получил 403; это ограничение разрешений
Actions, а не результат сборки.

## Полученный пакет

- Код: `44b6f882e8235316580184cfc72b028a82ab0c97`.
- [CI 42507680309 — PASS](https://github.com/Mutaracha/kobalt_ru/actions/runs/42507680309).
- [Скачать Kobalt-qt6-once](https://github.com/Mutaracha/kobalt_ru/actions/runs/42507680309/artifacts/13628387057).
- Внутри: Kobalt-qt6.zip и build-qt6.json; SHA-256 ZIP проверен CI и записан
  в JSON. Размер внешнего artifact: 36 517 352 bytes.
- Offline/Qt6 adapter checks/build/package/metadata/checksum PASS. Изменения
  application source для этой сборки не потребовались. Физическое устройство
  Qt6 не проверено; дальнейшая работа и обычные push остаются Qt5.
- Предшествующий обычный Qt5/native прогон 42507658383 также PASS.
