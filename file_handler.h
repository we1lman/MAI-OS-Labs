#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <stdio.h>

// Открыть файл для чтения
FILE* open_input_file(const char *filename);

// Открыть файл для записи
FILE* open_output_file(const char *filename);

// Закрыть файл
void close_file(FILE *file);

// Прочитать строку из файла
// Возвращает NULL при EOF или ошибке
char* read_line(FILE *file, char *buffer, size_t size);

// Записать строку в файл
void write_line(FILE *file, const char *data);

#endif // FILE_HANDLER_H
