all: prog

clean:
	rm -rf prog *.o

main.o: main.c
	gcc -c -o main.o main.c

vector.o: vector.c
	gcc -c -o vector.o vector.c

prog: main.o vector.o
	gcc -o prog main.o vector.o
