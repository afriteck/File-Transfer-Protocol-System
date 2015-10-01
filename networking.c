#include <strings.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "ftpDefs.h"
#include <stdio.h>
#include <stdlib.h>

#define NETWORKING_DEBUG 0

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
  // if (numBytes > MAX_BUFF_LEN) {
    // printErrorMsg("Can't send message because its size exceeds MAX_BUFF_LEN\n");
  // }
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

void receiveFile(char *buff, int descriptor, char *filename) {
  // Determine size of the incoming file
  uint32_t fileSize = -1;
  int totalNumBytes = sizeof(uint32_t);
  int numBytesRcvd = 0;

  while (numBytesRcvd < totalNumBytes) {
    int result = recv(descriptor, &fileSize, totalNumBytes - numBytesRcvd, 0);
    if (result < 0) {
      printErrorMsg("recv() failed for determining file size");
    }

    numBytesRcvd += result;
  }
  printf("Incoming file with a size of %u bytes.\n", fileSize);

  FILE *file = fopen(filename, "ab");
  if (file == NULL) {
    printErrorMsg("fopen() failed in receiveFile function");
  }
  printf("Created new file `%s` successfully.\n", filename);

  totalNumBytes = fileSize;
  numBytesRcvd = 0;
  bzero(buff, MAX_BUFF_LEN);

  while (numBytesRcvd < totalNumBytes) {
    int recvResult = recv(descriptor, buff, MAX_BUFF_LEN, 0);
    if (recvResult < 0) {
      printErrorMsg("recv() failed");
    }

    int writeResult = fwrite(buff, sizeof(char), recvResult, file);
    if (writeResult < recvResult) {
      printErrorMsg("fwrite() failed to write the bytes to disk");
    }

    bzero(buff, MAX_BUFF_LEN);
    numBytesRcvd += writeResult;
    printf("%d out of %d bytes written to disk.\n", numBytesRcvd, totalNumBytes);
  }

  fclose(file);

  if (totalNumBytes == numBytesRcvd) {
    printf("Finished downloading %s!\n", filename);
  } else {
    printf("It seems like something went wrong while downloading %s.\n");
  }
}

void sendFile(int descriptor, char *filename) {
  char *buff = malloc(MAX_BUFF_LEN);

  FILE *file = fopen(filename, "rb+");
  if (file == NULL) {
    free(buff);
    printErrorMsg("fopen() failed in receiveFile function");
  }

  // Determine the number of bytes that will be sent (i.e., file size)
  fseek(file, 0L, SEEK_END);
  uint32_t fileSize = ftell(file);
  rewind(file);
  printf("Opened `%s` successfully (%u bytes).\n", filename, fileSize);

  int totalNumBytes = sizeof(uint32_t);
  int numBytesSent = 0;
  while (numBytesSent < totalNumBytes) {
    int result = send(descriptor, &fileSize, totalNumBytes - numBytesSent, 0);
    if (result < 0) {
      printErrorMsg("Failed to send the message size header");
    }
    numBytesSent += result;
  }

  totalNumBytes = fileSize;
  numBytesSent = 0;

  while (numBytesSent < totalNumBytes) {
    int numBytesRead = fread(buff, sizeof(char), MAX_BUFF_LEN, file);
    if (numBytesRead < 0) {
      printErrorMsg("fread() failed");
    }

    int result = send(descriptor, buff, numBytesRead, 0);
    if (numBytesSent < 0) {
      free(buff);
      printErrorMsg("send() failed");
    }

    bzero(buff, MAX_BUFF_LEN);
    numBytesSent += result;
    printf("%d out of %d bytes sent over the socket.\n", numBytesSent, totalNumBytes);
  }

  if (feof(file)) {
    printf("Reached EOF.\n");
  }

  if (ferror(file)) {
    free(buff);
    printErrorMsg("fread() failed");
  }

  fclose(file);
  free(buff);
  printf("Finished sending file.\n");
}
