#define MAX_BUFF_LEN 1024
#define MAX_PENDING_CONNECTIONS 10

// Client
int sendMessageToServer(int serverSock, char* buff);
int receiveMessageFromServer(int serverSock, char* buff);

// Server
int createSocket();
int readMessage(int, char*);
int writeMessage(int, char*);
int readMessageFromClient(int clientSock, char (*buff)[MAX_BUFF_LEN]);
int writeMessageToClient(int clientSock, char (*buff)[MAX_BUFF_LEN]);
