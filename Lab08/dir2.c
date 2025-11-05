#include <stdio.h>
#include  <dirent.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
	char cwd[PATH_MAX];

	if(getcwd(cwd, sizeof(cwd)) != NULL){
		printf("Current working directory: %s\n", cwd);
	}
	else{
		perror("getcwd() error.\n");
		return 1;
	}

	return 0;
}
