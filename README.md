# Сборка
1. Создать папку внутри которой будет лежать сборка. Например build. ```mkdir build```
2. Внутри папки build выполнить
```cmake ..``` затем
```cmake --build . --config Release```

# Запуск
## Конвертация из XES в SQ3
Из папки build ```./XES_SQ3_Converter to_sq3 <xes_filename>.xes```
## Конвертация из SQ3 в XES
Из папки build ```./XES_SQ3_Converter to_xes <db_filename>.db```
