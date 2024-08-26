CC = g++
CFLAGS = -g -Wall

default:	gensystem

gensystem:	main.o Planet.o Star.o StarSystem.o
	$(CC) -o GenSystem main.o Planet.o Star.o StarSystem.o

main.o:
	$(CC) -c main.cpp

Planet.o:
	$(CC) -c Planet.cpp

Star.o:
	$(CC) -c Star.cpp

StarSystem.o:
	$(CC) -c StarSystem.cpp

clean:
	$(RM) GenSystem *.o *~

neat:
	$(RM) *.o *~
