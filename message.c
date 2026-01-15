#include "message.h"

pthread_mutex_t client_list_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t chat_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t array_lock = PTHREAD_MUTEX_INITIALIZER;


extern client_t **client_list;
extern size_t client_count;

void log_and_store_message(client_t *c, const char *msg);
void broadcast_messages(client_t *c, const char *message);
void readOfflineMessages(client_t *c, const char* username);
void clearOfflineMessages(client_t *c);
void handleOfflineMenu(client_t *c);
void forward_live_message(client_t *sender, const char *msg);
void end_chat(client_t *c);
void start_live_chat(client_t *A, client_t *B);
void deleteOfflineMessages(client_t *c, const char *username);

// Function to count logged in clients
int countLoggedInUsers() {
    	int count = 0;
    	for (size_t i = 0; i < client_count; i++) {
        	if (client_list[i] && client_list[i]->logged_in)
            		count++;
    	}
    	return count;
}

int valid_username(const char *username) {
    if (!username || strlen(username) == 0) return 0;
    for (size_t i = 0; username[i]; i++) {
        if (isalpha((unsigned char)username[i])) return 1;  // Found letter → VALID
    }
    return 0;  // No letters → then invalid
}

int valid_password(const char *pw) {
    if (!pw) return 0;
    size_t len = strlen(pw);
    return (len >= 6 && len <= 12);  
}

