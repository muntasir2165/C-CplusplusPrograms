// ECE 312 LEC A1
// Assignment 9, Fall 2014
// Author: Dr. Dileepan Joseph

#define _CRT_SECURE_NO_WARNINGS // Allow unsafe file operations
#include <stdio.h> // For standard C file input/output

// See Edit.cpp for comment header. This is just a prototype.
int zeroCrossCounter(int, int);

/* This function takes an input file pointer, fin, and an output file pointer, fout, that have been opened for text reading and writing, respectively. It reads integers from the input, two at a time, until the end of file. Each pair of integers represents two inputs at a moment in time for a zero-crossing counter (ZCC) operation. The ZCC output is computed and written to the output file in aligned columns, where each row has ten ZCC outputs in sequence.
*/
void testSolution(FILE *fin, FILE *fout) {
	int count = 0; // Count ZCC outputs to insert newline
	while (!feof(fin)) { // Exit loop when end-of-file detected
		int input, thresh; // Read these variables from input file
		int items = fscanf(fin, "%d %d", &input, &thresh);
		if (items == 2) { // Avoid end-of-file issues
			// Compute ZCC output and write it to the output file:
			fprintf(fout, "%6d", zeroCrossCounter(input, thresh));
			count = ++count % 10; // Zero every ten ZCC outputs
			if (!count) fprintf(fout, "\n"); // Insert newline
		}
	}
}

/* This program demonstrates a digital signal processing (DSP) function. It may be called with zero, one, or two command-line arguments (extra arguments are ignored). First and second arguments are the names of input and output files, respectively, if given. Otherwise, standard input and output are used. Upon exit, the program returns zero if there are no file open/close errors.
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
