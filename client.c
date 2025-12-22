/* 
 * File: client.c
 * Description:  * This file implements the client-side logic of the chat and file sharing system.
 * It handles user interaction through menus and commands, manages communication
 * with the server, and processes incoming server responses in real time.
 *
 * The client supports live chat, offline messaging, broadcasts, file upload and
 * download operations, and graceful disconnection handling. A dedicated listener
 * thread is used to continuously receive and display server messages while the
 * main thread processes user input.
 *
 * Author: [Batool Alenezi] -  handle_exit , handle_client_signal , called send and download functions in the clientloop based on user commands, i added logic to receive and process server responses related to file operations
 * Author: [Shahd Derbass] -  *  - Implemented the main client logic and server connection. I designed the general workflow of clientInputLoop and server listener, I contributed by handling user input, menu navigation, and command processing in the clientInputLoop, plus processing live chat mode, offlien chat mode and anything related to chatting in the server listener, and how the server responds.
 * Date : 21/12/2025
 */

#include "UserI.h"
#include "client_menu.h"
#include "csapp.h"
#include "Files.h"
#include "send_msg_client.h"
#include "client_Files.h"

int sock_fd;

volatile int in_chat = 0;
volatile int in_submenu = 0;
volatile int in_offline_chat = 0;
volatile int suppress_prompt = 0;

pthread_mutex_t chat_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t ui_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t file_mutex = PTHREAD_MUTEX_INITIALIZER;

/*
 * purpose : gracefully exit the client program
 * parameters : sock_fd - socket file descriptor to close
 * rio - rio buffers struct
 * returns: void
 * decription: - Prints goodbye message
 *   - Closes socket connection if open
 *   - Terminates program with exit code 0
*/
void handle_exit(int sock_fd, rio_t *rio) {
    printf("Exiting... Goodbye!\n");

     // Close socket connection
    if (sock_fd > 0) {
        close(sock_fd);
    }

    exit(0);
}

char *strip_prompt(char *msg) {
    while (*msg == '>' || *msg == ' ')
        msg++;
    return msg;
}



/*
clientInputLoop() is responsible for reading user input from the terminal and deciding how it should be handled.
It checks whether the client is currently in live chat mode or normal menu mode.
If the client is in live chat, all input is sent directly to the server as chat messages.
If the client is not in chat mode, the function distinguishes between menu choices, commands (starting with /), and invalid input.
menu choices trigger the appropriate client-side menu functions, while commands are sent to the server for processing.
The function also performs basic input validation to prevent invalid or unexpected input from being processed.
*/

void *clientInputLoop(int sock_fd, rio_t *rio) {
	char *buffer = malloc(MAXLINE);
	if(!buffer){
		perror("Memory allocation failed.");
		return NULL;	
	}
	
	while (1) {

		pthread_mutex_lock(&chat_mutex);
    		int chatting = in_chat;
    		int offline = in_offline_chat;
		pthread_mutex_unlock(&chat_mutex);
	
				
		if(offline || chatting){
			printf("> ");
			fflush(stdout);
		}
		
		
		if (!fgets(buffer, MAXLINE, stdin))
        		break;

    		buffer[strcspn(buffer, "\n")] = 0;
    		
		// re-print prompt when user presses enter
		if (buffer[0] == '\0'){
			printf("> ");
			fflush(stdout);
			continue;
		}


    		// chat mode, which means everything is a message 
    		if (chatting || offline) {
        		if (strncmp(buffer, "/end",4) == 0) {
        	    		suppress_prompt = 1;   // wait for server message
        		}
			sendMessage(sock_fd, buffer);
        		continue;
    		}
                 // FILE SEND COMMAND: /send <filename>
                  if (strncmp(buffer, "/send ", 6) == 0) {
                   char *filename = malloc(LEN_SMALL);
                   if (!filename) continue;

                   if (sscanf(buffer + 6, "%255s", filename) != 1) {
                       printf("Usage: /send <filename>\n");
                       free(filename);
                    } else {
                // this is a client-side function that sends UPLOAD command
                       send_file(sock_fd, filename);
                    }
                    printf("> ");
                    fflush(stdout);
                    continue;  
                }
            // FILE DOWNLOAD COMMAND: /download <filename>
		if (strncmp(buffer, "/download ", 10) == 0) {
                   char *filename = malloc(LEN_SMALL);
                   if (!filename) continue;

                   if (sscanf(buffer + 10, "%255s", filename) != 1) {
                       printf("Usage: /download <filename>\n");
                       free(filename);
                    } else {
                // this is a client-side function that sends DOWNLOAD command
                       download_file(sock_fd,filename);
                     }
                      printf("> ");
                      fflush(stdout);
                      continue;  
                 }

		// handle /return FIRST (works in submenu)
if (strcmp(buffer, "/return") == 0) {
    in_submenu = 0;
    showMainMenu(sock_fd, rio);
    continue;
}

// other commands go to server (only when not in submenu)
if (buffer[0] == '/' && !in_submenu) {
    sendMessage(sock_fd, buffer);
    continue;
}
	/*
    		// commands go to server
   		 if (buffer[0] == '/' && !in_submenu) {
	    		if (strcmp(buffer, "/return") == 0) {
		    		in_submenu = 0;
				showMainMenu(sock_fd, rio);
                    		continue;
	    		}       
            		sendMessage(sock_fd, buffer);
            		continue;
     		}
	*/

    		// menu, which takes numbers only
    		
		 if (!in_submenu && strlen(buffer) == 1 && isdigit(buffer[0])) {
        		int choice = buffer[0] - '0';
        		switch (choice) {
            			case 1:
					in_submenu = 1;
					authRegisterMenu(sock_fd,rio); 
					break;
            			case 2: 
					in_submenu = 1;
					authLoginMenu(sock_fd); 
					break;
            			case 3: 
					in_submenu = 1;
					authLogoutMenu(); 
					break;
            			case 4: 
					in_submenu = 1;
					fileTransferMenu(); 
					break;
            			case 5: 
					in_submenu = 1;
					request_List(sock_fd); 
					break;
            			case 6: 
					in_submenu = 1;
					messagingSessionMenu(); 
					break;
            			case 7: 
					handle_exit(sock_fd, rio); 
					return;
            			default:
                			printf("Invalid choice. Try again.\n");
        		}
        	continue;
    		}
	
		// anything else -> message (offline chat)
		sendMessage(sock_fd, buffer);
	
	}
	free(buffer);
	return;
}



