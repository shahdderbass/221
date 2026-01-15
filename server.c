#include "csapp.h"
#include "message.h"
#include "client_type.h"
#include "user_status.h"
#include "connection_manager_fixed.h"
// SERVER

client_t **client_list = NULL;		// dynamic array of pointers to clients using our own client struct, starts empty
size_t  client_count = 0;			// size of client list
						 
int listenfd;


void signal_handler(int sig) {
    char answer;
    const char* signal_name;

    if(sig == SIGINT) {
        signal_name = "SIGINT";   
    } else if (sig == SIGTSTP){
        signal_name = "SIGTSTP";
    }

    printf("\n[%s] Server shutdown requested.\n", signal_name);
    printf("Type 'y' to confirm shutdown, anything else to cancel: ");
    fflush(stdout);

    answer = getchar();
    while (getchar() != '\n'); 

    if (answer == 'y' || answer == 'Y') {
        printf("\n=== SERVER SHUTDOWN INITIATED ===\n");

        printf("Notifying %zu clients...\n", client_count);
        
        //  shutdown message
        const char* shutdown_msg = 
             "*** Server Notification ***\n"
             "The server is shutting down.\n"
             "You will be disconnected in 3 seconds.\n"
             "Thank you!\n"
             "\n";
                        
        
        // Send shutdown message to all clients
        for (size_t i = 0; i < client_count; i++) {
            if (client_list[i] && client_list[i]->fd > 0) {
                write(client_list[i]->fd, shutdown_msg, strlen(shutdown_msg));
                // Flush and shutdown the socket
                shutdown(client_list[i]->fd, SHUT_RDWR);
            }
        }

        sleep(3);  // Wait 3 seconds 

        printf("Closing all connections...\n");
        
        // Close all client sockets
        for (size_t i = 0; i < client_count; i++) {
            if (client_list[i] && client_list[i]->fd > 0) {
                close(client_list[i]->fd);
                client_list[i]->fd = -1;
            }
        }

        printf("Server shutdown complete. Goodbye!\n");
        close(listenfd);
        exit(0);
    } else {
        printf("Shutdown cancelled. Server continues.\n");
    }
}

void sigpipe_handler(int sig) {
    const char msg[] ="Warning: Tried to write to a disconnected client (SIGPIPE caught)\n";
    write(STDERR_FILENO, msg, sizeof(msg) - 1);
} // eg when a client disconnects, but server tries to send them a message and without handling it server crashes when a client disconnects unexpectedly
   

/*
// Function to allocate each client's fields when client connects
client_t *createClient(int connfd){
	client_t *c = Malloc(sizeof(client_t));
	if(!c)
		return NULL;

	// allocate username and password
	c->username = Malloc(MAXLINE);
	c->password = Malloc(MAXLINE);
	if(!c->username || !c->password){
		Free(c->username);
		Free(c->password);
		Free(c);
		return NULL;
	}

	// username and password will initially be empty
	c->username[0] = '\0';
	c->password[0] = '\0';

	c->fd = connfd;
	c->logged_in = 0;

	// allocate RIO buffer dynamically
	c->rio = Malloc(sizeof(rio_t));
	if(!c->rio){
		Free(c->username);
		Free(c->password);
		Free(c);
		return NULL;
	}

	// initialize RIO
	Rio_readinitb(c->rio, connfd);

	return c;
}
*/

// Thread routine, outputs menu for each client thread, connection/communication with the system begins
void *client_thread(void *arg){
    	client_t *c = (client_t *)arg;
	
	int fd = c->fd;

    	message_handler(c);

    	notify_userLeft(c);			// to broadcast to all clients that client c left the chatroom

	// free client's information when they disconnect
/*	Close(c->fd);
	Free(c->username);
	Free(c->password);
	Free(c->rio);
	Free(c);		// free client's struct         or i just call the function disconnect_client(c->fd);
	return NULL;*/
	close(fd);
	disconnect_client(fd);
	return NULL;
}


int main(int argc, char *argv[]){

	if(argc != 2){
		fprintf(stderr, "Usage %s", argv[0]);
		exit(1);
	}

	int port = atoi(argv[1]);		// saving port

	int connfd;

	struct sockaddr_in serveraddr, clientaddr;
	socklen_t clientlen = sizeof(clientaddr);

	
        signal(SIGINT, signal_handler);
	signal(SIGTSTP, signal_handler);
        signal(SIGPIPE, sigpipe_handler);

	//creating a socket
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if(listenfd < 0){
		perror("Socket creation failed.");
		exit(1);
	}

	// binding socket to port

	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;        	// IPv4
    	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);  	// Listen on all interfaces
    	serveraddr.sin_port = htons(port);		// set port
	
	bind(listenfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
		
	// listening for connection requests

	listen(listenfd, 10); 			// backlog = 10
		
	printf("Server is running on port %d...\n", port);

	
	// accepting any connect rquest recieved from client

	while(1){

	       // Creating a new client struct for each client that connects
		connfd = accept(listenfd, (struct sockaddr*)&clientaddr, &clientlen);	// client socket discriptor
		if(connfd < 0){
		     perror("Accept failed.");
                     continue;
                }
		

		if(fork() == 0){
			close(listenfd);

			// logic
			
			printf("connected\n");
			
			client_t *c = add_client(connfd);
			if(!c){
				perror("Client struct allocation failed.");
				close(connfd);
				continue;
			}
			message_handler(c);

			close(connfd);
			exit(0);			
		}
		

		//printf("Client connected from %s:%d\n",
        	//inet_ntoa(clientaddr.sin_addr),
        	//ntohs(clientaddr.sin_port));

		/*client_t *c = add_client(connfd);
		if(!c){
			perror("Client struct allocation failed.");
			close(connfd);
			continue;
		}*/

		// Creating a thread for each client
		//pthread_t tid;
                //Pthread_create(&tid, NULL, client_thread, c); 
                //pthread_detach(tid);    	         	
                          
              	
	}
	//close(listenfd);

	return 0;
}
