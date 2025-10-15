#include <stdio.h>
#include <signal.h>

void handler(){
	printf("Signal Caught\n");
}
int main(){

	signal(SIGUSR1, handler);

	raise(SIGUSR1);

	return 0;
}
