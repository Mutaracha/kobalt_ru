# Обложка сна/выключения после выхода из ридера

Статус: implemented, local/CI/device pending.
Источник: 34faf24, `test & diagnostic/94498e7/diagnostics.zip`.
Пользователь подтверждает оба формата: FB2 и EPUB. В архиве home-sleep-screen
дважды mode=plain, design=bookmark (18:49:40.168 и 18:50:44.866 UTC устройства).
Настройки: sleep_screen=cover, sleep_custom sleep/off background=cover, same=true.

## Причина и ограниченное исправление

Внутри ридера обложка декодируется движком. HomeSleepScreen::prepareHomeScreen
раньше искал только Nickel N3_FULL cover; отсутствие/недоступность native cache
оставляло plain paper с актуальной подписью. Это общий путь сна и выключения.
Лог доказывает fallback в plain; конкретный errno native cache в нём не записан.

Если native cover не загрузилась, теперь завершаем уже поставленное сохранение
sleep snapshot и читаем его только по текущему токену книги из Nickel. Используем
только изображение обложки и coverArea — подписи, процент, название, настройки
дизайна и позиция статуса по-прежнему формируются из текущих данных.

Guards: Mode::Cover, непустой совпавший token, privateBook=false, dimPage=false,
валидная непустая coverArea внутри image. Чужой/устаревший snapshot, private,
чтение страницы вместо обложки и неизвестное происхождение не подходят.
Durable privacy check по-прежнему выполняется первым. Native cover остаётся
первым источником. При смене размеров вырезаем только coverArea и вписываем
в новый экран. Не открываем и не верстаем книгу во время сна/выключения;
не пишем в кэш Nickel и не ослабляем fingerprint/token/privacy guards.

## Проверки

Дополнен реальный homeScreenDrawsLastKobaltBook в kobo_power_screen.cpp:
нет native cover, но Kobalt сохранил clean cover; новый HomeSleepScreen owner
восстанавливает её с диска для sleep и power-off, используя текущие метаданные,
а не старые title/percent snapshot. Отдельно отвергаются dim/private/no-coverArea
и изменившийся last-read token. Прежние no-cover/plain и privacy cases сохранены.
Workflow собирает kobalt-power-screen-tests и включает весь platform.kobo-power-screen
в selected native suite, не только новый case.

Основной 040 содержит два изменённых runtime пути: home_sleep_screen.cpp и
kobo_power_screen.cpp. File map и reference amendment обновлены явно. Порядок
всех семи патчей не меняется. Текущая правка не затрагивает FB2/EPUB engine,
Hybrid, книжный кэш или формат persistence.

## После успешного CI на устройстве

На FB2, затем EPUB: открыть книгу, выйти через «Закрыть Kobalt», отправить Kobo
в сон и разбудить. Обложка и подпись должны сохраниться. Проверить выключение
с выбранной обложкой. Не сбрасывать настройки и кэш. Новые логи понадобятся
только если симптом останется на сборке с исправлением.
