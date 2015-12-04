# Not a good example of a Makefile
DEBUG=-g
CFLAGS=-std=c99 -Wall -DMEMWATCH -DMW_STDIO $(DEBUG)
CC=gcc
Exec=avrg

avrg: memwatch.o avrg.o
avrg.o: avrg.c 
memwatch.o: memwatch.c

# for generating input file ... not part of e.g.
gen: gen.c

# contains an inf. loop
inf: inf.c

clean:
	-rm -f core *.o $(Exec)
