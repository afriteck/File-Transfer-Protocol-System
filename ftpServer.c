#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include "ftpDefs.h"

// TODO: verify that it works in the Linux VM

void ls(char **output) {
  DIR* directory = opendir("./");
  if (directory == NULL) {
    printErrorMsg("opendir() failed");
  }

  struct dirent *entry = NULL;
  while ((entry = readdir(directory)) != NULL) {
    strcat(*output, entry->d_name);
    strcat(*output, "\n");
  }

  closedir(directory);
}

void changeDirectory(char* name, char** output) {
  int result = chdir(name);

  if ( result == 0 ) {
    sprintf(*output, "Changed to directory: %s.\n", name);
  }
  else {
    sprintf(*output, "Failed to change directory: %s.\n", name);
  }
}

void makeDirectory(char *name, char **output) {
  int result = mkdir(name, 0777);
  if (result == 0) {
    sprintf(*output, "Directory `%s` created.\n", name);
  } else {
    sprintf(*output, "Failed to created directory `%s`.\n", name);
  }
}

void processRequest(char *request, int descriptor) {
  char *reply = malloc(MAX_BUFF_LEN);
  bzero(reply, MAX_BUFF_LEN);

  char* trimBuff = trimStringAfter(request);
  char* mergeStringForMakeDirectory = concat("mkdir ", trimBuff);
  char* mergeStringForChangeDirectory = concat("cd ", trimBuff);
  char* mergeStringForGet           = concat("get ", trimBuff);
  char* mergeStringForPut           = concat("put ", trimBuff);

  if ((strcmp(request, "ls")) == 0) {
    ls(&reply);
    sendMessage(reply, descriptor);
  }
  else if ((strcmp(request, mergeStringForChangeDirectory)) == 0) {
    changeDirectory(trimBuff, &reply);
    sendMessage(reply, descriptor);
  }
  else if ((strcmp(request, mergeStringForMakeDirectory)) == 0) {
    makeDirectory(trimBuff, &reply);
    sendMessage(reply, descriptor);
  }
  else if ((strcmp(request, mergeStringForGet)) == 0) {
    sendFile(descriptor, trimBuff);
  }
  else if ((strcmp(request, mergeStringForPut)) == 0 ) {
    receiveFile(request, descriptor, trimBuff);
  }
  else {
    sprintf(reply, "%s: command not found\n", request);
    sendMessage(reply, descriptor);
  }

  free(mergeStringForMakeDirectory);
  free(mergeStringForChangeDirectory);
  free(mergeStringForGet);
  free(reply);
}

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

  while (1) {
    acceptIncomingConnection(&listen_socket, &accept_socket);

    while (1) {
      char buffer[MAX_BUFF_LEN];
      if (receiveMessage(buffer, accept_socket) < 0) {
        printf("Breaking out of the while loop.\n");
        break;
      }

      printf("---------- REQUEST: `%s` ----------\n", buffer);
      processRequest(buffer, accept_socket);
      printf("----------  END REQUEST  ----------\n\n");
    }

    close(accept_socket);
  }
  close(listen_socket);

  return 0;
}
