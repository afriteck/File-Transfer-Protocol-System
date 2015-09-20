CXX = gcc
CXX_FLAGS = -Wextra -Werror -pedantic-errors

all: ftpClient ftpServer

ftpServer: utilities.o ftpServer.o networking.o
	$(CXX) $(CXX_FLAGS) -o ./ftpServer networking.o utilities.o ftpServer.o

ftpServer.o: ftpServer.c ftpDefs.h

ftpClient: utilities.o ftpClient.o
	$(CXX) $(CXX_FLAGS) -o ./ftpClient utilities.o ftpClient.o

ftpClient.o: ftpClient.c ftpDefs.h

networking.o: networking.c

utilities.o: utilities.c
	$(CXX) $(CXX_FLAGS) -c utilities.c

clean:
	rm *.o ftpClient ftpServer
