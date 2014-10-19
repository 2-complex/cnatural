
CC = gcc -g3

test: cnatural.o cnatural.h wav.o wav.h test.c
	$(CC) test.c cnatural.o wav.o -o test -lm

cnatural.o: cnatural.h cnatural.c
	$(CC) -c cnatural.c

wav.o: wav.c wav.h
	$(CC) wav.c -c
