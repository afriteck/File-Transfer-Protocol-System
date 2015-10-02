#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *mystrdup(const char *s);

/* This handle error printing to the
 * standard output */
void printErrorMsg(char *msg) {
  perror(msg);
  exit(1);
}

/* This determines where a strings has
 * a newLine character and replaces it
 * with a null terminating character */
void trimString(char *string) {
  size_t pos = strlen(string) - 1;
  if (string[pos] == '\n') {
    string[pos] = '\0';
  }
}

/* This trims a string after it ecounters
 * a space and returns the string after
 * the space. */
char* trimStringAfter(char* string) {

  char* starting = string;

  while (*starting != ' ') {
    starting++;
  }

  // first one after
  starting++;
  return starting;
}

/* Concatenate two strings together which is 
 * take one string and append another string 
 * to it */
char* concat(char *s1, char *s2) {

  size_t len1 = strlen(s1);
  size_t len2 = strlen(s2);

  // +1 for the zero-terminator
  char *result = malloc(len1 + len2 + 1);

  // Checks if malloc was successful
  if (result == NULL) { 
    printErrorMsg("malloc() failed in concat");
  }

  memcpy(result, s1, len1);

  //+1 to copy the null-terminator
  memcpy(result + len1, s2, len2 + 1);
  return result;
}

/* Compare two if the buff string starts with string a return 
 * success else return a failure */
int startsWith(char *a, char *b) {
  if (strncmp(a, b, strlen(b)) == 0) {
    return 1;
  }
  return -1;
}