// Server side menu, handles client's input and calls appropriate functions
void message_handler(client_t *c){
        char *buffer = malloc(MAXLINE);
	if(!buffer){
	//char *current_target_user = NULL;
           perror("Memory allocation failed.");
           return;
        }

         ssize_t nbytes;

         // while loop for reading from client
         while(1){
		 //sendMessage(c->fd, ">");
                nbytes = receiveMessage(c->rio, buffer);                // read message from client
                if (nbytes <= 0) {
			if(c->logged_in){
				notify_userLeft(c);
				c->logged_in = 0;
			}
			break;
		}

               buffer[strcspn(buffer, "\n")] = '\0';                          // null terminate

               // ignore empty input when user presses Enter
               if (buffer[0] == '\0') {
//		       sendMessage(c->fd, ">");
                        continue;
               }

	       c->last_activity = time(NULL);
				 	        
		// to format and store every message sent by (logged in) user to chat_history.txt
		//if (c->logged_in && buffer[0] != '/') {
    		log_and_store_message(c, buffer);
		//}

		//====== COMMAND HANDLING ======// 

		if (strncmp(buffer, "/end", 4) == 0) {
			// end live chat if active
    			if (c->chat_partner != NULL) {
        			end_chat(c);
        			continue;
    			}

    			// end offline chat mode
    			if (c->offline_target != NULL) {
        			free(c->offline_target);
        			c->offline_target = NULL;
        			sendMessage(c->fd, "Offline chat ended.\n");
        			continue;
    			}

    			sendMessage(c->fd, "You are not in a chat.\n");
			continue;
		}
			

		else if (strcmp(buffer, "/return") == 0) {
    		// client is leaving a menu, not an error
    			continue;
		}
		// REGISTER
		
		else if(strncmp(buffer, "/register", 9) == 0){

			char *username = malloc(LEN_SMALL);
			char *password = malloc(LEN_SMALL);
			if(!username || !password){
				free(username);
				free(password);
				perror("Memory allocation failed.");
				continue;
			}
/*			// extract username only
    			if(sscanf(buffer, "/register %255s", username) != 1){
        			sendMessage(c->fd, "Usage: /register <username>\n");
        			continue;
    			}

    			// receive hidden password from client
    			if(receiveMessage(c->rio, password) <= 0){
        			sendMessage(c->fd, "Error reading password.\n");
        			continue;
    			}
			password[strcspn(password, "\n")] = '\0';

			*/
 		        if(sscanf(buffer, "/register %255s %255s", username, password) != 2){
      		   		sendMessage(c->fd, "Usage: /register <username> <password>\n");
				free(username);
				free(password);
				continue;//continue the program
    			}


			if (!valid_username(username) || !valid_password(password)) {
 				   sendMessage(c->fd, "Username needs 1+ letter. Password 6-12 chars.\n");
    					free(username); free(password); continue;
			}
	      		int res = register_user(username, password, c->fd);//func take these arguments

		      if (res == 1) {
                		sendMessage(c->fd, "Registration successful! Please /login to continue.\n");
            		} else if (res == 0) {
                		sendMessage(c->fd, "Username already exists!\n");
            		} else {
             	   		sendMessage(c->fd, "Error registering user.\n");
            		}
		 	free(username);
		 	free(password);
            		continue;
        	}

                //LOGIN
                else if(strncmp(buffer, "/login", 6) == 0){
                        char *username = malloc(LEN_SMALL);
			char *password = malloc(LEN_SMALL);
			if(!username || !password){
				free(username);
				free(password);
				perror("Memory allocation failed.");
				continue;
			}

                        if(sscanf(buffer, "/login %255s %255s", username, password) != 2){
                                sendMessage(c->fd, "Usage: /login <username> <password>\n");
                                free(username);
				free(password);
				continue;
                        }/*
			if(sscanf(buffer, "/login %255s", username) != 1){
                                sendMessage(c->fd, "Usage: /register <username>\n");
                                continue;
                        }

                        // receive hidden password from client
                        if(receiveMessage(c->rio, password) <= 0){
                                sendMessage(c->fd, "Error reading password.\n");
                                continue;
                        }
                        password[strcspn(password, "\n")] = '\0';
*/

		
			if (!valid_username(username) || !valid_password(password)) {
    				sendMessage(c->fd, "Username needs 1+ letter. Password 6-12 chars.\n");
    				free(username); free(password); continue;
			}
                        int res = login_client(username, password, c->fd);//login cases


                        if (res == 1) {
				sendMessage(c->fd, "Login successful!");
                                notify_userJoined(c);
				continue;
			}	

			  else if (res == 0) {
                                sendMessage(c->fd, "Invalid username or password!\n");
                        } else if (res == -2) {
                                sendMessage(c->fd, "You are already logged in on this connection!\n");
                        } else if (res == -3) {
                                sendMessage(c->fd, "This connection is logged as another user. Please /logout first.\n");
                        } else if (res == -5) {
			sendMessage(c->fd, "This user is already logged in from another session.\n");
			} else {
                        	sendMessage(c->fd, "Error logging in.\n");
                        }
			free(username);
			free(password);
                        continue;
                }

                //LOGOUT
                else if (strcmp(buffer, "/logout") == 0) {
                        int was_logged_in = c->logged_in;

			if(was_logged_in){
				notify_userLeft(c);
			}

			int res = logout_client(c->fd);
			if (was_logged_in && res == 1){
        			sendMessage(c->fd, "Logged out successfully.\n");
			}
			else
        			sendMessage(c->fd, "You are already logged out.\n ");
				continue;
                        }
		

		//LIST ACTIVE
                else if(strcmp(buffer, "/list active") == 0) {
                     list_active_users(c->fd);
		    
                }
		//LIST
                else if(strcmp(buffer, "/list") == 0){
                     list_files(c->fd);
               }
		else if (strncmp(buffer, "/search ", 8) == 0) {
                       char *search_term= malloc(LEN_SMALL);
		        sscanf(buffer, "/search %255s", search_term);
		       	search_files(c->fd, search_term);
			free(search_term);
                }

		//METADATA
                else if (strncmp(buffer, "/metadata ", 10) == 0) {
                        char *filename= malloc(LEN_SMALL);
			sscanf(buffer, "/metadata %255s", filename);
			show_file_metadata(c->fd, filename);
			free(filename);
              }

		//CHAT
                else if(strncmp(buffer, "/chat",5) == 0){
                	if (c->chat_partner != NULL) {
    				sendMessage(c->fd, "You are already in a live chat. Type /end first.\n");
    				continue;
			}
			if(c->logged_in == 0){
				sendMessage(c->fd, "Must be logged in to start a live chat.\n");
				continue;
			}
                        char *user = malloc(LEN_SMALL);
                        if(!user) return;

                        if(sscanf(buffer, "/chat %225s", user) != 1){
                                sendMessage(c->fd, "Usage /chat <username>\n");
                                free(user);
                                continue;
                        }

			// prevent chatting with yourself
    			if (c->username && strcmp(c->username, user) == 0) {
        			sendMessage(c->fd, "You cannot chat with yourself.\n");
        			free(user);
        			continue;
    			}


                       	if (!username_exists(user)) {
    				sendMessage(c->fd, "User does not exist.\n");
    				free(user);
    				continue;
			}

                        client_t *B = findUser(user);

			//  user exists but is offline
    			if (B == NULL) {
        			// clear previous offline target
        			if (c->offline_target) {
            				free(c->offline_target);
            				c->offline_target = NULL;
        			}

				c->offline_target = strdup(user);
        			if (!c->offline_target) {
    					sendMessage(c->fd, "Internal error.\n");
				}
				sendMessage(c->fd,"User is offline. Messages will be delivered later.\n");
				free(user);
        			continue;
    			}

			if (B->chat_partner != NULL) {
    				sendMessage(c->fd, "User is currently busy in another chat.\n");
    				free(user);
    				continue;
			}

    			// user exists and is online
    			start_live_chat(c, B);
			// clear any offline state
    			if (c->offline_target) {
    				free(c->offline_target);
    				c->offline_target = NULL;
    			}
			free(user);
                	continue;
		}

                // BROADCAST
                else if(strncmp(buffer, "/broadcast",10) == 0){
			// check if user is logged in -> allow to broadcast	
			if(!c->logged_in){
				sendMessage(c->fd, "Must be logged in to broadcast a message. Please enter /login\n");
				continue;
			}
                        // check if any users are logged in (except sender)
	                        if(countLoggedInUsers() <= 1){
                                sendMessage(c->fd,"No other users online to broadcast to.\n");
                                continue;
                        }
			char *msg = malloc(MAXLINE);
			if(!msg) continue;

    			snprintf(msg, MAXLINE,"\n[BROADCAST] %s: %s\n",c->username, buffer + 11);
                        broadcast_messages(c, msg);
                        sendMessage(c->fd, "Message broadcasted.\n");
                	free(msg);
			continue;
		}
                // VIEW OFFLINE MESSAGES
                else if(strncmp(buffer, "/view messages", 14) == 0){
                        char *user = malloc(LEN_SMALL);
			if(!user){
				perror("Memory allocation failed.");
				return;
			}
			sscanf(buffer + 14, "255%s", user);
			readOfflineMessages(c,user);
                        //clearOfflineMessages(c);
                        free(user);
			continue;
                }
				 
	        else if(strncmp(buffer, "UPLOAD ", 7) == 0) {
                     handle_file_transfer(c, buffer);
                  }
                else if(strncmp(buffer, "DOWNLOAD ", 9) == 0) {
                     handle_file_transfer(c, buffer);
                }	

	        else {
    			// live chatting
    			if (c->chat_partner != NULL) {
				forward_live_message(c, buffer);
        			continue;
    			}
			// offline chatting
    			if (c->offline_target != NULL) {
        			char *formatted = formatMessage(c->username, buffer);
        			if(formatted){
					storeOfflineMessages(c->offline_target,c->username, formatted);
        				free(formatted);
        			}
				continue;
    			}

			sendMessage(c->fd, "Unknown command.\n");
         	}
        }

        	
	free(buffer);
}


