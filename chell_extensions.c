#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HISTORY_SIZE 20
#ifndef CHELL_EXTENSIONS_H
#define CHELL_EXTENSIONS_H

void add_to_history(char* line);
int shell_history(char** args);

#endif


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
    for (int i = 0; i < history_count; i++) {
        printf("%d: %s\n", i+1, history[i]);
    }
    return 1;
}