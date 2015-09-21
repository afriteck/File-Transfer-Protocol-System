#include <stdio.h>
#include <stdlib.h>
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
  printf("Accepting connections on port %d.\n",
    port_number);
  fflush(stdout);

  int accept_socket = -1;
  acceptIncomingConnection(&listen_socket, &accept_socket);

  char buffer[MAX_BUFF_LEN];
  while (1) {
    bzero(buffer, MAX_BUFF_LEN);
    readMessageFromClient(accept_socket, &buffer);
    trimString(buffer);
    handleRequest(buffer);
    writeMessageToClient(accept_socket, &buffer);
  }

  // TODO: close sockets
  return 0;
}

int readMessageFromClient(int clientSock, char (*buff)[MAX_BUFF_LEN]) {
  readFromSocket(&clientSock, (char **)&buff);
  printf("Message received: %s\n", (char *)buff);
  return 0;
}

int writeMessageToClient(int clientSock, char (*buff)[MAX_BUFF_LEN]) {
  writeToSocket(&clientSock, (char **)&buff);
  return 0;
}
