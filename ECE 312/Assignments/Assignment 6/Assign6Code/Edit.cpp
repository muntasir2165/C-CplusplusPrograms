// ECE 312 LEC A1
// Assignment 6, Fall 2014
// Author: 
// Student ID: 
// Percent Original: 
// Other Contributors: 

#include <stdio.h> // For standard C file input/output
#define PAUSE -1 // Symbolize pause in user input

/* WRITE A COMMENT HEADER HERE IN YOUR OWN WORDS.
*/
void multiTap(FILE *fout, int key) {
	static int count = 0; // State variable
	if (key != PAUSE) fputc(key + '0', fout);
	else count++; // Count pauses
}
