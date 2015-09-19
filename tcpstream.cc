#include <arpa/inet.h>
#include "tcpstream.h"

/*
 * Constructor stores teh connected socket descriptor then convert the socket 
 * information structure fields to a peer IP address string and peer TCP port
 *
 */ 
TCPStream::TCPStream(int sd, struct sockaddr_in* address) : m_sd(sd) {
	char ip[50];
	inet_ntop(PF_INET, (struct in_addr*)&(address->sin_addr.s_addr), ip, sizeof(ip)-1);
	m_peerIP = ip;
	m_peerPort = ntohs(address->sin_port);
}

/*
 * Destructor for simply closing the connection
 */ 
TCPStream::~TCPStream() {
  close(m_sd);
}

/*
 * This method Simply wrap calls to write(), returns the number of bytes sent
 * @param  buffer, len
 * @return ssize_t: size of blocks that can be writen
 */
ssize_t TCPStream::send(const char* buffer, size_t len){
  return write(m_sd, buffer, len);
}

/*
 * This method Simply wrap calls to read(), returns the number of bytes received
 * @param  buffer, len
 * @return ssize_t
 */
ssize_t TCPStream::receive(char* buffer, size_t len) {
  return read(m_sd, buffer, len);
}

/*
 * These  methods return the IP and TCP port information of the peer to which
 * the network application, client or server, are connected.
 */
string TCPStream::getPeerIP(){return m_peerIP;}
int TCPStream::getPeerPort(){return m_peerPort;}

