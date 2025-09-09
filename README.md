# Система управления библиотекой

## Требования
- Компилятор C++ с поддержкой стандарта C++23
- make (или MinGW/MSYS2 на Windows)

## Сборка и запуск

### Linux/macOS:
```bash
# Сборка
make

# Запуск
./main

# Очистка
make clear
```

### Windows:
```cmd
# Сборка
make

# Запуск
main.exe

# Очистка
make clear
```

### Альтернатива для Windows (без make):
```cmd
# Сборка
g++ -std=c++23 -Wall -Wextra main.cpp users.cpp app_time.cpp book.cpp library_manager.cpp -o main.exe

# Запуск
main.exe
```

