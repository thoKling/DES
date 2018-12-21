all: main

DES.o: DES.h DES.c
	gcc -Wall -c DES.c

main: DES.o main.c
	gcc -Wall -o main DES.o main.c

clean:
	rm *.o main
