#
# Creates draw1 executable.  'make clean' will rm all object files,
# the executable and a core file.
#
# Style is not what is expected for asn1 (e.g., no header in this
# comment), and this Makefile is not guaranteed to work with your
# code.  Feel free to use it as a template, though. 
#
# -lm used here only so Makefile looks closer to proper asn1 Makefile

draw1: draw1.o
	gcc -Wall -std=c99 -o draw1 draw1.o -lm

draw1.o: draw1.c draw1.h
	gcc -Wall -std=c99 -c draw1.c -g

clean:
	-rm -f *.o draw1 core
