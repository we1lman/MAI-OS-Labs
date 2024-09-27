#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "string_utils.h"
#include "file_handler.h"

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: child_process.exe <pipe_name> <output_file>\n");
        return 1;
    }

    const char *pipe_name = argv[1];
    const char *output_file = argv[2];

    // Подключение к именованному каналу
    HANDLE hPipe = CreateFileA(
        pipe_name,             // Имя канала
        GENERIC_READ,          // Чтение
        0,                     // Нет совместного доступа
        NULL,                  // Атрибуты безопасности
        OPEN_EXISTING,         // Открыть существующий канал
        0,                     // Атрибуты
        NULL                   // Шаблон файла
    );

    if (hPipe == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Child: Failed to connect to pipe %s. Error: %lu\n", pipe_name, GetLastError());
        return 1;
    }

    FILE *outfile = open_output_file(output_file);
    if (!outfile) {
        CloseHandle(hPipe);
        return 1;
    }

    char buffer[BUFFER_SIZE];
    DWORD bytesRead;
    while (1) {
        BOOL success = ReadFile(
            hPipe,          // Дескриптор канала
            buffer,         // Буфер для чтения
            BUFFER_SIZE - 1,// Максимальное количество байт для чтения
            &bytesRead,     // Количество прочитанных байт
            NULL            // Синхронное чтение
        );

        if (!success || bytesRead == 0) {
            break;
        }

        buffer[bytesRead] = '\0';
        invert_string(buffer);
        write_line(outfile, buffer);
    }

    close_file(outfile);
    CloseHandle(hPipe);
    return 0;
}
