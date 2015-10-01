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
  // determine the size of a uint

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
  printf("Incoming message of size %u", fileSize);


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
      printf("the write result was %d an the recvResult was %d", writeResult, recvResult);
      printErrorMsg("fwrite() failed to write the bytes to disk");
    }
    printf("Successfully wrote %d bytes to disk.\n", writeResult);

    bzero(buff, MAX_BUFF_LEN);
    numBytesRcvd += writeResult;
  }

  fclose(file);

  if (totalNumBytes == numBytesRcvd) {
    printf("Ya did good kid\n");
  } else {
    printf("ya got work to do\n");
  }

  // retrieve that from the socket

  // read from the socket and write to the file until you've seen the exact number of bytes


  // FILE *file = fopen(filename, "a");
  // if (file == NULL) {
  //   printErrorMsg("fopen() failed in receiveFile function");
  // }
  // printf("Created new file `%s` successfully.\n", filename);

  // bzero(buff, MAX_BUFF_LEN);

  // int numBytesRcvd = -1;
  // while ((numBytesRcvd = recv(descriptor, buff, MAX_BUFF_LEN, 0)) > 0) {
  //   printf("Successfully read %d bytes through the socket.\n", numBytesRcvd);
  //   int numBytesWritten = fwrite(buff, sizeof(char), numBytesRcvd, file);
  //   if (numBytesWritten < numBytesRcvd) {
  //     printErrorMsg("fwrite() failed to write the bytes to disk");
  //   }
  //   printf("Successfully wrote %d bytes to disk.\n", numBytesWritten);

  //   bzero(buff, MAX_BUFF_LEN);
  //   if (numBytesRcvd == 0 || numBytesRcvd != MAX_BUFF_LEN) {
  //     printf("We are bailing since we received %d bytes", numBytesRcvd);
  //     break;
  //   }
  // }

  // fclose(file);

  // if (numBytesRcvd < 0) {
  //   printErrorMsg("recv() failed in receiveFile function");
  // } else if (numBytesRcvd == 0) {
  //   printf("The peer closed its half of the connection.\n");
  //   // TODO: probably a return code so that we can break...
  // } else {
  //   printf("Finished downloading `%s`.\n", filename);
  // }
}

void sendFile(int descriptor, char *filename) {
  char *buff = malloc(MAX_BUFF_LEN);

  FILE *file = fopen(filename, "rb+");
  if (file == NULL) {
    free(buff);
    printErrorMsg("fopen() failed in receiveFile function");
  }
  printf("Opened `%s` successfully.\n", filename);

  // Determine the number of bytes that will be sent (i.e., file size)
  fseek(file, 0L, SEEK_END);
  uint32_t fileSize = ftell(file);
  rewind(file);
  printf("Preparing to send %u bytes", fileSize);

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
    // try to read max_buff_len bytes from the file.
    // try to send that many bytes trhough the socket
    int numBytesRead = fread(buff, sizeof(char), MAX_BUFF_LEN, file);
    if (numBytesRead < 0) {
      printErrorMsg("fread() failed");
    }
    printf("Successfully read %d bytes from the file.\n", numBytesRead);

    int result = send(descriptor, buff, numBytesRead, 0);
    if (numBytesSent < 0) {
      free(buff);
      printErrorMsg("send() failed");
    }
    printf("Successfully transmitted %d bytes over the socket.\n", numBytesSent);

    bzero(buff, MAX_BUFF_LEN);
    numBytesSent += result;
  }

  if (feof(file)) {
    printf("Reached EOF.\n");
  }

  if (ferror(file)) {
    free(buff);
    printErrorMsg("fread() failed");
  }
  // send that number over the socket and verify that the number of bytes were sent
  // keep looping until we have read and sent that exact amount of bytes



  // int numBytesRead = -1;
  // while ((numBytesRead = fread(buff, sizeof(char), MAX_BUFF_LEN, file)) > 0) {
  //   printf("Successfully read %d bytes from the file.\n", numBytesRead);
  //   int numBytesSent = send(descriptor, buff, numBytesRead, 0);
  //   if (numBytesSent < 0) {
  //     free(buff);
  //     printErrorMsg("send() failed");
  //   }
  //   printf("Successfully transmitted %d bytes over the socket.\n", numBytesSent);
  //   bzero(buff, MAX_BUFF_LEN);
  // }

  // if (feof(file)) {
  //   printf("Reached EOF.\n");
  // }

  // if (ferror(file)) {
  //   free(buff);
  //   printErrorMsg("fread() failed");
  // }

  fclose(file);
  free(buff);
  printf("Finished sending file.\n");
}
