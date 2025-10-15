#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

int count = 0;

void tick(){
	count++;
	printf("Tick %d\n", count);

	if(count < 5)
		alarm(1);
}

int main(){
	signal(SIGALRM, tick);
	alarm(1);
	
	while(count < 5)
		pause();
	
	printf("Exiting...\n");

	return 0;
}	
