#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid = fork();

    if (pid == 0) {
       printf("Child process: PID = %d, Parent PID = %d\n", getpid(), getppid());
      } else if (pid > 0) {
        printf("Parent process: PID = %d, Child PID = %d\n", getpid(), pid);
      } else {
        printf("Fork failed\n");
      }

       return 0;
}

