// ECE 312 LEC A1
// Assignment 6, Fall 2014
// Author: Dr. Dileepan Joseph
// Student ID: (Instructor)
// Percent Original: 100%
// Other Contributors: None

#include <stdio.h> // For standard C file input/output
#define PAUSE -1 // Symbolize pause in user input

/* This function takes two integers, key and tapsMinusOne, and returns a printable character according to the multi-tap specifications of a classic Nokia handset. The first input must range from 0 to 9, which represents a numeric key that was tapped. The number of taps less one, modulo the total number of symbols specified for the key, is given by the second input.
*/
char getSymbol(int key, int tapsMinusOne) {
	/* A constant 1D array of strings is used to store all possible symbols
	that may be selected by tapping keys 0 to 9. Unlike a 2D array, where all
	1D sub-arrays must have the same length, string lengths may vary. */
	const char *SYMBOLS[] = {
		" 0",    // Key 0
		",.'?!\"1-()@/:_", // Key 1
		"ABC2",  // Key 2
		"DEF3",  // Key 3
		"GHI4",  // Key 4
		"JKL5",  // Key 5
		"MNO6",  // Key 6
		"PQRS7", // Key 7
		"TUV8",  // Key 8
		"WXYZ9"  // Key 9
	};
	/* The 1D string array may be indexed like a 2D character array. */
	return SYMBOLS[key][tapsMinusOne];
}

/* This function takes two integers, key and tapsMinusOne, and updates the latter (via pass by reference). The first value must range from 0 to 9, which represents a numeric key tapped on a classic Nokia handset. Modulo the total number of symbols the key represents, the second variable is incremented.
*/
void countTaps(int key, int& tapsMinusOne) {
	/* These numbers correspond to each string in getSymbol above. */
	const int NUMSYMS[10] = {
		2,  // Key 0
		14, // Key 1
		4,  // Key 2
		4,  // Key 3
		4,  // Key 4
		4,  // Key 5
		4,  // Key 6
		5,  // Key 7
		4,  // Key 8
		5   // Key 9
	};
	/* Apart from ensuring the correct symbol is selected from strings of
	limited length, the modulo operation prevents arithmetic overflow. */
	tapsMinusOne = (tapsMinusOne+1) % NUMSYMS[key];
}

/* This function takes an output file pointer, fout, which has been opened for text writing, and an integer, key, which ranges from 0 to 9 or equals PAUSE, and implements the multi-tap features of a classic Nokia handset. The function has two state variables. The first stores the last key value passed, or PAUSE upon initialization. Whenever the first equals PAUSE, the second equals zero. Otherwise, the second variable counts the number of repeated key values. The counter is reset and a character is outputted, based on Nokia specifications, when the previous key is not PAUSE and the current key differs from it.
*/
void multiTap(FILE *fout, int key) {
	static int keyPrev = PAUSE; // "Key" that was previously tapped
	static int tapsMinusOne = 0; // Count of non-PAUSE taps less one
	if (keyPrev != PAUSE) {
		if (keyPrev != key) {
			fputc(getSymbol(keyPrev, tapsMinusOne), fout);
			tapsMinusOne = 0; // Reset counter (new key)
		}
		else countTaps(key, tapsMinusOne);
	}
	keyPrev = key; // Simple update of state variable
}
