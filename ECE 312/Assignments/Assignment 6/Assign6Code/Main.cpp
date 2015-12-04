// ECE 312 LEC A1
// Assignment 6, Fall 2014
// Author: Dr. Dileepan Joseph

#define _CRT_SECURE_NO_WARNINGS // Allow unsafe file operations
#include <stdio.h> // For standard C file input/output
#include <ctype.h> // For standard C ASCII functions
#define PAUSE -1 // Symbolize pause in user input

// See Edit.cpp for comment header. This is just a prototype.
void multiTap(FILE *, int);

/* This function takes an input file pointer, fin, and an output file pointer, fout, that have been opened for text reading and writing, respectively. It reads strings from the input, one line at a time, until the file is exhausted. For each string, it takes the characters in sequence for multi-tap processing. Those in the range '0' to '9' are treated as tapped keys 0 to 9. Terminating '\0' aside, other characters indicate pauses. Results of multi-tap processing are written to fout, one line written for every line read from fin.
*/
void testSolution(FILE *fin, FILE *fout) {
	const size_t BUFSIZE = 100; // Maximum string length, '\0' included
	char buffer[BUFSIZE];
	while (!feof(fin)) { // Exit loop after end-of-file detected
		if (fgets(buffer, BUFSIZE, fin)) { // Avoid end-of-file issues
			for (size_t i = 0; i < BUFSIZE; i++) {
				/* Process each character in the buffered string. */
				if (isdigit(buffer[i])) {
					int key = buffer[i] - '0'; // Convert '0' to 0 etc
					multiTap(fout, key); // Key tap detected
				}
				else if (buffer[i]) multiTap(fout, PAUSE); // Pause detected
				else break; // Terminating '\0' detected
			}
			fprintf(fout, "\n"); // Line completed
		}
	}
}

/* This program simulates multi-tap features of a classic mobile phone. It may be called with zero, one, or two command-line arguments (extra arguments are ignored). First and second arguments are the names of input and output files, respectively, if given. Otherwise, standard input and output are used. Upon exit, the program returns zero if there are no file open/close errors.
*/
int main(int argc, char *argv[]) {
	/* Unless file(s) is/are opened, standard input/output is used by default. */
	FILE *fin = stdin, *fout = stdout;
	/* Including the command, argc and argv provide all command-line strings. */
	if (argc > 1) {
		fin = fopen(argv[1], "r"); // Open text file for read
		if (fin == 0) return 1;
		if (argc > 2) {
			fout = fopen(argv[2], "w"); // Open text file for write
			if (fout == 0) return 2;
		}
	}
	testSolution(fin, fout);
	if (argc > 1) {
		if (fclose(fin)) return 3;
		if (argc > 2) {
			if (fclose(fout)) return 4;
		}
	}
	return 0;
}
