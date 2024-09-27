#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process_handler.h"
#include "file_handler.h"

#define PIPE1_NAME "\\\\.\\pipe\\pipe1"
#define PIPE2_NAME "\\\\.\\pipe\\pipe2"
#define CHILD_PROCESS_NAME "child_process.exe"
#define INPUT_FILE "input.txt"
#define OUTPUT_FILE1 "output1.txt"
#define OUTPUT_FILE2 "output2.txt"
#define BUFFER_SIZE 1024

int main() {
    // Создание именованных каналов
    HANDLE hPipe1 = create_named_pipe(PIPE1_NAME);
    if (hPipe1 == INVALID_HANDLE_VALUE) {
        return 1;
    }

    HANDLE hPipe2 = create_named_pipe(PIPE2_NAME);
    if (hPipe2 == INVALID_HANDLE_VALUE) {
        CloseHandle(hPipe1);
        return 1;
    }

    // Запуск дочерних процессов
    PROCESS_INFORMATION pi1 = create_child_process(CHILD_PROCESS_NAME, PIPE1_NAME, OUTPUT_FILE1);
    if (pi1.hProcess == NULL) {
        CloseHandle(hPipe1);
        CloseHandle(hPipe2);
        return 1;
    }

    PROCESS_INFORMATION pi2 = create_child_process(CHILD_PROCESS_NAME, PIPE2_NAME, OUTPUT_FILE2);
    if (pi2.hProcess == NULL) {
        CloseHandle(hPipe1);
        CloseHandle(hPipe2);
        // Закрыть первый процесс
        TerminateProcess(pi1.hProcess, 1);
        CloseHandle(pi1.hProcess);
        CloseHandle(pi1.hThread);
        return 1;
    }

    // Ожидание подключения дочерних процессов к каналам
    BOOL connected1 = ConnectNamedPipe(hPipe1, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);
    if (!connected1) {
        fprintf(stderr, "Failed to connect pipe1.\n");
    }

    BOOL connected2 = ConnectNamedPipe(hPipe2, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);
    if (!connected2) {
        fprintf(stderr, "Failed to connect pipe2.\n");
    }

    // Открытие входного файла
    FILE *infile = open_input_file(INPUT_FILE);
    if (!infile) {
        CloseHandle(hPipe1);
        CloseHandle(hPipe2);
        TerminateProcess(pi1.hProcess, 1);
        TerminateProcess(pi2.hProcess, 1);
        return 1;
    }

    char buffer[BUFFER_SIZE];
    int line_number = 1;
    DWORD bytesWritten;

    while (read_line(infile, buffer, BUFFER_SIZE)) {
        size_t len = strlen(buffer);
        if (len == 0) continue;

        // Определение, в какой канал отправлять строку
        HANDLE target_pipe = (line_number % 2 != 0) ? hPipe1 : hPipe2;

        BOOL success = WriteFile(
            target_pipe,     // Дескриптор канала
            buffer,          // Буфер для записи
            (DWORD)strlen(buffer), // Количество байт для записи
            &bytesWritten,   // Количество записанных байт
            NULL             // Синхронная запись
        );

        if (!success) {
            fprintf(stderr, "Failed to write to pipe%d. Error: %lu\n", (line_number % 2 != 0) ? 1 : 2, GetLastError());
        }

        line_number++;
    }

    // Закрытие входного файла
    close_file(infile);

    // Закрытие каналов для записи, чтобы сигнализировать дочерним процессам об окончании передачи
    CloseHandle(hPipe1);
    CloseHandle(hPipe2);

    // Ожидание завершения дочерних процессов
    WaitForSingleObject(pi1.hProcess, INFINITE);
    WaitForSingleObject(pi2.hProcess, INFINITE);

    // Закрытие дескрипторов процессов
    CloseHandle(pi1.hProcess);
    CloseHandle(pi1.hThread);
    CloseHandle(pi2.hProcess);
    CloseHandle(pi2.hThread);

    printf("Processing completed.\n");
    return 0;
}
