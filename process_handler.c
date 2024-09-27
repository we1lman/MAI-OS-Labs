#include "process_handler.h"
#include <stdio.h>
#include <string.h>

HANDLE create_named_pipe(const char *pipe_name) {
    HANDLE hPipe = CreateNamedPipeA(
        pipe_name,                // Имя канала
        PIPE_ACCESS_OUTBOUND,     // Направление передачи данных - запись
        PIPE_TYPE_MESSAGE |       // Тип канала - сообщения
        PIPE_READMODE_MESSAGE | 
        PIPE_WAIT, 
        1,                        // Максимальное количество экземпляров
        0,                        // Размер выходного буфера
        0,                        // Размер входного буфера
        0,                        // Время ожидания
        NULL                      // Атрибуты безопасности
    );

    if (hPipe == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Failed to create named pipe %s. Error: %lu\n", pipe_name, GetLastError());
    }

    return hPipe;
}

PROCESS_INFORMATION create_child_process(const char *process_name, const char *pipe_name, const char *output_file) {
    PROCESS_INFORMATION pi;
    STARTUPINFOA si;
    ZeroMemory(&pi, sizeof(pi));
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);

    // Формирование командной строки: process_name pipe_name output_file
    char cmdLine[512];
    snprintf(cmdLine, sizeof(cmdLine), "\"%s\" \"%s\" \"%s\"", process_name, pipe_name, output_file);

    // Создание процесса
    BOOL success = CreateProcessA(
        NULL,           // Имя модуля
        cmdLine,        // Командная строка
        NULL,           // Дескриптор процесса не наследуется
        NULL,           // Дескриптор потока не наследуется
        FALSE,          // Наследование дескрипторов
        0,              // Флаги создания
        NULL,           // Использовать родительскую среду
        NULL,           // Использовать родительский текущий каталог
        &si,            // Структура STARTUPINFO
        &pi             // Структура PROCESS_INFORMATION
    );

    if (!success) {
        fprintf(stderr, "Failed to create process %s. Error: %lu\n", process_name, GetLastError());
    }

    return pi;
}
