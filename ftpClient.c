#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include "ftpDefs.h"

int main(int argc, char *argv[]) {
  // Do not continue unless all arguments have been provided
  if (argc < 3) {
    fprintf(stderr, "usage: %s <ipaddress> <port>\n", argv[0]);
    exit(0);
  }

  char *ip_address = argv[1];
  int port_number = atoi(argv[2]), descriptor = -1;
  connectToServer(ip_address, port_number, &descriptor);
  printf("Initiated connection to %s at port %d.\n", ip_address, port_number);

  char buffer[MAX_BUFF_LEN];
  while (1) {
    bzero(buffer, MAX_BUFF_LEN);
    sendMessageToServer(descriptor, buffer);
    receiveMessageFromServer(descriptor, buffer);
  }

  // TODO: close socket
  return 0;
}

int sendMessageToServer(int serverSock, char* buff) {
  printf("> ");
  fgets(buff, MAX_BUFF_LEN, stdin);
  writeToSocket(&serverSock, &buff);
  return 0;
}

int receiveMessageFromServer(int serverSock, char* buff) {
  readFromSocket(&serverSock, &buff);
  printf("%s\n", buff);
  return 0;
}
