#define MAX_BUFF_LEN 2048
#define MAX_PENDING_CONNECTIONS 10

// Networking - Client
int sendMessageToServer(int serverSock, char *buff);
int receiveMessageFromServer(int serverSock, char*buff, size_t len);

// Networking - Server
int readMessageFromClient(int clientSock, char *buff);
int writeMessageToClient(int clientSock, char *buff);

// Networking - Sockets
void setupListenSocket(int port, int *listen_socket);
void acceptIncomingConnection(int *listen_socket, int *accept_socket);
void connectToServer(char *ip_address, int port, int *descriptor);

// Utilities
void printErrorMsg(char *msg);

// Request Handler
void handleAllRequests(int newSocketForClient, char* buffer);
void trimString(char *string);
void addNewLineCharacterToString(char* string);
char* trimStringAfter(char* string);
char* concat(char *s1, char *s2);
void handleReqForMkir(int sock, char* buff);
