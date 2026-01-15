/*
Shahd Derbass
ID: 2231172085
HW2
Q4
*/

#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
	
	int fd = open("hi.txt", O_RDWR);
	if(fd == -1){
		perror("Error opening file.");
		return 1;
	}

	struct stat st;

	if(fstat(fd, &st) == -1){
		perror("fstat error.");
		close(fd);
		return 1;
	}
	
	size_t new_size = strlen("hello universe\n");

	// to get new file size
	ftruncate(fd,new_size);

	// map file into memory
	char * map = mmap(NULL, new_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if(map == MAP_FAILED){
		perror("Error mapping.");
		close(fd);
		return 1;
	}
	
	strcpy(map, "hello universe\n");

	write(1, map, new_size);

	// unmap file from memory
	munmap(map, new_size);

	if(close(fd) == -1){
		perror("Error closing file.");
		return 1;
	}

	return 0;
}
