#include <stdio.h>
#include "csapp.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	if(argc != 3){
		fprintf(stderr, "Usage: %s\n", argv[0]);
		return 1;
	}

	int port = atoi(argv[2]);

	struct sockaddr_in serveraddr;
	int sock_fd;

	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(sock_fd < 0){
		perror("Socket creation failed.");
		return 1;
	}

	memset(&serveraddr, 0, sizeof(serveraddr));	// clear structure
	serveraddr.sin_family = AF_INET;		// IPv4
	serveraddr.sin_port = htons(port);		// setting the port
	
	char *ipAddr = argv[1];
	if(inet_pton(AF_INET, ipAddr, &serveraddr.sin_addr) <= 0){
		perror("Invalid IP address.");
		return 1;
	}
	

	if( connect(sock_fd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0 ){
		perror("Connection failed.");
		return 1;
	}

	printf("Connected to server.\n");

	rio_t rio;
	rio_readinitb(&rio, sock_fd);

	char *buffer = malloc(MAXLINE);	

	while(1){
		ssize_t n = rio_readlineb(&rio, buffer, MAXLINE);		// reads line from server
		
		if(n <= 0){
			printf("Server ended connection.\n");
			break;
		}

		printf("Message received from server: %s", buffer);			// prints server's message to screen
	}
	free(buffer);
	if(close(sock_fd) < 0){
		perror("Socket failed to close.");
		return 1;
	}

	return 0;
}
