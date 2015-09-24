#include <strings.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "ftpDefs.h"

void createServerAddrStruct(struct sockaddr_in *address, int port) {
  	bzero(address, sizeof(*address));
  	address->sin_port = htons(port); // Store bytes in network byte order
  	address->sin_family = AF_INET;
  	address->sin_addr.s_addr = INADDR_ANY;
}

void createClientAddrStruct(struct sockaddr_in *address, char *ip_address, int port) {
  	bzero(address, sizeof(*address));
  	address->sin_port = htons(port);
  	address->sin_family = AF_INET;

  	if (inet_pton(address->sin_family, ip_address, &(address->sin_addr.s_addr)) != 1) {
    		printErrorMsg("Can't parse IP address or system error occurred\n");
  	}
}

void createSocket(int *descriptor) {
  	*descriptor = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

  	if (*descriptor < 0) {
    		printErrorMsg("Can't create socket\n");
  	}
}

void bindSocket(struct sockaddr_in *address, int *descriptor) {
  	socklen_t address_len = (socklen_t) sizeof(*address);

  	if (bind(*descriptor, (struct sockaddr *)address, address_len) < 0) {
    		printErrorMsg("Can't bind socket\n");
  	}
}

void setupListenSocket(int port, int *listen_socket) {
  	struct sockaddr_in address;

  	createServerAddrStruct(&address, port);
  	createSocket(listen_socket);
  	bindSocket(&address, listen_socket);
  	listen(*listen_socket, MAX_PENDING_CONNECTIONS);
}

void acceptIncomingConnection(int *listen_socket, int *accept_socket) {
  	struct sockaddr address;

  	socklen_t address_len = (socklen_t) sizeof(address);

  	if ((*accept_socket = accept(*listen_socket, &address, &address_len)) < 0) {
   		printErrorMsg("Error accepting connection\n");
  	}
}

void connectToServer(char *ip_address, int port, int *descriptor) {
  	struct sockaddr_in address;

  	createClientAddrStruct(&address, ip_address, port);
  	createSocket(descriptor);

  	if (connect(*descriptor, (struct sockaddr *) &address, sizeof(address)) < 0) {
    		printErrorMsg("Can't initiate connection on socket\n");
  	}
}

