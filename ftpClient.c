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
    receiveMessageFromServer(descriptor, buffer,  MAX_BUFF_LEN - 1);
  }

  // TODO: close socket
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

    printErrorMsg("Error writing to the client socket");
    return -1;
  }

  return 0;
}



int receiveMessageFromServer(int serverSock, char* buff, size_t len) {

  int n;

  // first clear my buffer off any shit
  bzero(buff, MAX_BUFF_LEN);

  // read from the server sock
  n = read(serverSock, buff, len);

  if(n < 0) {

    printErrorMsg("Error reading from the socket");
    return -1;
  }

  printf("%s\n", buff);
  return 0;
}
