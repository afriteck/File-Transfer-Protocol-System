CXX = gcc
CXX_FLAGS = -Wextra -Werror -pedantic-errors

all: ftpClient ftpServer

ftpServer: utilities.o ftpServer.o
	$(CXX) $(CXX_FLAGS) -o ./ftpServer utilities.o ftpServer.o

ftpServer.o: ftpServer.c ftpDefs.h

ftpClient: utilities.o ftpClient.o
	$(CXX) $(CXX_FLAGS) -o ./ftpClient utilities.o ftpClient.o

ftpClient.o: ftpClient.c ftpDefs.h

utilities.o: utilities.c ftpDefs.h
	$(CXX) $(CXX_FLAGS) -c utilities.c

clean:
	rm *.o ftpClient ftpServer
