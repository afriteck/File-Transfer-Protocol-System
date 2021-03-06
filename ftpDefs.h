#define MAX_BUFF_LEN 1024
#define MAX_PENDING_CONNECTIONS 10

#include <stdio.h>

/* Networking  prototypes */
int   sendMessage(char *buff, int descriptor);
int   receiveMessage(char *buff, int descriptor);
void  setupListenSocket(int port, int *listen_socket);
void  acceptIncomingConnection(int *listen_socket, int *accept_socket);
void  connectToServer(char *ip_address, int port, int *descriptor);
void  receiveFile(char *buff, int descriptor, char *filename);
void  sendFile(int descriptor, char *filename);

/* Utilities prototypes */
void  printErrorMsg(char *msg);
char* trimStringAfter(char *string);
char* concat(char *s1, char *s2);
int   startsWith(char *a, char *b);

/* Request Handler prototypes */
void  handleAllRequests(int newSocketForClient, char* buffer);
void  trimString(char *string);
void  addNewLineCharacterToString(char* string);
