#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "ftpDefs.h"

void executeCommand(char *command, char **output) {
  FILE *file = popen(command, "r");
  if (file == NULL) {
    printErrorMsg("popen() failed\n");
  }

  int bytesRead = 0;
  char nextChar;
  while (bytesRead < MAX_BUFF_LEN - 1 && (nextChar = fgetc(file)) != EOF) {
    if (output != NULL) {
      (*output)[bytesRead] = nextChar;
    }
    ++bytesRead;
  }

  pclose(file);
}

int makeDirectory(char *name, char **output) {
  int result = mkdir(name, 0777);
  if (result == 0) {
    sprintf(*output, "Directory `%s` created.\n", name);
  } else {
    sprintf(*output, "Failed to created directory `%s`.\n", name);
  }
  return result;
}

void processRequest(char *request, char **reply) {
  bzero(*reply, MAX_BUFF_LEN);

  char **tokens = NULL;
  int numTokens = -1;
  split(request, " ", &tokens, &numTokens);

  if (numTokens >= 1 && strcmp(tokens[0], "ls") == 0) {
    executeCommand("ls", reply);
  } else if (numTokens >= 2 && strcmp(tokens[0], "mkdir") == 0) {
    makeDirectory(tokens[1], reply);
  } else {
    sprintf(*reply, "%s: command not found\n", request);
  }

  free(tokens);
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
  acceptIncomingConnection(&listen_socket, &accept_socket);

  while (1) {
    char buffer[MAX_BUFF_LEN];
    receiveMessage(buffer, accept_socket);

    char *reply = malloc(MAX_BUFF_LEN);
    processRequest(buffer, &reply);
    sendMessage(reply, accept_socket);
    free(reply);
  }

  close(accept_socket);
  close(listen_socket);

  return 0;
}