// searches all connected clients and returns a pointer to the client wanted
client_t* findUser(const char *username) {
    	if (!username) return NULL;

    	pthread_mutex_lock(&client_list_lock);
    	for (size_t i = 0; i < client_count; i++) {
        	if (client_list[i] &&
		    client_list[i]->logged_in &&
		    client_list[i]->username &&
		    strcmp(client_list[i]->username, username) == 0) {
            		
			client_t *res = client_list[i];
            		pthread_mutex_unlock(&client_list_lock);
            		return res;
        	}
    	}
    	pthread_mutex_unlock(&client_list_lock);
    	return NULL;
}
/*
 *Locks client_list
 *Reallocate client_list
 *Remove a client
 *why it is related to login?it needs to be synchronized so the login doesn't fail
 * */



// Send message to a client through their socket
void sendMessage(int sock_fd, const char* message){
        if(sock_fd < 0 || message == NULL)		// if the socket is invalid or no message, then do not send
                return;
	
	Rio_writen(sock_fd, (void *)message, strlen(message));          // send message to client (server ->client)
	// ensure newline so rio_readlineb() returns on the client side
        if (message[strlen(message) - 1] != '\n') {
                Rio_writen(sock_fd, "\n", 1);
	}

}


// Read a line from a client's rio buffer
ssize_t receiveMessage(rio_t *rp, char *buffer){
        ssize_t nbytes = Rio_readlineb(rp, buffer, MAXLINE);    // read message from client (client->server)
        return nbytes;
}


