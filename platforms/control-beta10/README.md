# Переносимый русский overlay для оригинальной beta10

База: upstream `1796a32559dbb2912bd565990d1c7a0ca7ac4e52`.
Источник: контрольный overlay из `8bd576f144069de7eac7a21fbbc79ce602486c93`,
не runtime основного форка. Последующее выравнивание: [отчёт](../../doc/reports/control-beta10/translation-review.md).

## Состав и ограничения

`ru.patch` — **исходный git binary patch**, не устанавливаемый патч прошивки и не ZIP.
Включает 36 файлов из исходного manifest: перевод/выбор RU, генерацию таблиц,
шрифты и лицензии, кириллическую отрисовку/проценты/оформление, тест RU/EN.
В частности, содержит унаследованное уменьшение RU-цифр статистики до 80%,
расширение области букв, fitting и две строки времени в закладке сна.
Это не «только ru.strings» и не новая пользовательская приёмка этих решений.
Кнопочный mapping Libra2, FB2/comics и остальные изменения форка не переносятся.
Основные `src/`, `assets/` текущей ветки этим набором НЕ заменяются.

- `ru.strings` — редактируемый источник перевода для overlay.
- `en.strings` — точно зафиксированный, проверенный английский оригинал beta10.
- `manifest.json` — upstream SHA и SHA-256 входов/результатов всех 36 файлов.
- `verify.py` — применение к чистому upstream в temp, контроль хешей и строк.
- `validate.cmake` — штатная генерация строк, проверка ссылок/аргументов/asset manifest.

## Текущая beta10

Из корня git-репозитория (Python 3.10+, git; для CMake-проверки CMake 3.22+):

```sh
git fetch --no-tags https://github.com/nicoverbruggen/kobalt.git 1796a32559dbb2912bd565990d1c7a0ca7ac4e52
python3 platforms/control-beta10/verify.py
python3 -m unittest discover -s platforms/control-beta10 -p 'test_*.py'
bash platforms/control-beta10/prepare.sh
cmake -DROOT="$PWD/build/control-beta10" -P platforms/control-beta10/validate.cmake
```

`prepare.sh` откажется перезаписывать существующий `build/control-beta10`.
Собирать нужно **только этот export**, штатными средствами выбранной платформы,
не родительский форк. В этой работе новая Qt5-бинарная сборка не выполнялась.

После изменения `platforms/control-beta10/ru.strings`:

```sh
python3 platforms/control-beta10/refresh_overlay.py --source build/control-beta10
python3 platforms/control-beta10/verify.py
cmake -DROOT="$PWD/build/control-beta10" -P platforms/control-beta10/validate.cmake
```

Refresh использует отдельный временный git index и список 36 файлов, а не `git add .`.
Копирует canonical RU в export, обновляет patch и выходные хеши. Ветку/HEAD не меняет.
Не собирает автоматически незарегистрированные в manifest файлы.

## Следующий upstream: обязательная проверка, а не слепое применение

```sh
python3 platforms/control-beta10/audit_strings.py \
  --english /path/to/new-upstream/assets/en.strings \
  --report doc/reports/control-beta10/next-translation-audit.md
```

Создаются Markdown + JSON. Коды возврата: 0 — нет дрейфа, 1 — требуется review,
2 — ошибка входов. Выявляются новые/удалённые ключи, **изменённый EN при прежнем
ключе и прежних аргументах**, пустые RU, потерянные/повторённые `%N`, устаревший
литеральный `%` после готового процентного аргумента. Ничего не переводится,
не удаляется и не утверждается автоматически. Нулевой отчёт не доказывает
качество перевода, геометрию, падежи или неизменность C++-контракта аргументов.

Далее вручную:
1. Перевести новые строки, сверить изменённые по смыслу и месту использования.
2. Проверить C++-форматирование аргументов (особенно проценты/время), локаль,
   шрифты и presentation hunks; удалить то, что уже исправил upstream.
3. На отдельном чистом export новой версии адаптировать overlay. Старый
   `prepare.sh` намеренно остаётся привязан к beta10; нельзя просто поменять
   SHA и считать отсутствие patch conflict доказательством совместимости.
4. Только после review обновить EN baseline, upstream SHA в manifest/prepare,
   upstream SHA-256 и список файлов. Затем refresh/verify/CMake, сборка и RU/EN
   device-проверка. Старый baseline до этого не перезаписывать: иначе дрейф EN
   потеряется.

Скрипты не переключают ветки и не устанавливают ничего на устройство.
