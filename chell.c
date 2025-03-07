#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <process.h>
#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif


#define MAX_LINE 1024
#define MAX_ARGS 64
#define PROMPT "chell> "

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

    token = strtok(line, "\t\r\n\a");
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

        token = strtok(NULL, "\t\r\n\a");
    }
    tokens[position] = NULL;
    return tokens;
}

int main() {
    char *line;
    char **args;
    int status = 1;

    while (status) {
        display_prompt();
        line = read_line();
        args = parse_line(line);

        if (args[0] != NULL) {
            printf("Command entered: %s \n", args[0]);
            // We will execute command here(will do later lol)
        }

        free(line);
        free(args);
    }

    return EXIT_SUCCESS;
    
}
