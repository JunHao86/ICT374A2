#makefile for myftp client

myftpd: myftpd.o stream.o 
	gcc -Wall myftpd.o stream.o -o myftpd

myftpd.o: myftpd.c
	gcc -c -Wall myftpd.c -o myftpd.o
	
stream.o: stream.c stream.h
	gcc -c -Wall stream.c -o stream.o

clean:
	rm *.o myftpd
