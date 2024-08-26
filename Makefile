CC = g++
CFLAGS = -g -Wall

default:	gensystem

gensystem:	main.o Planet.o Star.o
	$(CC) -o GenSystem main.o Planet.o Star.o

main.o:
	$(CC) -c main.cpp

Planet.o:
	$(CC) -c Planet.cpp

Star.o:
	$(CC) -c Star.cpp

clean:
	$(RM) GenSystem *.o *~

neat:
	$(RM) *.o *~