// Send message from to all clients except sender
void broadcast_messages(client_t *c, const char* message){
	if(!c || !c->logged_in)
		return;

	pthread_mutex_lock(&client_list_lock);	

	for(size_t i = 0; i < client_count; i++){
                client_t *curr = client_list[i];
		if(!curr)
			continue;

		// skip disconnected clients
		if(curr->fd < 0)
			continue;
		
		if(!curr->logged_in)
			continue;

		// skip sender
		if(curr->fd == c->fd)
			continue;

		sendMessage(curr->fd, message);
        }
	
        pthread_mutex_unlock(&client_list_lock);
}


// Format message before broadcasting/storing
char* formatMessage(const char *username, const char* message){
        if(username==NULL || message==NULL){
                perror("Empty username/message.");
                return NULL;
        }

        // build a timestamp
        char *timestamp = malloc(50);
        if(!timestamp){
                perror("Allocation failed for timestamp.");
                return NULL;
        }

        time_t current_time = time(NULL);                       // time function to get current time in seconds
        struct tm *local = localtime(&current_time);                    // convert current_time to a readable time structure

        strftime(timestamp, 50, "%H:%M:%S", local);             // formats *local into [hours:minutes:seconds]

        // final formatted message
        char *formatted = malloc(MAXLINE);
        if(!formatted){
                perror("Allocation failed for formatted message.");
                return NULL;
        }
	
	/*
	if(!username){
		snprintf(formatted, MAXLINE,"[%s] Anonymous User: %s\n", timestamp, message);
	}*/
	else{
        	// combin timestamp, username and message
        	snprintf(formatted, MAXLINE,"[%s] %s: %s\n", timestamp, username, message);             // writes formatted message to 'formatted'
	}

        free(timestamp);
        return formatted;               // returns now usable formatted message
}

// function to format (add timestamp and username) and stores to chat_history.txt
void log_and_store_message(client_t *c, const char *msg) {
    	if (!c || !c->logged_in || !msg)
        	return;

    	char *formatted = formatMessage(c->username, msg);
    	if (!formatted)
        	return;

    	storeMessage(formatted);   // write to file
    	free(formatted);
}




