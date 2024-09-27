#include "file_handler.h"
#include <stdio.h>

FILE* open_input_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open input file");
    }
    return file;
}

FILE* open_output_file(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open output file");
    }
    return file;
}

void close_file(FILE *file) {
    if (file) {
        fclose(file);
    }
}

char* read_line(FILE *file, char *buffer, size_t size) {
    return fgets(buffer, (int)size, file);
}

void write_line(FILE *file, const char *data) {
    if (file && data) {
        fputs(data, file);
    }
}
