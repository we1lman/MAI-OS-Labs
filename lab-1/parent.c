#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "sys/wait.h"

#define MAX_BUFFER_SIZE 1024

int create_process() {
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(-1);
    }
    return pid;
}

int main() {
    int pipe1[2];
    int pipe2[2];
    char filename1[MAX_BUFFER_SIZE];
    char filename2[MAX_BUFFER_SIZE];

    printf("Enter filename for child1: ");
    fgets(filename1, MAX_BUFFER_SIZE, stdin);
    filename1[strcspn(filename1, "\n")] = 0;

    printf("Enter filename for child2: ");
    fgets(filename2, MAX_BUFFER_SIZE, stdin);
    filename2[strcspn(filename2, "\n")] = 0;
    
    if (pipe(pipe1) == -1) {
    perror("pipe1");
    exit(1);
    }
    if (pipe(pipe2) == -1) {
    perror("pipe1");
    exit(1);
    }
    
    pid_t pid1 = create_process();
    if (pid1 == 0) {
        close(pipe1[1]);
        close(pipe2[0]);
        close(pipe2[1]);

        dup2(pipe1[0], STDIN_FILENO);
        close(pipe1[0]);

        execl("./build/child1", "./build/child1", filename1, NULL);
        perror("execl");
        exit(1);
    }

    pid_t pid2 = create_process();
    if (pid2 == 0) {
        close(pipe2[1]);
        close(pipe1[0]);
        close(pipe1[1]);

        dup2(pipe2[0], STDIN_FILENO);
        close(pipe2[0]);

        execl("./build/child2", "./build/child2", filename2, NULL);
        perror("execl");
        exit(1);
    }

    close(pipe1[0]);
    close(pipe2[0]);

    char input_str[MAX_BUFFER_SIZE];
    int str_count = 0;

    printf("Enter strings (to finish, enter 'stop'):\n");

    while(1) {
        if (fgets(input_str, MAX_BUFFER_SIZE, stdin) == NULL) {
            perror("fgets");
            break;
        }
        if (strncmp(input_str, "stop", 4) == 0) {
            break;
        }
        str_count++;

        if (str_count%2 == 1) {
            write(pipe1[1], input_str, strlen(input_str));
        } else {
            write(pipe2[1], input_str, strlen(input_str));
        }
    }

    close(pipe1[1]);
    close(pipe2[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    printf("Parent procces has terminated.\n");

    return 0;
}