/*
This function runs in a separate client-side thread.
It continuously listens for messages sent by the server 
and prints them immediately to the terminal.

Without this thread, server messages such as broadcasts,
chat messages, and login notifications would not be
displayed until the user enters a command.
*/
void *server_listener(void *arg) {
    	rio_t *rio = (rio_t *)arg;
    	char *buf = malloc(MAXLINE);
        

	if(!buf) return NULL;
        
	int in_file_transfer = 0;

    	while (1) {
        	ssize_t n = Rio_readlineb(rio, buf, MAXLINE);
        	if (n <= 0) break;
        	if (strcmp(buf, "\n") == 0) continue;

        	int chat_started = 0;
        	int chat_ended   = 0;
		int offline_chat_started = 0;
		int offline_chat_ended = 0;
		int print_offline_header = 0;

		// detect different states //

		// offline chat started
		if (strstr(buf, "User is offline.") != NULL) {
    			offline_chat_started = 1;
		}
		
		// offline chat ended
		if (strstr(buf, "Offline chat ended") != NULL) {
    			offline_chat_ended = 1;
		}


        	// Handle END_OF_LIST marker (ignore)
        	if (strcmp(buf, "END_OF_LIST\n") == 0) {
                   continue;  
                }
               // Handle FILE_BEGIN - start of file download
                if (strncmp(buf, "FILE_BEGIN ", 11) == 0) {
                     char *cmd = malloc(32);
                     char *fname = malloc(LEN_SMALL);
                     long filesize;

                     if (!cmd || !fname) {
                         free(cmd);
                         free(fname);
                         continue;
                      }

                   sscanf(buf, "%31s %255s %ld", cmd, fname, &filesize);

		   printf("\nDownloading '%s' (%ld bytes)...\n", fname, filesize);
                   FILE *fp = fopen(fname, "wb");
                   if (!fp) {
                   
		      printf("ERROR: Cannot create file '%s'\n", fname);	   
		      free(cmd);
                      free(fname);
                      continue;
                     }

                  char *filebuf = malloc(CHUNK_SIZE);
                  if (!filebuf) {
                     fclose(fp);
                     free(cmd);
                     free(fname);
                     continue;
                    }

                    long received = 0;
                     // Download file data
                    while (received < filesize) {
                        size_t to_read = (filesize - received < CHUNK_SIZE) ? (filesize - received): CHUNK_SIZE;

                        ssize_t r = Rio_readnb(rio, filebuf, to_read);// Returns items written
                        if (r <= 0) break;

                        fwrite(filebuf, 1, r, fp);// Returns items written
                        received += r;

		        if (received % (1024 * 1024) == 0 || received == filesize) {
                        double percent = (double)received / filesize * 100;
                        printf("Progress: %ld/%ld bytes (%.1f%%)\n",
                               received, filesize, percent);
                    }
                     }

                        fclose(fp);
                        free(filebuf);

                        // read FILE_END marker
                        Rio_readlineb(rio, buf, MAXLINE);

                        if (received == filesize) {
                            printf("\nDownload complete: %s (%ld bytes)\n", fname, filesize);
                        } else {
                           printf("\nDownload failed\n");
                           remove(fname);
                          }

                       free(cmd);
                       free(fname);
                       continue;
	            
                }
        
                if (strcmp(buf, "FILE_END\n") == 0) {
                   // Don't print this either
		     continue;
                 }

		 if (in_file_transfer) {
                    continue;  // Don't print file content
                  }
                    
		 // Handle server shutdown notification
		if (strstr(buf, "*** Server Notification ***") != NULL) {
                    printf("\n%s", buf);
                   // Read 4 more lines of notification
		    for (int i = 0; i < 4; i++) {
                       if (Rio_readlineb(rio, buf, MAXLINE) > 0) {
                          printf("%s", buf);
                        }
                    }
		 

                  sleep(3);

		  if (sock_fd > 0) {
                    close(sock_fd);
                  }
                    exit(0);

             }

		// chat started
		if (strstr(buf, "Live chat started") != NULL ||
            	    strstr(buf, "A user started a live chat") != NULL) {
            		chat_started = 1;
       	 	}

        	// live chat ended
		if (strstr(buf, "Chat ended") != NULL ||
            	    (strstr(buf, "Chat with") != NULL && strstr(buf, "ended") != NULL) ||
            	    strstr(buf, "User went offline. Chat ended") != NULL) {
            		chat_ended = 1;
		}

        	// update the state
		pthread_mutex_lock(&chat_mutex);
        	if (chat_started) in_chat = 1;
        	if (chat_ended)   in_chat = 0;
        	if (offline_chat_started && !in_offline_chat){
			in_offline_chat = 1;
			print_offline_header = 1;	
		}	
		if (offline_chat_ended)   in_offline_chat = 0;
		pthread_mutex_unlock(&chat_mutex);

		// print output
        	if (chat_started) {
            		printf("\n=== Live Chat ===\n");
        	}
		
		if (print_offline_header) {
    			printf("\n=== Offline Chat ===\n");
    			printf("Messages will be delivered when the user logs in.\n");
    			printf("Type messages. Use /end to stop.\n");
		}
		pthread_mutex_lock(&chat_mutex);
		int chatting = in_chat;
		pthread_mutex_unlock(&chat_mutex);
		
		printf("%s", buf);

		if (!chatting) {
    			printf("> ");
		}
		
		fflush(stdout);

	}
	free(buf);
	return NULL;
}

