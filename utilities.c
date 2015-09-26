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

// Split input into tokens (e.g., "mkdir pictures" => ["mkdir", "pictures"])
void split(char *input, char *delimiter, char ***output, int *numTokens)
{
  char *request = strdup(input);
  char *token = strtok(request, delimiter);
  int numSpaces = 0, i = 0;

  while (token) {
    *output = realloc(*output, sizeof(char *) * ++numSpaces);
    if (*output == NULL) {
      printErrorMsg("realloc() failed");
    }
    (*output)[numSpaces - 1] = token;
    token = strtok(NULL, " ");
  }

  *output = realloc(*output, sizeof(char *) * (numSpaces + 1));
  (*output)[numSpaces] = 0;

  *numTokens = numSpaces + 1;

  free(request);
}
