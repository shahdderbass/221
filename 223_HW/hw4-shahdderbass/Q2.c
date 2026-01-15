#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Invalid number of arguments.\n");
        return 1;
    }

    char *ip = argv[1];
    int sock;
    struct sockaddr_in addr;

    FILE *fp = fopen("open_ports.txt", "w");
    if (!fp) {
        perror("File open failed");
        return 1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);

    for (int port = 0; port <= 65535; port++) {
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) continue;

        addr.sin_port = htons(port);

        if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) == 0) {
            printf("Open port: %d\n", port);
            fprintf(fp, "%d\n", port);
        }

        close(sock);
    }

    fclose(fp);
    return 0;
}

