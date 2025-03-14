#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "chell_extensions.h"

#ifdef _WIN32
    #include <windows.h>
    #include <process.h>
    #include <direct.h>
    #define chdir _chdir
    #define HOME_ENV "USERPROFILE" 
#else
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/wait.h>
    #define HOME_ENV "HOME"
#endif


#define MAX_LINE 1024
#define MAX_ARGS 64
#define PROMPT "chell> "

int shell_cd(char** args);
int shell_help(char** args);

//this is the list of builtin commands
char* builtin_commands[] = {
    "cd",
    "help",
    "history",
};

//Array of function pointers to builtin commands
int(*builtin_functions[])(char**) = {
    &shell_cd,
    &shell_help,
    &shell_history,
};

int num_builtins() {
    return sizeof(builtin_commands) / sizeof(char*);
}


void display_prompt() {
    printf("%s", PROMPT);
    fflush(stdout);
}

ssize_t getline(char **lineptr, size_t *n, FILE *stream) {
    char *buffer;
    size_t size = 120;
    int ch;
    
    if (*lineptr == NULL) {
        *lineptr = malloc(size);
        *n = size;
    }

    buffer = *lineptr;
    size_t len = 0;
    
    while ((ch = fgetc(stream)) != EOF && ch != '\n') {
        if (len + 1 >= *n) {
            *n *= 2;
            buffer = realloc(buffer, *n);
            *lineptr = buffer;
        }
        buffer[len++] = ch;
    }

    buffer[len] = '\0';
    return len;
}


char * read_line() {
    char* line = NULL;
    size_t bufsize = 0;
    ssize_t chars_read = getline(&line, &bufsize, stdin);

    if (chars_read == -1) {
        if (feof(stdin)) {
            printf("\n");
            exit(EXIT_SUCCESS); 
        } else {
            perror("read_line");
            exit(EXIT_FAILURE);
        }
    }

    return line;
}

char** parse_line(char* line) {
    int bufsize = MAX_ARGS;
    int position = 0;
    char** tokens = malloc(bufsize * sizeof(char*));
    char* token;

    if (!tokens) {
        fprintf(stderr , "chell: allocation error \n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, " \t\r\n\a");
    while (token != NULL) {
        tokens[position] = token;
        position++;

        if (position >= bufsize) {
            bufsize += MAX_ARGS;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
                fprintf(stderr, "chell: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, " \t\r\n\a");
    }
    tokens[position] = NULL;
    return tokens;
}

int execute_command(char** args) {

    // Check for builtin commands
    for (int i = 0; i < num_builtins(); i++) {
        if(strcmp(args[0], builtin_commands[i]) == 0) {
            return (*builtin_functions[i])(args);
        }
    }

    if (args[0] == NULL) {
        return 1; // No command entered
    }

    if (execut_piped_commands(args) != -1) {
        return 1;
    }

    int redir_result = handle_redirection(args);
    if (redir_result != 0) {
        return (redir_result == -1) ? 1 : redir_result;
    }
    
#ifdef _WIN32
    // conver ls to dir
    if (strcmp(args[0], "ls") == 0) {
        system("dir");
        return 1;
    }

    if(strcmp(args[0], "exit") == 0) {
        exit(0);
    }

    // For other commands
    intptr_t pid = _spawnvp(_P_WAIT, args[0], (const char* const*)args);
    if (pid == -1) {
        printf("Failed to execute command: %s\n", args[0]);
        return 1;
    }

#else
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0) {
        // Child process
        if (execvp(args[0], args) == -1) {
            perror("chell");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // Fork failed
        perror("chell");
    } else {
        // Parent 
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
#endif

    return 1;
}

//cd command change directory
int shell_cd(char** args) {
    if (args[1] == NULL) {
        // If no argument then go to the HOME directory
        char* home = getenv(HOME_ENV);
        if (home == NULL) {
            fprintf(stderr, "chell: HOME directory not set\n");
            return 1;
        }
        if (chdir(home) != 0) {
            perror("chell");
        }
    } else {
        if (chdir(args[1]) != 0) {
            perror("chell");
        }
    }
    return 1;
}

//Help command
int shell_help(char** args) {
    (void)args;

    printf("Chell - A simple shell implemetation made in C. \n");
    printf("Type program names and arguments, then press enter.\n");
    printf("Built-in commands:\n");

    for (int i = 0; i < num_builtins(); i++){
        printf("%s\n", builtin_commands[i]);
    }

    printf("Use the man command for information on other programs. \n");
    return 1;
}

int main() {
    char *line;
    char **args;
    int status = 1;

    while (status) {
        display_prompt();
        line = read_line();
        add_to_history(line);
        args = parse_line(line);

        if (args[0] != NULL) {
            status = execute_command(args);
        }

        free(line);
        free(args);
    }

    return EXIT_SUCCESS;
}