#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include "csapp.h"

void handler(){
	sio_puts("\n1\n");
	sleep(1);
	sio_puts("2\n");
	sleep(1);
	sio_puts("3\n");
	sleep(1);
}


int main(){
	
	if((signal(SIGINT, handler)) == SIG_ERR){
		perror("Signal Error\n");
	}

	//pause();
	
	return 0;
}
