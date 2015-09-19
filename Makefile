CXX = gcc
CXX_FLAGS = -Wextra -Werror -pedantic-errors

all: ftpClient ftpServer

ftpServer: ftpServer.o
	$(CXX) $(CXX_FLAGS) -o ./ftpServer ftpServer.o

ftpServer.o: ftpServer.c

ftpClient: ftpClient.o
	$(CXX) $(CXX_FLAGS) -o ./ftpClient ftpClient.o

ftpClient.o: ftpClient.c

clean:
	rm *.o ftpClient ftpServer
