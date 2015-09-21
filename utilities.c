#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printErrorMsg(char *msg) {
  perror(msg);
  exit(1);
}

void trimString(char *string) {
  size_t pos = strlen(string) - 1;
  if (string[pos] == '\n') {
    string[pos] = '\0';
  }
}
