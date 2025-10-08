#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(){
	int status;
	char input[100];
	while(1) {
		printf("Enter command: ");
	        if(fgets(input, sizeof(input), stdin)==NULL)
			break;
		input[strcspn(input, "\n")] = 0;

		if(strcmp(input, "exit") == 0){
			printf("Goodbye!\n");
			break;
		}
		char *args[10];
		int j = 0;
        	char *token = strtok(input, " ");
        	while (token != NULL && j < 9) {
             		args[j++] = token;
             		token = strtok(NULL, " ");
	     	}
          	args[j] = NULL;
		
		printf("Executing: ");
		for (int k = 0; k < j; k++) {
    			printf(" %s", args[k]);
		}
		printf("\n");

		pid_t pid = fork();

		if(pid < 0){
			perror("Fork failed.\n");
			exit(1);
		}
		else if(pid == 0){
			execvp(args[0], args);
			perror("execvp failed");
			exit(1);
		}
		else{
			pid_t child_pid = wait(&status);
			if(child_pid > 0)
				printf("Child %d finished!.\n", child_pid);
		}
	}
	return 0;

}	
