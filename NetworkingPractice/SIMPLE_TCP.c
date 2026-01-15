#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

int main(){
	
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);	// AF_INET = ipv4, SOCK_STREAM = connection endpoint

	struct sockaddr_in addr;

	addr.sin_family = AF_INET;		// protocol family = ipv4 = AF_INET
	addr.sin_addr.s_addr = INADDR_ANY;	// big-endian order
	addr.sin_port = htons(8000);

	bind(server_fd, (struct sockaddr *)&addr, sizeof(addr));

	listen(server_fd, 5);

	printf("Server running on port 8000...\n");

	struct sockaddr_in client;

	socklen_t sock_len = sizeof(client);

	int client_fd = accept(server_fd, (struct sockaddr *)&client, &sock_len);

	char * buffer = malloc(100 * sizeof(char));

	int n = recv(client_fd, buffer, 99, 0);

	buffer[n] = '\0';

	printf("Client said: %s\n", buffer);

	send(client_fd, "Hello!\n", 7, 0);

	close(client_fd);
	close(server_fd);

	return 0;

}
