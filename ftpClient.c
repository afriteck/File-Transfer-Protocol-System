#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "ftpDefs.h"

int main(int argc, char *argv[]) {
  // Do not continue unless all arguments have been provided
  if (argc < 3) {
    fprintf(stderr, "usage: %s <ipaddress> <port>\n", argv[0]);
    exit(0);
  }

	char *ip_address = argv[1];
  int port_number = atoi(argv[2]);
  struct sockaddr_in address;
  bzero(&address, sizeof(address));

  // `htons` ensures that bytes are stored in memory in network byte order
	address.sin_port = htons(port_number);
	address.sin_family = AF_INET;
  if (inet_pton(address.sin_family, ip_address, &address.sin_addr.s_addr) != 1) {
  	printErrorMsg("Can't parse IP address or system error occurred\n");
  }

  // Create a socket using TCP
 	int descriptor;
  if ((descriptor = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    printErrorMsg("Can't create socket\n");
  }

  // Initiate a connection on the socket
  if (connect(descriptor, (struct sockaddr *) &address, sizeof(address)) < 0) {
    printErrorMsg("Can't initiate connection on socket\n");
  }

  printf("Initiated connection to %s at port %d.\n\n", ip_address, port_number);

	char buffer[MAX_BUFF_LEN];
  sendMessageToServer(descriptor, buffer);
  receiveMessageFromServer(descriptor, buffer);

  return 0;
}

int sendMessageToServer(int serverSock, char* buff) {
  int n;

  printf("Enter a message to send to the server: ");
  // clear my buffer
  bzero(buff, MAX_BUFF_LEN);
  fgets(buff, MAX_BUFF_LEN, stdin);

  n = write(serverSock, buff, strlen(buff));

  if (n < 0) {
    printErrorMsg("Error writing to the client socket");
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

  if (n < 0) {
    printErrorMsg("Error reading from the socket");
    return -1;
  }

  printf("%s\n", buff);
  return 0;
}
