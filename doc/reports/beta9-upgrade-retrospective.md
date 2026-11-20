# Подготовка следующего обновления: выводы из beta.8 → beta.9

## Граница и источники

Анализ опубликованного форка `4e3a33b9` и сохранённых отчётов переноса;
upstream baseline — beta.9 `be47258850ffdc3295257475afdf8b8f6cb53176`.
Это не анализ ещё не зафиксированной beta.10 и не обещание её свойств.
Runtime не менялся. Активная очередь — [план](../../FB2_FORK_PLAN.md),
решение о границе EPUB/FB2 — UPSTREAM-NEXT в [реестре](../fork-decisions.md).

## Отказы и меры предупреждения

| Наблюдение / доказательство | Установленный механизм и ограничение вывода | Проверка при следующем обновлении |
|---|---|---|
| Stage 1: native `engine.page-layout` FAIL, Qt5/package PASS; затем PASS (`beta9-stage1-blaze.md`) | Новая библиотека требовала adapter policy `setKoblazeDropEmptyPages(true)`. Не дефект vendor и не основание менять assertion. | До подключения библиотеки составить список новых configure/render switches и их upstream callers; выполнить layout corpus вместе с adapter contract tests. |
| Worker CI `44601310965`: native configuration FAIL (`beta9-worker-contracts.md`) | Новые тесты не имели обязательных area labels. | Одновременно переносить test registration, labels и focused workflow selection; проверять список реально выбранных тестов, а не только успешную компиляцию. |
| Adapter CI `44610120056`: compile FAIL (`beta9-engine-adapter.md`) | Пропущены `ResourceCache::fresh` и новая сигнатура Qt5 test double. | Переносить замкнутый набор API + implementation + callers + mocks + tests; предварительно сравнить зависимости интерфейса. |
| `44611131876`: Qt5 PASS, native link FAIL; позднее UI compile failures | `page_estimate.cpp` отсутствовал в явном desktop source list; `reading_page.cpp` полагался на транзитивные includes. | Проверять одновременно recursive Kobo globs и explicit desktop lists; при отвязке concrete adapter добавлять прямые headers типов. |
| `44612383960`: FB2 mapping assertion FAIL | Fixture использовал processing instruction `/?xml[1].0`, не body passage. Строгое сравнение сохранено после выбора passage через search/goTo; initial bookmark renderer не исправлен этим тестом. | Для position tests указывать семантический anchor и expected coordinate space; отдельно проверять invalid/initial anchors без подмены ими passage round-trip. |
| Session integration: styling/checkpoint/progress failures | Потребители нуждались в upstream transient-state sanitation и book-level координатах вместо fragment-local. | Переносить producers и persistence/UI consumers одной контрактной группой. Проверять non-final-fragment finish, whole-book percentage, checkpoint budget и отсутствие transient state в sample keys. |
| Highlight/markup/activity tests после переноса session; итоговый PASS `44633797097` | Пропущен upstream launcher `run-test.cmake` с Accurate default для legacy tests. FB2 test также переведён в app.exec для worker shutdown; отдельная причина прежнего process exit не изолирована. | Сравнивать launcher/env/teardown вместе с тестами. Legacy defaults и явные Hybrid overrides проверять отдельно. Не чинить EPUB algorithm под несовместимый harness. |
| `44635988130`: advancedText FAIL, Qt5 PASS; затем PASS `44637222380` | Тест блокировал worker до завершения background work; отсутствовал upstream idle barrier. | Переносить test synchronization; сохранять strict no-worker assertion. Не заменять barrier случайной задержкой. |
| Sleep integration: anonymous checklistOption; Activity/Stats fixture failures (текущий план) | Не полностью перенесена общая helper/API граница; UI tests должны ждать loaded Overview, а не welcome, и учитывать новые shared word labels. | Переносить helper visibility и UI state machine вместе с consumers; ожидать наблюдаемое состояние, не таймер. |
| `44778323964`: activityUnitForms FAIL; `44781746091` PASS (`033d0be2-followup.md`) | Empty-history fixture скрывал Overview и давал пустой label. Добавлены reading day и проверка видимости; сравнения не ослаблялись. | Разделить empty/loaded fixtures; до проверки перевода подтвердить состояние и видимость, сохранить exact text/plural checks. |
| Device repeat Accurate/Hybrid почти одинаков; follow-up подтвердил `page-note-layout` fallback (`cfb8f51c-device-followup.md`) | Retained model не cold A/B; fallback — защита сносок, не доказательство поломки Hybrid. | Сохранять requested/effective mode и reason, retained/disk-cache context; не складывать вложенные timings. Не отключать безопасность ради цифр. |

