#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#define MAX_BUFF_LEN 256


void errorMessage(char *msg) {

	perror(msg);
	exit(1);

}

int createSocket();
int readMessage(int, char*);
int writeMessage(int, char*);
int readMessageFromClient(int clientSock, char (*buff)[MAX_BUFF_LEN]);
int writeMessageToClient(int clientSock, char (*buff)[MAX_BUFF_LEN]);

int main(int argc, char* argv[]) {

	int	checkSocketConnectionSuccessful, portNo, newSocketForClient;
	socklen_t clientLen;
	char 	buffer[MAX_BUFF_LEN];
	struct 	sockaddr_in server_addr, client_addr;


	/* Do a basic error check if the command line argument is not complete throw an error am using
         * exit(1) to flag an error which is un successful termination */
	if (argc < 2) {

                fprintf(stderr,"ERROR, no port was specified in your command line argument\n");
                exit(1);

        }


	// Create a socket and check if it was successful or not
	checkSocketConnectionSuccessful = createSocket();

	if(checkSocketConnectionSuccessful < 0) {
		errorMessage("Error opening socket");
	}

	/* Before I use my server address I will like to clear and
	 * release it of anything that was using it previously so I
	 * can use it from a fresh and free of all junks LOL */
	bzero((char*) &server_addr, sizeof(server_addr));


	// Now I want to store my port no which I will retrieve from my command line argument
	portNo = atoi(argv[1]);

	// Initialize my server address
	server_addr.sin_family 		= AF_INET;
	server_addr.sin_addr.s_addr 	= INADDR_ANY;
	server_addr.sin_port 		= htons(portNo);

	// Bind my socket
	if(bind(checkSocketConnectionSuccessful, (struct sockaddr *) &server_addr,sizeof(server_addr)) < 0)	{
		errorMessage("Error Binding the IP address, try again! ");
	}

	// Listen for connection, the number 10 is the maximum no. of clients the server should allow
	listen(checkSocketConnectionSuccessful, 10);

	clientLen = sizeof(client_addr);


	// Accept connection when client sends a request
	newSocketForClient = accept(checkSocketConnectionSuccessful, (struct sockaddr *) &client_addr, &clientLen);

	if(newSocketForClient < 0) {

		errorMessage("Error accepting connection\n");
	}

	// Just to make sure my buffer is all clear and set to use I will clear it again just to be safe
	bzero(buffer, 256);

	// read message received from client
	readMessageFromClient(newSocketForClient, &buffer);

	// send message to client
	writeMessageToClient(newSocketForClient, &buffer);

	return 0;
}



int createSocket() {
	return socket(AF_INET, SOCK_STREAM, 0);
}


int readMessageFromClient(int clientSock, char (*buff)[MAX_BUFF_LEN]) {

	int n;

	n = read(clientSock, buff, MAX_BUFF_LEN);

	if(n < 0) {
		errorMessage("Error reading from the client socket");
		return -1;
	}

	else {
		printf("Message received From the client: %s\n", (char *)buff);
		return 0;
	}

}

int writeMessageToClient(int clientSock, char (*buff)[MAX_BUFF_LEN]) {

	int n;

	char *message = "I got your message";
	n = write(clientSock, message, sizeof(message));

	if(n < 0) {
		errorMessage("Error writing to the client socket");
		return -1;
	}

	return 0;
}
