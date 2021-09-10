all: matrix

matrix: matrix.o
	g++ -o matrix matrix.o

matrix.o: matrix.cpp
	g++ -c matrix.cpp

clean:
	rm -f matrix.o matrix
