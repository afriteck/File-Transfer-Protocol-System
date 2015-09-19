#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include "ftpDefs.h"

void errorMessage(char *msg) {

        perror(msg);
        exit(0);

}

int main(int argc, char* argv[]) {

	int 	checkSocketConnectionSuccessful, portNo;
	struct 	sockaddr_in server_addr;
	struct	hostent *server;

	char	buffer[MAX_BUFF_LEN];


	// Check if command lind argument supplied is sufficient enough to proceed
	if(argc < 3) {

		fprintf(stderr, "usage %s hostname port", argv[0]);
		exit(0);
	}

	// Get my port number
	portNo = atoi(argv[2]);

	// Create a socket
	checkSocketConnectionSuccessful = socket(AF_INET, SOCK_STREAM, 0);

	if(checkSocketConnectionSuccessful < 0) {

		errorMessage("Error cannot create a socket");
	}

	// store local address or IP address and storee it in my server
	server = gethostbyname(argv[1]);

	if(server == NULL) {

		fprintf(stderr, "Error, No such host\n");
		exit(0);
	}




	// clear my resources used from my sockets always to be safe
	bzero((char*) &server_addr, sizeof(server_addr));

	// init my server
	server_addr.sin_family = AF_INET;

	// copying the value of my server address intp my server hostname
	bcopy((char *) server->h_addr, (char *) &server_addr.sin_addr.s_addr, server->h_length);

	// usual convert my port number to a network port number using htons
	server_addr.sin_port = htons(portNo);

	if(connect(checkSocketConnectionSuccessful, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {

		errorMessage("There was an error Connectiong\n");

	}

	printf("\n================================================================================\n");
	printf("Successfully connected to the server on port %d you can send your messages now:\n", portNo);
	printf("================================================================================\n\n");

	sendMessageToServer(checkSocketConnectionSuccessful, buffer);
	receiveMessageFromServer(checkSocketConnectionSuccessful, buffer);

	return 0;

}




int sendMessageToServer(int serverSock, char* buff) {

	int n;

	printf("Enter a message to send to the server: ");
	// clear my buffer
	bzero(buff, MAX_BUFF_LEN);
	fgets(buff, MAX_BUFF_LEN, stdin);

	n = write(serverSock, buff, strlen(buff));

	if(n < 0) {


		errorMessage("Error writing to the client socket");
		return -1;
	}

	return 0;
}


int receiveMessageFromServer(int serverSock, char* buff) {

	int n;

	// first clear my buffer off any shit
        bzero(buff, MAX_BUFF_LEN);


	// read from the server sock
	n = read(serverSock, buff, MAX_BUFF_LEN);

	if(n < 0) {

		errorMessage("Error reading from the socket");
		return -1;
	}

	printf("%s\n", buff);
	return 0;
}
