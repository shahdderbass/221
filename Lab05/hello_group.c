#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

void handler(){
	//write(1, "Signal Caught\n");
	printf("Child received SIGTERM\n");
}

int main(){
	pid_t pid = fork();

	if(pid == 0){
		signal(SIGTERM, handler);
		printf("hello from child\n PID: %d\n PGID: %d\n", getpid(), getpgrp());
	pause();
	exit(0);
	}
	else{
		sleep(3);
		kill(pid, SIGTERM);
	}

	return 0;
}
