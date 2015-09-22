#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "ftpDefs.h"
#include "dirent.h"

int main(int argc, char *argv[]) {
  // Do not continue unless all arguments have been provided
  if (argc < 2) {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(1);
  }

  int port_number = atoi(argv[1]), listen_socket = -1;
  setupListenSocket(port_number, &listen_socket);
  printf("Accepting connections on port %d.\n", port_number);
  fflush(stdout);

  int accept_socket = -1;
  acceptIncomingConnection(&listen_socket, &accept_socket);

  char buffer[MAX_BUFF_LEN];
  while (1) {
    bzero(buffer, MAX_BUFF_LEN);
    readMessageFromClient(accept_socket, &buffer);
    trimString(buffer);
    handleAllRequests(accept_socket ,buffer);
  }

  // TODO: close sockets
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
    printf("Message received From the client: %s\n", *buff);
    return 0;
  }
}

int writeMessageToClient(int clientSock, char (*buff)[MAX_BUFF_LEN]) {

  int n;

  n = write(clientSock, buff, strlen(*buff));

  if(n < 0) {
    printErrorMsg("Error writing to the client socket");
    return -1;
  }

  return 0;

}

void handleAllRequests(int newSocketForClient, char* buffer) {
  if((strcmp(buffer, "ls")) == 0) {

    char newline[MAX_BUFF_LEN] = "\n";

    DIR* d = opendir("./");
    if (d == NULL) exit(1);

    for(struct dirent *de = NULL; (de = readdir(d)) != NULL; ) {

      writeMessageToClient(newSocketForClient, &de->d_name);
      writeMessageToClient(newSocketForClient, &newline);
    }

    closedir(d);

  }

  else if ((strcmp(buffer, "mkdir")) == 0) {

  }

}
