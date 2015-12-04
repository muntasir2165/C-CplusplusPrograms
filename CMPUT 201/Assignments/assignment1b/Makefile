# name:               Mohammad Sirajee
# ONE Card number:    1255986
# Unix id:            sirajee
# lecture section:    EB1
# instructor's name:  Jacqueline Smith
# lab section:        EH02
# TA's name:          Nicolas Barriga, Shida He,
#                     and Rong Feng
#
# Creates a draw1b executable, and provides a make clean.
#

draw1b: draw1b.o
	gcc -std=c99 -Wall draw1b.o -o draw1b -lm

draw1b.o: draw1b.c
	gcc -Wall -std=c99 -c draw1b.c

clean:
	-rm -f *.o draw1b core
