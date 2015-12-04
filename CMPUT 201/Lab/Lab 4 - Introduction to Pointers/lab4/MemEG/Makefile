# A sample Makefile that can be used as a template for asn2.  Mainly
# shows you how to use MEMWATCH.  Note it can be done other ways, using
# more advanced Makefile features.
# 
# Uncomment indicated lines below by removing the appropriate #
#
# Note this Makefile doesn't have perfect style (e.g. no header) and
# will likely not work directly with your code.  So, you will have to
# modify it to work with your asn2.

# You need to link all .o files (corresponding to all .c files)
memEG: memEG.o memwatch.o # UNCOMMENT!
	gcc -std=c99 -Wall -o memEG memEG.o memwatch.o # UNCOMMENT!

# Have as prereqs. all .h files included in .c file that corresponds to
# target (in this case memEG.c corresponds to target memEG.o)
# 
# For each .c file, when compiling into a .o, have -D flags for MEMWATCH
memEG.o: memEG.c memwatch.h # UNCOMMENT!
	gcc -std=c99 -Wall -c memEG.c -DMEMWATCH -DMW_STDIO # UNCOMMENT!

# For each .c file, add rules similar to the preceding one

# UNCOMMENT!
memwatch.o: memwatch.c memwatch.h
	gcc -std=c99 -Wall -c memwatch.c -DMEMWATCH -DMW_STDIO # UNCOMMENT!

clean:
	-rm -f *.o memEG core
