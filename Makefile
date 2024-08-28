CC = g++
CFLAGS = -std=c++17

default:	gensystem

gensystem:	main.o Moon.o Planet.o Star.o StarSystem.o
	$(CC) -o GenSystem main.o Moon.o Planet.o Star.o StarSystem.o

main.o:
	$(CC) $(CFLAGS) -c main.cpp

Moon.o:
	$(CC) -c Moon.cpp

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
