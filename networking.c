#include <strings.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "ftpDefs.h"

#define NETWORKING_DEBUG 0
#if NETWORKING_DEBUG
#include <stdio.h>
#endif

void createServerAddrStruct(struct sockaddr_in *address, int port) {
  bzero(address, sizeof(*address));
  address->sin_port = htons(port); // Store bytes in network byte order
  address->sin_family = AF_INET;
  address->sin_addr.s_addr = INADDR_ANY;
}

void createClientAddrStruct(struct sockaddr_in *address, char *ip_address, int port) {
  bzero(address, sizeof(*address));
  address->sin_port = htons(port);
  address->sin_family = AF_INET;
  if (inet_pton(address->sin_family, ip_address, &(address->sin_addr.s_addr)) != 1) {
    printErrorMsg("Can't parse IP address or system error occurred\n");
  }
}

void createSocket(int *descriptor) {
  *descriptor = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (*descriptor < 0) {
    printErrorMsg("Can't create socket\n");
  }
}

void bindSocket(struct sockaddr_in *address, int *descriptor) {
  socklen_t address_len = (socklen_t) sizeof(*address);
  if (bind(*descriptor, (struct sockaddr *)address, address_len) < 0) {
    printErrorMsg("Can't bind socket\n");
  }
}

void setupListenSocket(int port, int *listen_socket) {
  struct sockaddr_in address;
  createServerAddrStruct(&address, port);
  createSocket(listen_socket);
  bindSocket(&address, listen_socket);
  listen(*listen_socket, MAX_PENDING_CONNECTIONS);
}

void acceptIncomingConnection(int *listen_socket, int *accept_socket) {
  struct sockaddr address;
  socklen_t address_len = (socklen_t) sizeof(address);
  if ((*accept_socket = accept(*listen_socket, &address, &address_len)) < 0) {
    printErrorMsg("Error accepting connection\n");
  }
}

void connectToServer(char *ip_address, int port, int *descriptor) {
  struct sockaddr_in address;
  createClientAddrStruct(&address, ip_address, port);
  createSocket(descriptor);
  if (connect(*descriptor, (struct sockaddr *) &address, sizeof(address)) < 0) {
    printErrorMsg("Can't initiate connection on socket\n");
  }
}

int sendMessage(char *buff, int descriptor) {
  if (strlen(buff) < 1) {
    return strlen(buff);
  }

  int numBytes = strlen(buff) + 1; // strlen() + null terminator
  if (numBytes > MAX_BUFF_LEN) {
    printErrorMsg("Can't send message because its size exceeds MAX_BUFF_LEN\n");
  }
  buff[strlen(buff)] = '\0';

  #if NETWORKING_DEBUG
  printf("Sending a %d-byte message: %s\n", numBytes, buff);
  #endif

  int totalBytesSent = 0;
  while (totalBytesSent < numBytes) {
    int result = write(descriptor, buff, numBytes);
    if (result < 0) {
      printErrorMsg("write() failed\n");
    }
    totalBytesSent += result;
  }
  return totalBytesSent;
}

int receiveMessage(char *buff, int descriptor) {
  bzero(buff, MAX_BUFF_LEN);
  int numBytesRcvd = 0;
  while (1) {
    int result = recv(descriptor, buff, MAX_BUFF_LEN - numBytesRcvd, 0);
    if (result < 0) {
      printErrorMsg("recv() failed\n");
    }
    numBytesRcvd += result;
    if (strchr(buff, '\0') != NULL) { // The null terminator has been read
      break;
    }
  }

  #if NETWORKING_DEBUG
  printf("Received a %d-bytes message: %s\n", numBytesRcvd, buff);
  #endif

  return numBytesRcvd;
}

