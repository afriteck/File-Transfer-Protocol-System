/*
 * TCPConnector provides the connect() method to actively establish a connection with a server. 
 * It accepts the server port and a string containing the server host name or IP address.
 *  If successful, a pointer to a TCPStream object is returned to the caller
 */

#include <netinet/in.h>
#include "tcpstream.h"

class TCPConnector {
  public:
    TCPStream* connect(const char* server, int port);
  private:
    int resolveHostName(const char* host, struct in_addr* addr);
};