/*
 * Purpose: Handle SIGINT (Ctrl-C) signal for graceful client shutdown
 * Parameters: sig - signal number (always SIGINT)
 * Returns: void (calls exit(0) which never returns)
 * Description:
 *   - Sends /quit command to server before disconnecting
 *   - Closes socket connection
 *   - Exits client program cleanly
 */
void handle_client_signal(int sig) {
    	const char msg[] ="\nCtrl-C detected! Disconnecting from server...\n";
    	write(STDERR_FILENO, msg, sizeof(msg) - 1);
    	sendMessage(sock_fd, "/quit");
	close(sock_fd);
        exit(0);
}


int main(int argc, char *argv[]) {
	if (argc != 3) {
        	fprintf(stderr, "Usage: %s <server IP> <port>\n", argv[0]);
        	exit(1);
    	}

    	rio_t rio;
    	struct sockaddr_in serveraddr;

    	char *serverIP = argv[1];		// saving IP address
    	int port = atoi(argv[2]);		// saving port

	signal(SIGINT, handle_client_signal);
	while(1){
	// create socket
    	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    	if (sock_fd < 0) { perror("Socket creation failed"); exit(1); }

	// configure server address
    	memset(&serveraddr, 0, sizeof(serveraddr));
    	serveraddr.sin_family = AF_INET;
    	serveraddr.sin_port = htons(port);
    	Inet_pton(AF_INET, serverIP, &serveraddr.sin_addr);

	// connect to server
    	connect(sock_fd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)); 
        
    	Rio_readinitb(&rio, sock_fd);

	pthread_t listener_tid;
	pthread_create(&listener_tid, NULL, server_listener, &rio);
	pthread_detach(listener_tid);
   	
    	printf("Connected to server.\n");
	
        showMainMenu();
	
	clientInputLoop(sock_fd,&rio);

	
	close(sock_fd);
	}
    	return 0;
}
