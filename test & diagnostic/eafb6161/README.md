# eafb6161: upload 46ecb8f

Оригинальные diagnostics H.zip / diagnostics A.zip и два screenshots сохранены
без изменения bytes. Пользователь: все измеренные запуски ручные, stopwatch ручной.
H: первые cold H1/H2 выполнены до включения логов; после reboot/ON 4.70 и 5.31 s.
A: 14.81, retained repeat 3.80, A2 5.20; reboot, A1 20.49, A2 5.08 s.

screen_001.png: пользователь принял exit-wordmark. screen_001 (2).png: замечание
к RU cache cleanup prompt — явно назвать закрытие Kobalt. Снимок не доказывает
выполнение очистки; её отдельное событие есть в A-логе.

Логи содержат background book-preload, отличную от ручного открытия UI; наличие
этих событий не означает автоматический измеренный запуск. Подробный разбор —
`doc/reports/patch-series/035-eafb6161-device-review.md`.
