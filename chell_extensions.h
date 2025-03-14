#ifndef CHELL_EXTENSIONS_H
#define CHELL_EXTENSIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Only include what's needed for declarations
#ifdef _WIN32
    #include <windows.h>
#endif

#define HISTORY_SIZE 20

// Function declarations only
void add_to_history(char* line);
int shell_history(char** args);
int execut_piped_commands(char** args);
int handle_redirection(char** args);

#endif