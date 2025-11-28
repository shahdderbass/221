#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        return 1;
    }

    int port = atoi(argv[1]);
    int listenfd, connfd;
    struct sockaddr_in serveraddr, clientaddr;
    socklen_t clientlen = sizeof(clientaddr);

    // Create socket
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) {
        perror("Socket error");
        return 1;
    }

    // Bind
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = INADDR_ANY;
    serveraddr.sin_port = htons(port);

    if (bind(listenfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0) {
        perror("Bind error");
        return 1;
    }

    // Listen
    listen(listenfd, 5);
    printf("Server listening on port %d...\n", port);

    while (1) {
        connfd = accept(listenfd, (struct sockaddr *)&clientaddr, &clientlen);
        if (connfd < 0) {
            perror("Accept error");
            continue;
        }

        pid_t pid = fork();

        if (pid == 0) {
            // Child process
            close(listenfd); 

            char buffer[200];
            snprintf(buffer, sizeof(buffer),
                     "Hello from process PID: %d\n", getpid());

            write(connfd, buffer, strlen(buffer));

            sleep(5);

            close(connfd);
            exit(0);
        }
        else if (pid > 0) {
            // Parent process
            close(connfd); 
        }
        else {
            printf("Fork failed!\n");
        }
    }

    return 0;
}

