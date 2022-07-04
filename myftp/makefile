#makefile for myftp client

myftp: myftp.o stream.o token.o
	gcc -Wall myftp.o token.o stream.o -o myftp

myftp.o: myftp.c
	gcc -c -Wall myftp.c -o myftp.o
	
stream.o: stream.c stream.h
	gcc -c -Wall stream.c -o stream.o

token.o: token.c token.h
	gcc -c -Wall token.c -o token.o

clean:
	rm *.o myftp
