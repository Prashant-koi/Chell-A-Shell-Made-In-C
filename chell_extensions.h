#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#ifdef _WIN32
    #include <windows.h>
    #include <process.h>
#else
    #include <sys/wait.h>
#endif

#define HISTORY_SIZE 20
#ifndef CHELL_EXTENSIONS_H
#define CHELL_EXTENSIONS_H

void add_to_history(char* line);
int shell_history(char** args);
int execut_piped_commands(char** args);

#endif

int execut_piped_commands(char** args) {
    int i;
    int pipe_os = -1;

    //find pipe
    for (i=0; args[i] != NULL; i++) {
        if (strcmp(args[i], "|") == 0) {
            pipe_os = i;
            break;
        }
    }

    //No pipe
    if (pipe_os == -1) {
        return -1;
    }

    //create pipe
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }

    //split
    args[pipe_os] = NULL;
    char** cmd1 = args;
    char** cmd2 = &args[pipe_os+1];

    //first command
    pid_t pid1 = fork();
    if (pid1 < 0) {
        perror("fork");
        return 1;
    }

    if (pid1 == 0) {
        //child process
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        if (execvp(cmd1[0], cmd1) == -1) {
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    }

    pid_t pid2 = fork();
    if (pid2 < 0) {
        perror("fork");
        return 1;
    }

    if (pid2 == 0) {
        //child process
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);

        if (execvp(cmd2[0], cmd2) == -1) {
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    }

    //close pipe
    close(pipefd[0]);
    close(pipefd[1]);

    //wait for children
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    return 1;
}

//History implementation
char* history[HISTORY_SIZE];
int history_count = 0;

void add_to_history(char* line) {
    //skip empty commands
    if (line == NULL || line[0] == '\0' || line[0] == '\n') {
        return;
    }

    //copy
    char* cmd = strdup(line);

    //if history is full, remove the oldest command
    if (history_count == HISTORY_SIZE) {
        free(history[0]);
        for (int i = 0; i < HISTORY_SIZE - 1; i++) {
            history[i] = history[i + 1];
        }
        history_count--;
    }
    history[history_count++] = cmd;
}

int shell_history(char** args) {
    (void)args;
    for (int i = 0; i < history_count; i++) {
        printf("%d: %s\n", i+1, history[i]);
    }
    return 1;
}