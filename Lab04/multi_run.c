#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main() {
    int N;
    printf("Enter number of commands: ");
    scanf("%d", &N);
    getchar();

    if (N < 1 || N > 5) {
        printf("Invalid number of commands.\n");
        return 1;
    }

    char *commands[5][10]; 
    char input[100];

    for (int i = 0; i < N; i++) {
        printf("Command %d: ", i + 1);
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0; 

        int j = 0;
        char *token = strtok(input, " ");
        while (token != NULL && j < 9) {
            commands[i][j++] = strdup(token);
            token = strtok(NULL, " ");
        }
        commands[i][j] = NULL; 
    }

    for (int i = 0; i < N; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("Fork failed");
            exit(1);
        } else if (pid == 0) {
            execvp(commands[i][0], commands[i]);
            perror("execvp failed");
            exit(1);
        }
    }

    for (int i = 0; i < N; i++) {
        int status;
        pid_t child_pid = wait(&status);
       if (child_pid > 0)
            printf("Child %d completed.\n", child_pid);
    
    }

    printf("All commands executed successfully.\n");
    return 0;
}

