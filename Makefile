CXX = gcc
CXX_FLAGS = -Wextra -Werror -pedantic-errors

all: ftpClient ftpServer

ftpServer: utilities.o ftpServer.o networking.o request_handler.o
	$(CXX) $(CXX_FLAGS) -o ./ftpServer networking.o utilities.o request_handler.o ftpServer.o

ftpServer.o: ftpServer.c ftpDefs.h

ftpClient: networking.o utilities.o ftpClient.o
	$(CXX) $(CXX_FLAGS) -o ./ftpClient networking.o utilities.o ftpClient.o

ftpClient.o: ftpClient.c ftpDefs.h

networking.o: networking.c ftpDefs.h

request_handler.o: request_handler.c ftpDefs.h

utilities.o: utilities.c

clean:
	rm *.o ftpClient ftpServer
