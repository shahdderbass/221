#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid = fork();

	for(int i = 0; i < 3; i++){
    if (pid == 0) {
       printf("I am child number %d, PID = %d\n",i, getpid());
       printf("All children have been created.\n");
      } else if(pid < 0){
        printf("Fork failed\n");
      }
	}

       return 0;
}