Источники в таблице — файлы этого каталога и `../validation/033d0be2-followup.md`.
CI-идентификаторы относятся к историческим прогонам, не к новой проверке.
Старое зависание после frame load остаётся отдельным неустановленным дефектом;
успех переноса, standby или package build не доказывает его устранение.

## Что замедляло перенос

1. Поэтапность была полезна для локализации, но промежуточные API попадали без
   полного набора consumers, тестовой инфраструктуры и source lists.
2. Исторические этапы в плане сохраняли pending после последующих PASS. Их нельзя
   использовать как текущую карту расхождений. Следующему обновлению нужна одна
   матрица покрытия, включая сознательные исключения, а не только список конфликтов.
3. Отсутствующий upstream файл или старый вызывающий код не всегда создаёт merge
   conflict. Нужны отдельные inventories added/deleted/changed и семантический review.
4. Qt5/package PASS не заменяет native behavior tests, а native PASS — устройство.
   Полный package на каждом промежуточном переносе без test-registration preflight
   даёт позднюю и дорогую обратную связь.

## Протокол следующего обновления

### До начала runtime-переноса

- Зафиксировать точные fork/upstream SHA и release notes; beta.10 сначала изучить,
  не предугадывать. Сделать backup полного профиля и описать безопасный rollback.
- Обновить `doc/fork-delta.md`: upstream-owned, fork-owned, relocated common,
  justified integration, explicit exclusion. Для каждого отклонения — причина,
  владелец, тест. Старый beta9-port-map — исторический, не новая матрица.
- Построить inventories всех production/build/assets/test/launcher файлов,
  включая upstream-only. Проверить удалённые upstream API и оставшихся callers.
- Для каждой контрактной группы перечислить dependencies, consumers, persistence,
  test doubles, test env, desktop source list, Kobo selection и gate результата.

### Порядок переноса

1. Build/dependency/library policies вместе с минимальными adapter gates.
2. Цельные Engine/worker/session contracts с upstream EPUB consumers и harness;
   узкие fork hooks интегрировать контекстно, не оставлять второй legacy путь.
3. Persistence/native lifecycle и asynchronous barriers; отдельно rejected/stale/
   destroyed-owner/close/sleep/USB cases и сохранность профиля.
4. UI/shared helpers/approved strings и consumers целиком. Новые формулировки
   согласовывать до добавления. Сохранять принятый strip/comics/sleep fit.
5. Повторный семантический delta audit: добавленные/пропущенные файлы, дублирующие
   реализации, старые patches и необоснованные отличия. Затем device handoff.

### Gates без ослабления тестов

- Дешёвый preflight: diff/whitespace, доступные strings/schema/source checks,
  registration/labels и фактический focused test list. При отсутствии SDK указать
  NOT RUN, не объявлять проверки пройденными.
- Native: upstream EPUB Accurate и Hybrid/adoption отдельно; fork FB2/comics,
  positions/reflow, cache/checkpoint budgets, annotations/native saves и lifecycle.
  Проверки должны действительно входить в focused CI selection.
- Kobo Qt5: compile/package/contents; reuse неизменившихся результатов допустим
  только с указанной областью. Qt6/Android не становятся release blockers.
- Устройство: корректность навигации/сносок/сохранений/strip/sleep и только затем
  контролируемые timings. Записывать binary SHA, artifact, cache/mode context,
  ограничения и непроверенное. Не называть docs-only commit новым runtime.
- После каждого блока обновлять одну текущую матрицу и краткий статус плана;
  подробный журнал и старые failures оставлять в reports, не в активной очереди.

## Отдельный FB2 workstream

Можно параллельно с UI/build cleanup исследовать FB2 sections/resources/notes,
стабильные source anchors, dependency map и corpus. Это подготовка, не включение
частичной раскладки. Gate перед реализацией: доказать сохранность позиции/reflow,
переходов через sections, cross-section notes, exact/estimated progress, cache
identity и корректный whole-book fallback. Общие Engine/session contracts сначала
стабилизируются; EPUB slicing/adoption/fallback алгоритмы не меняются ради FB2.
Если требуется их переделка, остановиться и вынести отдельное согласование.
