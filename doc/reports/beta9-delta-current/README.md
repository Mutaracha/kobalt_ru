# beta9 delta — текущий checkpoint выравнивания

Upstream `be47258850ffdc3295257475afdf8b8f6cb53176` → fork
`e04c5d8446d6dc71cf13c01d65767b39054b59a6`.
[Исторический вход и правила beta.10](../beta9-delta/README.md) сохранены неизменными.

[Все файлы](files.tsv), [точные счётчики](summary.json), [diff stat](changes.stat).
389 неидентичных tracked путей: 215 different, 165 fork-only, 9 upstream-only;
1159 идентичны. Добавленные checkpoint reports тоже входят в эти числа;
число изменившихся путей не равно числу исправленных багов.
155 записей — документация/evidence. REVIEW остаётся консервативным статусом,
не автоматическим одобрением оставшихся hunks.

## Изменения после исходного журнала

| Пункт журнала | Реализация / граница |
|---|---|
| B9-03/04 | `4cd2e6ca`, `5f01dece`: upstream lazy Search/Dictionary keyboard lifecycle, search text-readiness/chapter captions, Dictionaries stale/showEvent; whole chapter-choice page update API + Reader callers; goToChapter(index) вместо позиции, shared anchors не сливаются. Сохранены fork save-action glyph, CLDR и row presentation. Search chapter caption использует существующий `%1: %2`, без новых строк. |
| B9-07 | `82db52a8`: upstream Android Activity/JNI request generations и delivery, LibraryOpen, ScreenBrightness/Light gesture persistence; Gradle тесты зарегистрированы; standalone BookOpen request сохраняет passage при замене открытия. Сохранены locale hooks и BOOX waveform implementation/close lifecycle. Specific system-brightness label согласован и подключён в8486d267. Android APK не проверен. |
| B9-08 | `15c4d2bf`, `9643d168`: whole upstream diagnostic worker/API (включая startup failure handling), scenario resource reporting/player, comparator/config/scenarios, desktop monitor + app/menu consumers, upstream monitor/scenario/model tests. Нет второй fork diagnostic очереди. |
| B9-09 | `223e87b1`: полные upstream chapter_parity + scenarios, fast_shaping/surfaces, layout_interrupt, page_estimate, text_sample и4 fixtures; source/test dependencies и fixture definitions перенесены. Production EPUB Hybrid алгоритмы не менялись ради этих тестов. |
| B9-06 | `a342b680`: поиск всех production includes/callers показал, что fork NativeReadingSave/NativeReadingTime/saveNativeEvents — мёртвые helpers, использовавшиеся только из своих isolated tests, а не второй live writer. Helpers и их dead-only tests удалены; живые native session/time/progress implementations не переписаны. Whole upstream NativeSaveTrace + его самостоятельный test восстановлены. В pinned upstream этот trace тоже не имеет production callers: это не новая инструментализация реальных saves. Раннее предположение журнала о параллельном runtime writer не подтвердилось. FB2 progress_policy/native_publication hooks остаются отдельно. |
| Qt6 copy | `a342b680`: whole upstream opaque-copy helper, display-format consumer, NickelFix ownership guard и pewter test. Это оптимизация opaque pixel composition, не смена waveform. Qt6 runtime пока не проверен. |
| Sleep positions | `82db52a8`: resetNativeUnread и standalone reset сохраняют upstream sleep_positions, добавлена regression assertion. |

## Продолжение после согласования строк

-8486d267: применены62 утверждённых RU строки с EN дословно upstream;
  подключён system-brightness consumer; stale opening-cover test заменён upstream.
-957618b9: whole upstream ReaderOrientation model/header/test, exact-lock controls
  Appearance/Reader assignment/callback/input guard/refresh, LockOpen geometry.
  Fork comics/progress controls и icon/presentation сохранены.
-e04c5d84: upstream finishExit deadline/failExit recovery, TimedOut modal variant,
  close-save input guards и popup ownership, private sleep snapshot off-state;
  upstream slowCloseSave/closeWithoutWaitingForHost/sleepCancelsPendingExit tests
  и data/CTest registrations. Нет replay уже начатого native write после timeout.

## Проверки и ошибки

- Source allowlist PASS; локальная offline suite PASS с CMAKE_GENERATOR=Ninja.
  Один запуск с Makefiles упал на существующем timestamp/reconfigure test;
  assertions не менялись и тест не удалён.
- CI45053027010 на9643d168: Qt5 SUCCESS, native configure FAIL — новым tests
  не хватало area labels. `ec9eee6c` добавляет конкретные area labels, не catch-all
  и не исключение тестов. Последующие runtime/tests также входят в новый запуск.
- CI45053881024 наec9eee6c: Qt5 SUCCESS; native497 tests — единственный
  failure desktop.settingsOpeningCover: старый test обращался к openingCanvas
  после того, как upstream lifecycle уже освободил его. В8486d267 тест заменён
  целиком pinned upstream вариантом: проверка live controls до opened, затем
  QPointer/assertion на освобождение surface/dialog. Assertions не отключены.
- Текущая проверка e04c5d84: **CI45055269191 / build280 SUCCESS**.
  Native job11m57s; Qt5/package job7m41s.
  Device artifact14767806860 (37426810 bytes), native14768081201.
  https://github.com/Mutaracha/kobalt_ru/actions/runs/45055269191/artifacts/14767806860
  Промежуточный8486d267 / build278: CI45055083027 SUCCESS (native + Qt5).
  Device artifact14767726086,37424719 bytes; native14766924758.
  Этот промежуточный run superseded полностью зелёным build280.
- Device/Qt6/Android не проверены; физический drain/ghosting/freeze не исправлены
  на основании одних source/offline checks.

## Что ещё не закрыто

B9-02 quiet/preload + связанные Reader lifecycle consumers; B9-05 debug/status,
percentage/header-footer и часть Settings consumers. Rotation lock и close-timeout
перенесены и проверены native/Qt5 CI280. [62 строки](../../beta9-remaining-strings-review.md)
согласованы пользователем целиком и применены; это больше не wording blocker. Нельзя объявить это выравнивание
завершённым или намеренной fork delta до переноса callers/tests и проверки.

Принятые границы остаются: FB2/comics/RU/brand/Libra2/waveform сохраняются,
EPUB Hybrid algorithms frozen; beta.10 progress-bar replacement PLANNED,
старый renderer полностью удаляется при замене. Не возвращать удалённую полосу
page-count/time-to-end меню и quiet/frontlight diagnostics.

Воспроизведение: тот же platforms/ci/upstream_delta.py с --fork e04c5d84 и
--output в отдельный каталог. Все новые данные берутся из Git objects, не root.
