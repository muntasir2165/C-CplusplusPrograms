# name:               Stef Nychka
# ONE Card number:    9999999
# Unix id:            stef
# lecture section:    A1
# instructor's name:  Dr. Davood Rafiei
# lab section:        D01
# TA's name:          Kiana Hajebi
#
# Creates a parse executable, and provides a make clean.
#

parse: parse.o
	gcc -Wall -std=c99 -o parse parse.o -lm

parse.o: parse.c parse.h
	gcc -Wall -std=c99 -c parse.c

clean:
	-rm -f *.o parse core
