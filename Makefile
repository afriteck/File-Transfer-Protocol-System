CXX = g++
CXX_FLAGS = -Wall -Wextra -Werror -pedantic-errors

server: server.o
	$(CXX) $(CXX_FLAGS) -o ./server server.o

server.o: server.cc
