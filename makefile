
test: cnatural.o test.c
	c++ test.c cnatural.o -o test

cnatural.o: cnatural.h cnatural.c
	c++ -c cnatural.c

