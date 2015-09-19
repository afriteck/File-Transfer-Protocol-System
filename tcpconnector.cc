#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "tcpconnector.h"

/*
 * Call takes a server host name or IP address string and the server
 *  listening port as arguments. The server struct sockaddr_in sin_family is set to PF_INEt and
 *  the sin_port is set to the TCP port on which the server is listening for connections.
 *
 *  @param const char, int
 *  @TCPStream: object
 */ 
TCPStream* TCPConnector::connect(const char* server, int port) {
  struct sockaddr_in address;

  memset(&address, 0, sizeof(address));
  address.sin_family = AF_INET;
  address.sin_port   = htons(port);

  if (resolveHostName(server, &(address.sin_addr)) != 0 ) {
    inet_pton(PF_INET, server, &(address.sin_addr)); // convert IPv4 and IPv6 addresses from text to binary form
  }
  int sd = socket (AF_INET, SOCK_STREAM, 0);
  if (::connect(sd, (struct sockaddr*)&address, sizeof(address)) != 0) {
	  return NULL;
  }
  return new TCPStream(sd, &address);
}

/*
 * Converts a DNS host name to an IP address in network byte order by calling
 * getaddinfo(). this function was chosen over gethostbyname() since it is thread safe whreas 
 * gethostbyname() is not. IF the host name is not a valid DNS name, i.e. It is an IP address 
 * string or something else, -1 is returned, otherwise 0 is returned.
 */
int TCPConnector::resolveHostName(const char* hostname, struct in_addr* addr) {
 struct addrinfo *res;

 int result = getaddrinfo (hostname, NULL, NULL, &res);

 if ( result == 0) {
   memcpy(addr, &((struct sockaddr_in *) res->ai_addr)->sin_addr, sizeof(struct in_addr));
   freeaddrinfo(res);
 }
 return result;

}
