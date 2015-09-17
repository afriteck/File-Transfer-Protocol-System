CXX = g++
CXX_FLAGS = -Wall -Wextra -Werror -pedantic-errors

all: client server

server: server.o
	$(CXX) $(CXX_FLAGS) -o ./server server.o

server.o: server.cc

client: client.o
	$(CXX) $(CXX_FLAGS) -o ./client client.o

client.o: client.cc
