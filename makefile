all: main

MyUtils.o: MyUtils.h MyUtils.c
	gcc -Wall -c MyUtils.c

DES.o: DES.h DES.c
	gcc -Wall -c DES.c

main: MyUtils.o DES.o main.c
	gcc -Wall -o main MyUtils.o DES.o main.c

clean:
	rm *.o main
