#makefile for myftp client

myftp: myftp.o token.o
	gcc -Wall myftp.o token.o -o c_ex3

myftp.o: myftp.c
	gcc -c -Wall myftp.c

token.o: token.c token.h
	gcc -c -Wall token.c

clean:
	rm *.o
