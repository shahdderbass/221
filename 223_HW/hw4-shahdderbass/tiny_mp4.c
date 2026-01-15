#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "csapp.h"
#define MAX 8000

void serve_mp4(int fd, char *filename) {
    char buf[MAX];
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        sprintf(buf, "HTTP/1.0 404 Not Found\r\n");
        Rio_writen(fd, buf, strlen(buf));
        sprintf(buf, "Server: Tiny MP4 Server\r\n\r\n");
        Rio_writen(fd, buf, strlen(buf));
        return;
    }

    fseek(fp, 0, SEEK_END);
    long filesize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Server: Tiny MP4 Server\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-Type: video/mp4\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-Length: %ld\r\n", filesize);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Accept-Ranges: bytes\r\n\r\n");
    Rio_writen(fd, buf, strlen(buf));

    size_t n;
    while ((n = fread(buf, 1, MAX, fp)) > 0) {
        Rio_writen(fd, buf, n);
    }

    fclose(fp);
}

int main() {
    char buffer[MAX];
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket error");
        return 1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8081);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind error");
        return 1;
    }

    if (listen(server_fd, 5) < 0) {
        perror("Listen error");
        return 1;
    }

    printf("Tiny MP4 Server running on port 8081...\n");
    fflush(stdout);

    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    while (1) {
        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        if (client_fd < 0) {
            perror("Accept failed");
            continue;
        }

       
       int n = read(client_fd, buffer, MAX-1);
        if (n <= 0) {
            close(client_fd);
            continue;
        }
        buffer[n] = '\0';

        if (strstr(buffer, "butterflyvideo.mp4")) {
            serve_mp4(client_fd, "./videos/butterflyvideo.mp4");
        }
	
        close(client_fd);
    }

    close(server_fd);
    return 0;
}

