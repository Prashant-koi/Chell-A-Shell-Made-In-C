#include "chell_extensions.h"

#include <unistd.h>
#ifdef _WIN32
    #include <windows.h>
    #include <process.h>
#else
    #include <sys/wait.h>
#endif


int execut_piped_commands(char** args) {
    int i;
    int pipe_os = -1;

    // Find pipe
    for (i=0; args[i] != NULL; i++) {
        if (strcmp(args[i], "|") == 0) {
            pipe_os = i;
            break;
        }
    }

    // No pipe
    if (pipe_os == -1) {
        return -1;
    }

#ifdef _WIN32
    // Windows 
    // Split commands
    args[pipe_os] = NULL;
    char** cmd1 = args;
    char** cmd2 = &args[pipe_os+1];
    
    // Create command strings for system()
    char cmd1_str[1024] = {0};
    char cmd2_str[1024] = {0};
    char pipe_cmd[2048] = {0};
    
    for (i = 0; cmd1[i] != NULL; i++) {
        strcat(cmd1_str, cmd1[i]);
        if (cmd1[i+1] != NULL) strcat(cmd1_str, " ");
    }
    
    for (i = 0; cmd2[i] != NULL; i++) {
        strcat(cmd2_str, cmd2[i]);
        if (cmd2[i+1] != NULL) strcat(cmd2_str, " ");
    }
    
    // Build the pipe command
    snprintf(pipe_cmd, sizeof(pipe_cmd), "%s | %s", cmd1_str, cmd2_str);
    system(pipe_cmd);
    return 1;
#else 
    // Linux
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
#endif
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