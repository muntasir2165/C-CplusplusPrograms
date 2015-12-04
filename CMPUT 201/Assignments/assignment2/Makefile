# name:               Mohammad Sirajee
# ONE Card number:    1255986
# Unix id:            sirajee
# lecture section:    EB1
# instructor's name:  Jacqueline Smith
# lab section:        EH02
# TA's name:          Nicolas Barriga, Shida He,
#                     and Rong Feng
#
# Creates a draw2 executable, and provides a make clean.
#

draw2: draw2.o functions.o memwatch.o
	gcc -std=c99 -Wall draw2.o functions.o memwatch.o -o draw2 -lm

draw2.o: draw2.c memwatch.h
	gcc -Wall -std=c99 -c draw2.c -DMEMWATCH -DMW_STDIO

functions.o: functions.c memwatch.h
	gcc -Wall -std=c99 -c functions.c -DMEMWATCH -DMW_STDIO

memwatch.o: memwatch.c memwatch.h
	gcc -std=c99 -Wall -c memwatch.c -DMEMWATCH -DMW_STDIO

clean:
	-rm -f *.o draw2 core

submit.tar: draw2.c functions.c draw2.h Makefile README memwatch.c memwatch.h
	tar cvf submit.tar draw2.c functions.c draw2.h Makefile README memwatch.c memwatch.h