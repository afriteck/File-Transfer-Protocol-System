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
    		string[pos] = '\0' ;
  	}
}

char* trimStringAfter(char* string) {

	char* starting = string;

	while (*starting != ' ') {
    	starting++;
   	}

   	// first one _after_
   	starting++;
   	return starting;
}


char* concat(char *s1, char *s2) {

    	size_t len1 = strlen(s1);
    	size_t len2 = strlen(s2);

	// +1 for the zero-terminator
    	char *result = malloc(len1 + len2 + 1);

    	//check for errors in malloc here later...
    	memcpy(result, s1, len1);
    	memcpy(result + len1, s2, len2 + 1);//+1 to copy the null-terminator
    	return result;
}
