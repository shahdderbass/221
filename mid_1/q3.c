#include <stdio.h>
#include <signal.h>
#include "csapp.h"
#include <sys/types.h>
#include <sys/wait.h>
#define N 5

void handler(){
	int olderror = errno;
	pid_t child_pid;
	while((child_pid = wait(NULL))>0){
			sio_puts("Child reaped: ");
			sio_putl((long)child_pid);
			sio_puts("\n");
	}
	if(errno != ECHILD){
		sio_error("Wait Error!\n");
	}
	errno = olderror;
}

int main(){
	pid_t pid[N];

	signal(SIGCHLD, handler);
	
	for(int i = 0; i < N; i++){
		if((pid[i] = fork()) == 0){
			sleep(3);
			exit(0);
		}
	}
	pause();
	return 0;
}
