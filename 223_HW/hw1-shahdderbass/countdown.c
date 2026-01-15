/*
Shahd Derbass
2231172085
HW1
Q7
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void handler(){
	const char error[] = "\nLaunch aborted!\n";
	write(1, error, strlen(error));
	_exit(1);
	//sio_error("Launch aborted!\n");		//To safely output the message then exit
}

int main(int argc, char *argv[]){
	if(argc < 2){
		printf("Error, Invalid number of arguments.\n");
		return -1;
	}
	if(argv[1][0] == '\0'){
		printf("Error, Empty string.\n");
		return -1;
	}
	int sec = atoi(argv[1]);
	
	signal(SIGINT, handler);

	for(int i = 1; i <= sec; i++){
		printf("T-%d seconds\n", i);
		sleep(1);
	}
	printf("Launch initiated!\n");

	return 0;
}
