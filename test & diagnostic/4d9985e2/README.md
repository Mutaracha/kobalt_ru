# 4d9985e2 — эксперимент без FB2 Home preload

Оригинальный diagnostics.zip: upload 7bbb6aef. screen_001-24.png — исходный screenshot
из разговора, bytes сохранены. Пользователь сообщил ручные A1 11.20 / A2 22.32 s
после A1→A2→reboot; все UI-открытия ручные. В архиве есть несколько других открытий,
их не объединять со stopwatch в одну среднюю и не приписывать одному tap без времени.

Пользователь принял cache-cleanup wording и persistent OFF после reboot;
About wordmark неверен, требуется Instrument Serif. Предзагрузка должна остаться
для FB2 в финальной конфигурации: 035 — только исторический эксперимент.
Анализ: doc/reports/patch-series/036-4d9985e2-device-review.md.
