# Not a good example of a Makefile
DEBUG=-g
CFLAGS=-std=c99 -Wall -DMEMWATCH -DMW_STDIO $(DEBUG)
CC=gcc
Exec=avrg

avrg: memwatch.o avrg.o
avrg.o: avrg.c memwatch.h
	gcc -std=c99 -Wall -DMEMWATCH -DMW_STDIO -c avrg.c #-g 
memwatch.o: memwatch.c memwatch.h
	gcc -std=c99 -Wall -DMEMWATCH -DMW_STDIO -c memwatch.c #-g 

# for generating input file ... not part of e.g.
gen: gen.c

clean:
	-rm -f core *.o $(Exec)
