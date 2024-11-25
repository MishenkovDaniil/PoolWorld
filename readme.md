# Pool World! 

## Возможные операции над контейнером бассейнов:
- создать бассейн
- узнать количество бассейнов в контейнере
- соединить два бассейна каналом
- разорвать связь
- узнать сколько воды в каком-либо бассейне
- добавить воду в бассейн 

## Тесты 

- simple - простой тест на работоспособность. Реализация - [simple test impl](https://github.com/MishenkovDaniil/PoolWorld/blob/master/src/tests/tests.cpp).
- time - тест на производительность, когда количество операций над контейнером порядка 10 миллионов. Реализация - [time test impl](https://github.com/MishenkovDaniil/PoolWorld/blob/master/src/tests/tests.cpp). 


О подробных сценариях тестов можно прочитать в [readme](https://github.com/MishenkovDaniil/PoolWorld/blob/master/src/tests/readme.md). 

Время работы `time` теста для описанного там случая составило около 190 секунд. С параметром запуска `-O3` - 97 секунд.

## Запуск 

Для запуска тестов:
- `make simple`
- `make time`

Для запуска собственных файлов можно откорректировать `Makefile` удалив файлы `tests.cpp` и `simpletest/timetest.cpp` и подставив свои вместо них.