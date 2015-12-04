parse: parse.o
	gcc -Wall -std=c99 -o parse parse.o -lm

parse.o: parse.c parse.h
	gcc -Wall -std=c99 -c parse.c

clean:
	-rm -f *.o parse core
