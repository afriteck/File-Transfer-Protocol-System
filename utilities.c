#include <stdio.h>
#include <stdlib.h>

void printErrorMsg(char *msg) {
  perror(msg);
  exit(1);
}
