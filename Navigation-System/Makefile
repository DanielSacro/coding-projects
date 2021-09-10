# // ---------------------------------------------------
# // Name: Daniel Sacro
# // CMPUT 275, Winter 2021
# //
# // Major Assignment 1: Navigation System (Individual)
# // ---------------------------------------------------

all: server client

server: server/server.o server/dijkstra.o server/digraph.o
	g++ server/server.o server/dijkstra.o server/digraph.o -o server/server

server/server.o: server/server.cpp
	g++ server/server.cpp -o server/server.o -c

server/dijkstra.o: server/dijkstra.cpp
	g++ server/dijkstra.cpp -o server/dijkstra.o -c

server/digraph.o: server/digraph.cpp
	g++ server/digraph.cpp -o server/digraph.o -c

client: client/client.o
	g++ client/client.o -o client/client

client/client.o:
	g++ client/client.cpp -o client/client.o -c

clean:
	rm -f server/server server/server.o server/dijkstra.o server/digraph.o
	rm -f client/client client/client.o
	unlink inpipe
	unlink outpipe
