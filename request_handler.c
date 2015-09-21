#include <stdio.h>
#include <string.h>
#include "ftpDefs.h"

void ls(char **output) {
  FILE* stream = popen("ls", "r");
  if (stream == NULL) {
    printErrorMsg("Can't run `ls` command");
  }

  // TODO: figure this out...
  fgets(*output, MAX_BUFF_LEN - 1, stream);
  pclose(stream);
}

void handleRequest(char *request) {
  if (strcmp("ls", request) == 0) {
    ls(&request);
  }
}

