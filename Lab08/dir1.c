#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

int main(){
	char dirname[256];

	printf("Enter directory name: ");
	scanf("%s", dirname);

	DIR *dir = opendir(dirname);
	if(dir == NULL){
		perror("Unable to open directtory\n");
		exit(1);
	}
	
	struct dirent *entry;

	printf("Contents of directory %s:\n", dirname);
	while((entry = readdir(dir)) != NULL){
		printf("%s\n", entry->d_name);
	}
	
	closedir(dir);

	return 0;
}
