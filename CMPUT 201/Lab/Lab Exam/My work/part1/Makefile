part1: part1.c
	gcc -Wall -std=c99 -g -o part1 part1.c -lm

part12: part1.o memwatch.o
	gcc -std=c99 -Wall part1.o memwatch.o -o part1 -lm

part1.o: part1.c memwatch.h
	gcc -Wall -std=c99 -c part1.c -DMEMWATCH -DMW_STDIO

memwatch.o: memwatch.c memwatch.h
	gcc -std=c99 -Wall -c memwatch.c -DMEMWATCH -DMW_STDIO

clean:
	-rm -f part1
