# Simple Vector

## Описание
Проект представляет собой реализацию шаблонного класса SimpleVector, упрощенный вариант std::vector из стандартной библиотеки C++. Основные особенности:

- Управление памятью через класс ArrayPtr
- Поддержка операций вставки, удаления, изменения размера
- Move/copy семантика
- Итераторы
- Обработка исключений
- Поддержка некопируемых объектов

## Основные компоненты

### SimpleVector
Шаблонный класс, реализующий динамический массив с поддержкой:

- Конструкторов (по умолчанию, копирования, перемещения)
- Операторов присваивания (копирования, перемещения)
- Методов:
  - Reserve - резервирование памяти
  - Resize - изменение размера
  - PushBack/PopBack - добавление/удаление элементов
  - Insert/Erase - вставка/удаление по итератору
  - Clear - очистка вектора
- Итераторов (begin, end, cbegin, cend)

### ArrayPtr
Класс для управления сырым выделением памяти:

- Выделение/освобождение памяти
- Операторы доступа к элементам
- Управление емкостью
- Поддержка move семантики
- Запрет копирования

## Уникальные особенности

- Использование ReserveProxyObj для удобного резервирования памяти
- Явный вызов деструкторов при удалении элементов
- Использование placement new для создания объектов
- Поддержка некопируемых объектов
- Оптимизация работы с временными объектами

## Сборка проекта

Проект можно собрать через CMake:

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## Запуск тестов

Проект включает набор тестов, проверяющих:

- Корректность работы методов SimpleVector
- Обработку исключений
- Move/copy семантику
- Работу с некопируемыми объектами

Тесты находятся в файле main.cpp. Для запуска необходимо скомпилировать проект и запустить полученный исполняемый файл.

## Инструменты разработки

- Язык программирования: C++17
- Сборка: CMake 3.11+
- Компилятор: GCC (MinGW-W64) 13.2.0+ или Clang 18.1.8+
- IDE: Visual Studio Code, CLion или любая другая с поддержкой CMake
