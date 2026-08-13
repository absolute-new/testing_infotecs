# C++17 Logger Library & Stats Server

[![C++17](https://img.shields.io/badge/C++-17-blue.svg)](https://en.cppreference.com/w/cpp/17)
[![CMake](https://img.shields.io/badge/CMake-3.10+-brightgreen.svg)](https://cmake.org)
[![Linux](https://img.shields.io/badge/Platform-Linux%20%7C%20Ubuntu-lightgrey)](https://ubuntu.com)

Многопоточная библиотека логирования на C++17 с поддержкой записи в файл и отправки по TCP-сокету.  
Включает в себя консольный клиент для ввода сообщений и сервер для сбора статистики.

## Возможности

- **Два типа логгеров:** `FileLogger` (в файл) и `SocketLogger` (по TCP)
- **Единый интерфейс** через `ILogger` (полиморфизм)
- **Фильтрация по уровню:** `INFO`, `DEBUG`, `ERROR`
- **Потокобезопасность** (мьютексы, атомарные переменные, очередь)
- **Сервер статистики:** подсчёт сообщений, длин, вывод по N сообщений и по таймеру T сек.
- **Сборка:** CMake, статическая/динамическая библиотеки

## Зависимости

Для сборки и работы с проектом **желательно** установить:

Linux/Ubuntu
```bash
sudo apt update
sudo apt install g++ cmake make doxygen graphviz lcov clang-format
```

## Сборка

```bash
mkdir -p build
cd build
cmake ..
make
```

## Запуск

Сборка создаёт исполняемые файлы в папках:
- `build/logger/` — клиент `logger`
- `build/stats/` — сервер статистики `stats`
- `build/logger/logger.conf` — конфигурация для сокетного режима
---


### Клиент (отправка логов)
Перейди в папку с клиентом и запусти:

```bash
cd build/logger/
./logger <log_file> <log_level> [--socket]
```
- `<log_file>` — путь к файлу журнала (например, log.txt)
- `<log_level>` — уровень логирования (INFO, DEBUG, ERROR)
- `--socket` — (необязательно) отправлять логи по сокету вместо файла

Примеры

```bash
./logger log.txt DEBUG
./logger log.txt INFO --socket
```

### Сервер статистики
Перейди в папку с сервером и запусти:

```bash
cd build/stats/
./stats <port> <N> <T>
```
- `<port>` — порт для прослушивания (например, 9000)
- `<N>` — выводить статистику после каждых N сообщений
- `<T>` — выводить статистику каждые T секунд (если изменилась)

Примеры

```bash
./stats 9000 5 10
```

## Конфигурация (для сокетного клиента)

Настройки клиента хранятся в файле `logger.conf` (создай его в корне проекта, если его нет):

```ini
# Файл конфигурации для клиента
address = 127.0.0.1
port = 9000
```
**При сборке он автоматически копируется** в папку `build/logger/` — рядом с исполняемым файлом `logger`.

При запуске через `./logger` файл `logger.conf` должен лежать в той же папке, откуда запущен `logger`.  
По умолчанию он копируется в `build/logger/` при сборке, поэтому запускай из этой папки.

### Уровни логирования

- `INFO` — информационные сообщения (обычный ход работы)
- `DEBUG` — отладочные сообщения (для разработчиков)
- `ERROR` — сообщения об ошибках (критические проблемы)

## Тестирование

```bash
cd build
ctest --verbose
```

Запуск с покрытием:
```bash
make gcov_report
```
После этого перейди в папку `build/htmlcov` и открой файл `index.html` в браузере:

```bash
cd build/htmlcov
firefox index.html
# или
google-chrome index.html
## Структура проекта
```

## Документация

```bash
make docs
```
После этого открой build/docs/html/index.html в браузере.


<details>
<summary><b>📁 Структура проекта</b></summary>

```bash
./
├── build/                  # Папка сборки
├── lib/                    # Библиотека
│   ├── include/            # Публичные заголовки
│   ├── source/             # Исходный код библиотеки
│   └── CMakeLists.txt      # CMake Библиотеки
├── linters/                # Стиль кода
├── logger/                 # Клиентское приложение
│   ├── include/            # Публичные заголовки
│   ├── source/             # Исходный код логгера
│   ├── main.cpp            # Точка входа
│   └── CMakeLists.txt      # CMake Логгера
├── stats/                  # Сервер статистики 
│   ├── include/            # Публичные заголовки
│   ├── source/             # Исходный код сервера
│   ├── main.cpp            # Точка входа.
│   └── CMakeLists.txt      # CMake Сервера
├── tests/                  # Юнит-тесты
├── CMakeLists.txt          # Корневой CMake
├── Doxyfile                # Doxyfile
├── logger.conf             # Конфигурация для сокетного клиента
└── README.md               # README
```

</details>