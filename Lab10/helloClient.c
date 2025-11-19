#include <stdio.h>

int main(int argc, char *argv[]){
	if(argc != 3){
		fprintf(stderr, "Usage: %s\n", argv[0]);
		return 1;
	}

	int port = atoi(argv[2]);

	struct sockaddr_in serveraddr;
	int sock_fd;

	sock_fd = socket(AF_INET, SOCK_STREAM, 0);

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port);
	
	if( connect(sock_fd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0 ){
		perror("Connection failed.");
		return 1;
	}



	return 0;
}
