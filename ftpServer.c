#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include "ftpDefs.h"

int main(int argc, char *argv[]) {
	// Do not continue unless all arguments have been provided
  if (argc < 2) {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(1);
  }

	int port_number = atoi(argv[1]);
  struct sockaddr_in server_addr;
  bzero(&server_addr, sizeof(server_addr));

  // `htons` ensures that bytes are stored in memory in network byte order
	server_addr.sin_port = htons(port_number);
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;

  int listenSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (listenSocket < 0) {
    printErrorMsg("Can't create socket\n");
  }

  // Bind the socket to an IP address and port number
  socklen_t server_addr_len = (socklen_t) sizeof(server_addr);
  if (bind(listenSocket, (struct sockaddr *) &server_addr, server_addr_len) < 0) {
    printErrorMsg("Can't bind socket\n");
  }

  listen(listenSocket, MAX_PENDING_CONNECTIONS);

  char buffer[MAX_BUFF_LEN];
  while (1) {
		struct sockaddr_in client_addr;
	  socklen_t client_addr_len = sizeof(client_addr);
    int acceptSocket = accept(listenSocket, (struct sockaddr *) &client_addr, &client_addr_len);
    if (acceptSocket < 0) {
      printErrorMsg("Error accepting connection\n");
    }

    bzero(buffer, MAX_BUFF_LEN);
    readMessageFromClient(acceptSocket, &buffer);
    writeMessageToClient(acceptSocket, &buffer);
  }

  return 0;
}

int readMessageFromClient(int clientSock, char (*buff)[MAX_BUFF_LEN]) {

  int n;

  n = read(clientSock, buff, MAX_BUFF_LEN);

  if(n < 0) {
    printErrorMsg("Error reading from the client socket");
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
  n = write(clientSock, message, strlen(message));

  if(n < 0) {
    printErrorMsg("Error writing to the client socket");
    return -1;
  }

  return 0;
}
