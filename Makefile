CXX = gcc
CXX_FLAGS = -Wextra -Werror -pedantic-errors -std=c99

all: ftpClient ftpServer

ftpServer: utilities.o ftpServer.o networking.o
	$(CXX) $(CXX_FLAGS) -o ./ftpServer networking.o utilities.o ftpServer.o

ftpClient: networking.o utilities.o ftpClient.o
	$(CXX) $(CXX_FLAGS) -o ./ftpClient networking.o utilities.o ftpClient.o

ftpServer.o: ftpServer.c ftpDefs.h
	$(CXX) $(CXX_FLAGS) -c -o ftpServer.o ftpServer.c

ftpClient.o: ftpClient.c ftpDefs.h
	$(CXX) $(CXX_FLAGS) -c -o ftpClient.o ftpClient.c

networking.o: networking.c ftpDefs.h
	$(CXX) $(CXX_FLAGS) -c -o networking.o networking.c

utilities.o: utilities.c
	$(CXX) $(CXX_FLAGS) -c -o utilities.o utilities.c

clean:
	rm *.o ftpClient ftpServer
