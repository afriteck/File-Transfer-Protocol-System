#define MAX_BUFF_LEN 1024
#define MAX_PENDING_CONNECTIONS 10

// Networking - Client
int sendMessageToServer(int serverSock, char *buff);
int receiveMessageFromServer(int serverSock, char*buff);

// Networking - Server
int readMessageFromClient(int clientSock, char (*buff)[MAX_BUFF_LEN]);
int writeMessageToClient(int clientSock, char (*buff)[MAX_BUFF_LEN]);

// Networking - Sockets
void setupListenSocket(int port, int *listen_socket);
void acceptIncomingConnection(int *listen_socket, int *accept_socket);
void connectToServer(char *ip_address, int port, int *descriptor);
void readFromSocket(int *descriptor, char **buffer);
void writeToSocket(int *descriptor, char **buffer);

// Utilities
void printErrorMsg(char *msg);

// Request Handler
void handleRequest(char *request);
void trimString(char *string);
