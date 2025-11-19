#include <stdio.h>
#include <unistd.h>
#include "csapp.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

//#define PORT 8000
int main(int argc, char *argv[]){
	if(argc != 2){
		fprintf(stderr, "Usage: %s", argv[0]);
		exit(1);
	}
	int port = atoi(argv[1]);
	
	int listen_fd, connfd;
	struct sockaddr_in serveraddr, clientaddr;
	socklen_t clientlen = sizeof(struct sockaddr_in);

	listen_fd = socket(AF_INET, SOCK_STREAM,0);	// create socket
	if(listen_fd < 0){
		perror("Socket creation failed.");
		exit(1);
	}
	
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;		// family is IPv4
	serveraddr.sin_addr.s_addr = INADDR_ANY;	// listen on all interfaces
	serveraddr.sin_port = htons(port);

	// binding socket address with socket descriptor
	if(bind(listen_fd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0){
		perror("Bind failed");
		exit(1);
	}
	
	// listen for any connection request
	if(listen(listen_fd, 10) < 0){
		perror("Listen failed.");
		exit(1);
	}
	
	char *msg1 = "Message 1\n";
	char *msg2 = "Message 2\n";
	char *msg3 = "Message 3\n";

	while(1){
		connfd = accept(listen_fd, (struct sockaddr*)&clientaddr, &clientlen);		// accept connect request
		if(connfd < 0){
			perror("Connection failed.");
			exit(1);
		}
		
		
		rio_writen(connfd,(void*)msg1, strlen(msg1));		// robustly send a message to the accepted client
		rio_writen(connfd,(void*)msg2, strlen(msg2));		// robustly send a message to the accepted client
		rio_writen(connfd,(void*)msg3, strlen(msg3));		// robustly send a message to the accepted client
		close(connfd);
	}
	close(listen_fd);

	return 0;
}
