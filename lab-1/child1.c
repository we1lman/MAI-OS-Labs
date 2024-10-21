#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 1024

void str_reverse(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len/2; i++) {
        char temp = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = temp;
    }
}

int main(int argc, char *argv[]) {
    char *filename = argv[1];
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("fopen");
        exit(1);
    }

    char input_str[MAX_BUFFER_SIZE];
    while (fgets(input_str, MAX_BUFFER_SIZE, stdin) != NULL) {
        input_str[strcspn(input_str, "\n")] = 0;
        str_reverse(input_str);
        fprintf(file, "%s\n", input_str);
    }

    fclose(file);
    printf("Child1 process has terminated.\n");
    
    return 0;
}