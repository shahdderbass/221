#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include "csapp.h"
#include <sys/wait.h>

void handler(){
	sigset_t mask, prev_mask;
	int olderror = errno;
	pid_t child_pid;

	while((child_pid = wait(-1,NULL, 0)) > 0){
		/*sio_puts("Child terminated: ");
		sio_putl((long)child_pid);
		sio_puts("\n");
		*/
		sigprocmask(SIG_BLOCK, &mask, &prev_mask);
		deletejob(child_pid);
		sigprocmask(SIG_SETMASK, &prev_mask, NULL);
	}
	if(errno != ECHILD)
		sio_error("waitpid() error.\n");
	
	errno = olderror;
}

int main(int argc, char *argv[]){
	pid_t pid;
	sigset_t mask, prev_mask;
	
	sigfillset(&mask);
	initjobs();
	signal(SIGCHLD, handler);
	while(1){
		if((pid = fork()) == 0)
			execve("/bin/date", argv, NULL);

	sigprocmask(SIG_BLOCK, &mask, &prev_mask);
	addjob(pid);
	sigprocmask(SIG_SETMASK, &prev_mask, NULL);
	
	}
	return 0;
}