// Send initial "chat started" message to both clients
void sendStartMessages(client_t *A, client_t *B){
        char *msgA = malloc(MAXLINE);
        char *msgB = malloc(MAXLINE);

        if (!msgA || !msgB) {
                free(msgA);
                free(msgB);
                return;
        }

        snprintf(msgA, MAXLINE,"Live chat started with %s!\nEnter \"/end\" to end the chat.\n",B->username);

        snprintf(msgB, MAXLINE,"Incoming live chat from %s...\nEnter \"/end\" to end the chat.\n",A->username);

        sendMessage(A->fd, msgA);
        sendMessage(B->fd, msgB);

        free(msgA);
        free(msgB);
}


void start_live_chat(client_t *A, client_t *B){
	printf("[DEBUG] entering start_live_chat.\n");
	
	if(!A || !B)	return;

	/*
	if (B == NULL) {
        	sendMessage(A->fd, "User is offline. Messages will be delivered later.\n");
		return;
    	}
	*/

    	pthread_mutex_lock(&chat_lock);
	
	// prevent double chat race
	if(A->chat_partner || B->chat_partner){
		pthread_mutex_unlock(&chat_lock);
        	sendMessage(A->fd, "User is already in a chat.\n");
        	return;
	}
	
	// clear offline state
    	if (A->offline_target) {
        	free(A->offline_target);
        	A->offline_target = NULL;
    	}
    	if (B->offline_target) {
        	free(B->offline_target);
        	B->offline_target = NULL;
    	}

    	A->chat_partner = B;
    	B->chat_partner = A;

    	pthread_mutex_unlock(&chat_lock);
	
    	sendMessage(A->fd, "Live chat started. Type /end to stop.\n");
    	sendMessage(B->fd, "A user started a live chat with you. Type /end to stop.\n");
}

void forward_live_message(client_t *sender, const char *msg){	
	if (!sender || !msg) return;

    	pthread_mutex_lock(&chat_lock);
    	client_t *receiver = sender->chat_partner;
    	pthread_mutex_unlock(&chat_lock);

    	if (!receiver || receiver->logged_in == 0 || receiver->fd < 0) {
        	sendMessage(sender->fd, "User went offline. Chat ended.\n");
		end_chat(sender);
		return;
    	}
    	
	if (sender->offline_target) {
        	free(sender->offline_target);
        	sender->offline_target = NULL;
    	}
	
	char *formatted = formatMessage(sender->username, msg);
    	if (!formatted){
		sendMessage(sender->fd, "Internal error formatting message.\n");       
		return;
	}

    	sendMessage(receiver->fd, formatted);
    	//storeMessage(formatted);

    	free(formatted);
}

void end_chat(client_t *c){
	if(!c)	return;

	client_t *partner = NULL;

	pthread_mutex_lock(&chat_lock);

    	partner = c->chat_partner;
    	if (partner) {
        	c->chat_partner = NULL;
        	partner->chat_partner = NULL;
    	}

    	pthread_mutex_unlock(&chat_lock);
	
	// clear offline state
    	if (c->offline_target) {
        	free(c->offline_target);
        	c->offline_target = NULL;
    	}
    	if (partner && partner->offline_target) {
        	free(partner->offline_target);
        	partner->offline_target = NULL;
    	}

	if (partner && partner->logged_in && partner->fd >= 0 && c->username) {
        	char *msg = malloc(MAXLINE);
		if(msg){
        		snprintf(msg, MAXLINE,"Chat with %s ended.\n", c->username);
        		sendMessage(partner->fd, msg);
    			free(msg);
		}
	}

    	if (c->fd >= 0 && partner && partner->username) {
        	char *msg = malloc(MAXLINE);
		if(msg){
        		snprintf(msg, MAXLINE,"Chat with %s ended.\n", partner->username);
        		sendMessage(c->fd, msg);
			free(msg);
		}
    	}
}

