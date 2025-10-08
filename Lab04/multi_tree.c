#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(){

	pid_t grandchild;
	
	pid_t child_1 = fork();
	
	if(child_1 == 0){
		printf("[Child1] PID=<%d>, PPID=<%d>\n", getpid(), getppid());
		
		grandchild = fork();

		if(grandchild == 0){
			printf("[Grandchild1] PID=<%d>, PPID=<%d>\n", getpid(), getppid());
		}
	}

	pid_t child_2 = fork();

	if(child_2 == 0){
		printf("[Child2] PID=<%d>, PPID=<%d>\n", getpid(), getppid());

		grandchild = fork();

		if(grandchild == 0){
			printf("[Grandchild2] PID=<%d>, PPID=<%d>\n", getpid(), getppid());
 		}
	}
	
	sleep(2);
	printf("[Parent] PID=<%d>, PPID=<%d>\n", getpid(), getppid());

	return 0;
}
