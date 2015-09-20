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

  int port_number = atoi(argv[1]), listen_socket = -1;
  setupListenSocket(port_number, &listen_socket);
  printf("The server is ready to accept connections on port %d.\n",
    port_number);
  fflush(stdout);

  char buffer[MAX_BUFF_LEN];
  while (1) {
    bzero(buffer, MAX_BUFF_LEN);

    int accept_socket = -1;
    acceptIncomingConnection(&listen_socket, &accept_socket);
    readMessageFromClient(accept_socket, &buffer);
    writeMessageToClient(accept_socket, &buffer);
  }

  // TODO: close sockets
  return 0;
}

int readMessageFromClient(int clientSock, char (*buff)[MAX_BUFF_LEN]) {
  if (read(clientSock, buff, MAX_BUFF_LEN) < 0) {
    printErrorMsg("Error reading from the client socket");
    return -1;
  } else {
    printf("Message received From the client: %s\n", (char *)buff);
    return 0;
  }
}

int writeMessageToClient(int clientSock, char (*buff)[MAX_BUFF_LEN]) {
  char *message = "I got your message";
  if (write(clientSock, message, strlen(message)) < 0) {
    printErrorMsg("Error writing to the client socket");
    return -1;
  }
  return 0;
}
