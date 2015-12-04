#	Does not necessarily have the style and quality required for the 201
#	Assignments.

CC = gcc
CFLAGS = -Wall -std=c99

eg1: eg.c
	$(CC) $(CFLAGS) -o eg1 eg.c
eg2: eg.c
	$(CC) $(CFLAGS) -DMAKECOPY  -o eg2 eg.c

clean:
	-rm -f eg1 eg2