// Remove a sender from the receiver's index file
void removeSenderFromIndex(const char *receiver, const char *sender){
    	char *indexPath = Malloc(MAXLINE);
    	char *tempPath  = Malloc(MAXLINE);

    	if (!indexPath || !tempPath) {
        	free(indexPath);
        	free(tempPath);
        	return;
    	}

    	// build paths
    	snprintf(indexPath, MAXLINE,"data/offline_messages/%s_index.txt", receiver);

    	snprintf(tempPath, MAXLINE,"data/offline_messages/%s_index.tmp", receiver);

    	pthread_mutex_lock(&user_file_lock);

    	FILE *in = fopen(indexPath, "r");
    	if (!in) {
        	pthread_mutex_unlock(&user_file_lock);
        	free(indexPath);
        	free(tempPath);
        	return;
    	}

    	FILE *out = fopen(tempPath, "w");
    	if (!out) {
        	fclose(in);
        	pthread_mutex_unlock(&user_file_lock);
        	free(indexPath);
        	free(tempPath);
        	return;
    	}

    	char *line = malloc(256);
    	if (!line) {
        	fclose(in);
        	fclose(out);
        	pthread_mutex_unlock(&user_file_lock);
        	free(indexPath);
        	free(tempPath);
        	return;
    	}

    	// copy all senders EXCEPT the one to remove
    	while (fgets(line, 256, in)) {
        	line[strcspn(line, "\n")] = 0;

        	if (strcmp(line, sender) != 0 && line[0] != '\0') {
            		fprintf(out, "%s\n", line);
        	}
    	}

    	free(line);
    	fclose(in);
    	fclose(out);

    	// replace old index with updated one
    	rename(tempPath, indexPath);

    	pthread_mutex_unlock(&user_file_lock);

    	free(indexPath);
    	free(tempPath);
}

// keeps track of who sent messages, so we can list them later
void addSenderToIndex(const char *receiver, const char *sender){
    	char *path = malloc(LEN_LARGE);
    	if(!path) return;

    	snprintf(path, LEN_LARGE,"data/offline_messages/%s_index.txt", receiver);

    	pthread_mutex_lock(&user_file_lock);

    	FILE *f = fopen(path, "a+");   // create if not exists
    	if (!f) {
        	pthread_mutex_unlock(&user_file_lock);
		free(path);
        	return;
    	}

    	// check if sender already exists
    	char *line = malloc(LEN_MEDIUM);
	if(!line){
		fclose(f);
		pthread_mutex_unlock(&user_file_lock);
        	free(path);
		return;
		
	}
	
	// check if the sender already exists
    	rewind(f);
    	while (fgets(line, LEN_MEDIUM, f)) {
        	line[strcspn(line, "\n")] = 0;
        	if (strcmp(line, sender) == 0) {
			free(line);
            		fclose(f);
            		pthread_mutex_unlock(&user_file_lock);
            		free(path);
			return;
        	}
    	}

    	// add sender
    	fprintf(f, "%s\n", sender);

	free(line);
    	fclose(f);
    	pthread_mutex_unlock(&user_file_lock);
	free(path);
}

// stores offline messages per sender and updates the sender index, and updates the receiver's index file
void storeOfflineMessages(const char *receiver, const char *sender,const char *msg){
    	char *path = malloc(LEN_LARGE);
    	if(!path) return;

    	// each sender has a separate file
    	snprintf(path, LEN_LARGE,"data/offline_messages/%s_%s.txt",receiver, sender);

    	pthread_mutex_lock(&user_file_lock);

    	// append message
    	FILE *f = fopen(path, "a");
    	if (f) {
        	fprintf(f, "%s\n", msg);
        	fclose(f);
    	}

    	pthread_mutex_unlock(&user_file_lock);

    	// register sender in index file
    	addSenderToIndex(receiver, sender);

	free(path);
}


