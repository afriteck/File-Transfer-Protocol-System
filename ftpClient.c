#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <sys/socket.h>
#include "ftpDefs.h"

void getUserInput(char *buff) {
  printf("> ");
  fflush(stdout);
  bzero(buff, MAX_BUFF_LEN);
  fgets(buff, MAX_BUFF_LEN, stdin);
  trimString(buff); // removes newline and adds null-terminator
}

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

  while (1) {
    char buff[MAX_BUFF_LEN];
    getUserInput(buff);
    sendMessage(buff, descriptor);

    if (startsWith(buff, "get") > 0) {
      receiveFile(buff, descriptor, "download.jpg");
    }
    else if (startsWith(buff, "put") > 0) {
      printf("put client\n");
      sendFile(descriptor,trimStringAfter(buff));
    }
    else if ( startsWith(buff, "quit") > 0) {
    printf("Good bye!\n");
    close(descriptor);
    exit(1);
    }
    else {
      receiveMessage(buff, descriptor);
      printf("%s", buff);
    }
  }

  printf("\n");
  close(descriptor);

  return 0;
}
