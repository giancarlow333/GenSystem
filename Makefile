CC = g++
CFLAGS = -g -Wall

default:	gensystem

gensystem:	main.o
	$(CC) -o GenSystem main.o

main.o:
	$(CC) -c main.cpp

clean:
	$(RM) GenSystem *.o *~
