#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#define N 5

int main(){

	pid_t pid[N];
	int child_status;
	pid_t child_pid;

	for(int i = 0; i < N; i++){
		if((pid[i] = fork()) == 0){
			while(1);
		}
	}

	for(int i = 0; i < N; i++){
		kill(pid[i], SIGINT);
	}
	/*
	for(int i = 0; i < N; i++){
		child_pid = wait(&child_status);

		if(WIFEXITED(child_status)){
			printf("Child %d terminated with exit status: %d\n", child_pid, WEXITSTATUS(child_status));
		}
		else
			printf("Child %d terminated abnormally\n", child_pid);
	}
	*/

	for(int i = 0; i < N; i++){
    		child_pid = wait(&child_status);

    		if (WIFEXITED(child_status)) {
        		printf("Child %d terminated with exit status: %d\n", child_pid, WEXITSTATUS(child_status));
    		} 
    		else if (WIFSIGNALED(child_status)) {
        		printf("Child %d terminated by signal: %d\n", child_pid, WTERMSIG(child_status));
    		} 
    		else {
        		printf("Child %d terminated abnormally\n", child_pid);
	  	}
	}

	return 0;
}