// notify user about offline message senders
int notifyUnreadMessages(client_t *c)
{
    	if (!c || !c->logged_in)
        	return 0;

    	char *path = malloc(LEN_LARGE);
	if(!path) return 0;

    	snprintf(path, LEN_LARGE,"data/offline_messages/%s_index.txt",c->username);

    	pthread_mutex_lock(&user_file_lock);
    	FILE *f = fopen(path, "r");
    	pthread_mutex_unlock(&user_file_lock);

    		if (!f){
			free(path);
        		return 0;   // no offline messages
		}
    		sendMessage(c->fd,"You have unread messages from:\n");

   	char *line = malloc(LEN_SMALL);
    	if(!line){
		fclose(f);
		free(path);
		return 0;
	}
    	int found = 0;

    	while (fgets(line, LEN_SMALL, f)) {
        	line[strcspn(line, "\n")] = 0;
        	if (line[0] != '\0') {
        	    	char *msg = malloc(LEN_MEDIUM);
			if(msg){
            			snprintf(msg, LEN_MEDIUM, "- %s\n", line);
            			sendMessage(c->fd, msg);
            			found = 1;
			}
        	}
    	}

    	fclose(f);
	free(line);
	free(path);

    	if (!found)
        	return 0;

    	sendMessage(c->fd,
        	"\nUse:\n"
        	"/view <username>\n"
        	"/ignore <username>\n"
        	"/return\n");

    	return 1;
}



// Read stored messages and send them to user

void readOfflineMessages(client_t * c, const char *username){
        char *path = malloc(MAXLINE);

	if(!path){
                perror("Memory allocation failed.");
                return;
        }

        snprintf(path, MAXLINE, "data/offline_messages/%s_%s.txt", c->username, username);

        FILE *fptr = fopen(path, "r");
        if (!fptr) {
                sendMessage(c->fd, "No offline messages found.\n");
                free(path);
                return;
        }

        char *line = malloc(MAXLINE);
        if(!line){
                perror("Memory allocation failed.");
                fclose(fptr);
                free(path);
                return;
        }
        sendMessage(c->fd, "\n----- Offline Messages -----\n");

        while (fgets(line, MAXLINE, fptr)) {
		sendMessage(c->fd, line);
	}
	

        sendMessage(c->fd, "-----------------------------\n\n");

        free(line);
        fclose(fptr);
        free(path);
}


// Delete offline message file (when /ignore is entered by client)

void deleteOfflineMessages(client_t *c, const char *username) {
        char *path = malloc(MAXLINE);

        if (!path) {
                perror("Memory allocation failed");
                return;
        }

        snprintf(path, MAXLINE, "data/offline_messages/%s_%s.txt", c->username, username);

        remove(path);

        free(path);

	
	// removes sender from index file
	removeSenderFromIndex(c->username, username);
}


// Handle user commands after login: /view, /ignore
void handleOfflineMenu(client_t *c) {

	char *buffer = malloc(MAXLINE);
        if(!buffer) return;

         while(1){
                if(receiveMessage(c->rio, buffer) <= 0)
                        break;

                buffer[strcspn(buffer,"\n")] = 0;
		
		// /view <username>
                if (strncmp(buffer, "/view ", 6) == 0) {
			char *user = malloc(256);
			if(!user) break;

			sscanf(buffer + 6, "255%s",user);
                        readOfflineMessages(c, user);
			free(user);
               		break;
	       }
		// /ignore <username>
                else if (strncmp(buffer, "/ignore ",8) == 0) {
			char *user = malloc(256);
			if(!user) break;
			
			sscanf(buffer + 8, "255%s",user);
                        deleteOfflineMessages(c, user);
			
			char *msg = malloc(LEN_MEDIUM);
			if(msg){
				snprintf(msg, LEN_MEDIUM, "Offline messages from %s deleted.\n", user);
                        	sendMessage(c->fd, msg);
                        	free(msg);
			}
			free(user);
			break;
                }
		// /return
                else if(strcmp(buffer, "/return") == 0){
			break;
		}
		else {
         		sendMessage(c->fd, "Usage:\n/view <username>\n/ignore <username>\n/return\n");
        	}
                
        }
        free(buffer);
  }


