#include <stdio.h>
#include <fcntl.h>
#include "csapp.h"

#define len 100

int main(int argc, char *argv[]){
	if(argc < 2){
		fprintf(stderr, "Invalid number of arguments.\n");
		return -1;
	}

	int fd1 = open(argv[1], O_RDONLY);
	if(fd1 < 0){
		fprintf(stderr, "Error opening file %s.\n", argv[1]);
		return -1;
	}

	int fd2 = open("output.txt", O_CREAT | O_WRONLY, 0644);
	if(fd2 < 0){
                 fprintf(stderr, "Error opening file outpput.txt.\n");
                 return -1;
 	}
	
	char buf[len];
	rio_t rp;
	ssize_t nbytes = 0;
	int rc;

	rio_readinitb(&rp, fd1);
	if (rc == -1) {
        	perror("rio_readinitb failed");
        	close(fd1);
        	close(fd2);
        	return -1;
    	}	
	while((nbytes = rio_readlineb(&rp, buf, len)) != 0){
		int line_len = nbytes;
	if (buf[nbytes - 1] == '\n')
		line_len = nbytes - 1;
		for(int i = 0; i < line_len/2; i++){
			char tmp = buf[i];
			buf[i] = buf[line_len - 1 - i];
			buf[line_len - 1 - i] = tmp;
		}
		rio_writen(fd2, buf, nbytes);
	}


	if(close(fd1)<0){
		fprintf(stderr, "Error closing file %s.\n", argv[1]);
		return -1;
	}
	if(close(fd2)<0){
                 fprintf(stderr, "Error closing file output.txt.\n");
                 return -1;
         }
}
