CC = g++
CFLAGS = -std=c++17 -g

default:	gensystem

gensystem:	main.o Moon.o Planet.o Star.o
	$(CC) -o GenSystem main.o Moon.o Planet.o Star.o

main.o:
	$(CC) $(CFLAGS) -c main.cpp

Moon.o:
	$(CC) $(CFLAGS) -c Moon.cpp

Planet.o:
	$(CC) $(CFLAGS) -c Planet.cpp

Star.o:
	$(CC) $(CFLAGS) -c Star.cpp

clean:
	$(RM) GenSystem *.o *~

neat:
	$(RM) *.o *~
