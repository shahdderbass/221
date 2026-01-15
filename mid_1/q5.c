#include <stdio.h>
#include <signal.h>
#include "csapp.h"
void handler(){
	longjmp(buf,1);
}

jmp_buf buf;

int main(){
	
	signal(SIGINT,handler);

		if(!(sigsetjmp(buf,1))){
			sio_puts("Starting...\n");
		}
		else
			sio_puts("restarting...\n");

		while(1){
			sio_puts("Processing...\n");
			sleep(1);
		}
	

}
