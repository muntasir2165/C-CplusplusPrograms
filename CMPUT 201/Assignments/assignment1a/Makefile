# name:               Mohammad Sirajee
# ONE Card number:    1255986
# Unix id:            sirajee
# lecture section:    EB1
# instructor's name:  Jacqueline Smith
# lab section:        EH02
# TA's name:          Nicolas Barriga, Shida He,
#                     and Rong Feng
#
# Creates a draw1a executable, and provides a make clean.
#

draw1a: draw1a.o
	gcc -Wall -std=c99 -o draw1a draw1a.o

draw1a.o: draw1a.c
	gcc -Wall -std=c99 -c draw1a.c

clean:
	-rm -f *.o draw1a core
