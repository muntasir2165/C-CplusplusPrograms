// ECE 312 LEC A1
// Assignment 3, Fall 2014
// Author: 
// Student ID: 
// Percent Original: 
// Other Contributors: 

#include <thread> // For standard C++ multi-threading
#include <mutex> // For standard C++ mutual exclusion

using namespace std; // Avoid std:: prefixes below
typedef unsigned int uint; // Unsigned integer type

// See Main.cpp for comment header. This is just a prototype.
void randMatrix(uint, uint, double *);

// See Main.cpp for comment header. This is just a prototype.
void dispMatrix(const char *, uint, uint, const double *);

// WRITE A COMMENT HEADER HERE IN YOUR OWN WORDS.
void seqMultiply(uint rows, uint cols, const double *A, const double *x, double *y) {
	dispMatrix("A", rows, cols, A);
	dispMatrix("x", cols, 1, x);
	randMatrix(rows, 1, y); // Initialize
	dispMatrix("ySeq", rows, 1, y);
}

// WRITE A COMMENT HEADER HERE IN YOUR OWN WORDS.
void conMultiply(uint rows, uint cols, const double *A, const double *x, double *y) {
	const uint THREADS = 4; // Number of threads
	randMatrix(rows, 1, y); // Initialize
	dispMatrix("yCon", rows, 1, y);
}
