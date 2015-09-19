/*
 * TCPAcceptor includes member variables for the listening socket descriptor,
 * the socket address iformation-IP address and TCP port - and a flag that
 * indicates whether or not the TCPAcceptor has started listening for connections.
 * Two public methods are supported. One to start the listening and the other to accept connections.
 */

#include <string>
#include <netinet/in.h>
#include "tcpstream.h"

using namespace std;

class TCPAcceptor {
  int     m_lsd;
  string  m_address;
  int     m_port;
  bool    m_listening;

  public:
    TCPAcceptor(int port, const char* address="");
    ~TCPAcceptor();
    int         start();
    TCPStream*  accept();

  private:
    TCPAcceptor(){}
};

