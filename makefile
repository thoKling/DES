all: main

MyUtils.o: MyUtils.h MyUtils.c
	gcc -Wall -c MyUtils.c

DES.o: DES.h DES.c
	gcc -Wall -c DES.c -lm

structure.o: structure.h structure.c
	gcc -Wall -c structure.c 

main: MyUtils.o DES.o structure.o main.c
	gcc -Wall -o main MyUtils.o DES.o structure.o main.c -lm

clean:
	rm *.o main
