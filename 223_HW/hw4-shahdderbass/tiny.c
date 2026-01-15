#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "csapp.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#define MAX 1024

extern char **environ;

void serve_dynamic(int fd, char* filename, char* cgiargs){
    char buf[MAX], *emptylist[] = {NULL};

    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Server: Tiny Web Server\r\n");
    Rio_writen(fd, buf, strlen(buf));

    if(Fork() == 0){
        setenv("QUERY_STRING", cgiargs, 1);
        Dup2(fd, STDOUT_FILENO);
        Execve(filename, emptylist, environ);
    }
    Wait(NULL);
}

int main(){
    char buffer[MAX];
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd < 0){
        perror("Socket\n");
        return 1;
    }
    struct sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        perror("Bind\n");
        return 1;
    }
    if(listen(server_fd,5) < 0){
        perror("Listen\n");
        return 1;
    }

    printf("Server running on port 8080...\n");
    fflush(stdout);

    int client_fd;
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    while(1){
	rio_t rio;

        client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        
	Rio_readinitb(&rio, client_fd);

	/*
	int n = read(client_fd, buffer, MAX-1);
        buffer[n] = '\0';

        char *qs = strstr(buffer, "bmi.cgi?");
        if (qs) {
            qs += strlen("bmi.cgi?");

            serve_dynamic(client_fd, "./cgi-bin/bmi.cgi", qs);
        }
	*/
	if (Rio_readlineb(&rio, buffer, MAX) <= 0) {
            close(client_fd);
            continue;
        }
	
	char method[MAX], uri[MAX], version[MAX];

	sscanf(buffer, "%s %s %s", method, uri, version);

	char *qs = strstr(uri, "bmi.cgi?");

	if (qs) {
            qs += strlen("bmi.cgi?");
	    serve_dynamic(client_fd, "./cgi-bin/bmi.cgi", qs);
	}
        close(client_fd);
    }
    close(server_fd);

    return 0;
}
