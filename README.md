# COMP 3203 A: Assignment 1
## Team
- Michael Lutaaya (Student #100843544)
- Guelor  Emauel  (Student #100884107)
- **UPDATE WITH NAMES AND STUDENT NUMBERS**

## Classes Description (Network programming Pattern)

Since TCP/IP network programs involve three basic pattens of usage or behaviors: actively connecting to servers, passively accepting connections from clients and transferring data between network peers – in other words clients and servers.  Each behavior suggests a distinct abstraction that can be implemented in a separate class.
 
- TCPConnector - Encapsulates the socket mechanisms to actively connect to a server. This is a factory class which produces TCPStream objects when client applications establish connections with servers.
- TCPAcceptor - Encapsulates the socket mechanisms to passively accept connections from a client. This is also a factory class which produces TCPStream objects when server applications establish connections with clients
- TCPStream - Provides network I/O mechanisms and returns IP address and TCP port of peer applications.


## Compilation Instructions
- To build the client and server in one step, type `make all` into the command line.
- The client and server can also be built individually by typing `make client` or `make server.`

## Launching Instructions
To begin using the application, the server must be launched before the client.

1. Type `./server -port <portnumber>` into the command line and specify the port number that server should use.
2. Type `./client -port <portnumber> -ip <ipaddress>` and specify the port number and IP address of the server the client should connect to.
