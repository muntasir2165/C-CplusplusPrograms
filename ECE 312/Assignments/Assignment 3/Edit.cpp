// ECE 312 LEC A1
// Assignment 3, Fall 2014
// Author: Dr. Dileepan Joseph
// Student ID: (Instructor)
// Percent Original: 100%
// Other Contributors: None

#include <thread> // For standard C++ multi-threading
#include <mutex> // For standard C++ mutual exclusion

using namespace std; // Avoid std:: prefixes below
typedef unsigned int uint; // Unsigned integer type

// See Main.cpp for comment header. This is just a prototype.
void randMatrix(uint, uint, double *);

// See Main.cpp for comment header. This is just a prototype.
void dispMatrix(const char *, uint, uint, const double *);

// This function takes a matrix A, an input array in row-major order of given size rows-by-cols, a vector x, an input array of size cols-by-1, and a vector y, an input/output array of size rows-by-1. Although sufficient memory (input aspect) must have been allocated for y, initial contents do not matter. The matrix-vector product A * x is computed sequentially, using one thread, and returned in y.
void seqMultiply(uint rows, uint cols, const double *A, const double *x, double *y) {
	// Outer loop goes over rows of A and rows of y:
	for (uint i = 0; i < rows; i++) {
		y[i] = 0; // Initialize output (row of y) to zero
		// Inner loop goes over columns of A and rows of x:
		for (uint j = 0; j < cols; j++) {
			y[i] += *A * x[j]; // Accumulate output (row of y)
			A++; // Next element of matrix A (row-major order)
		}
	}
}

// This function takes a matrix A, an input array in row-major order of given size rows-by-cols, a vector x, an input array of size cols-by-1, and a vector y, an input/output array of size rows-by-1. Although sufficient memory (input aspect) must have been allocated for y, initial contents do not matter. The matrix-vector product A * x is computed concurrently, using THREADS threads, and returned in y. By partitioning the matrix A and vector y into groups of consecutive rows, i.e., floor(1 / THREADS) rows each in THREADS - 1 partitions and the remaining rows in a final partition, each thread does approximately 1 / THREADS of the work. However, if the number of rows is less than THREADS, sequential multiplication is used (one thread only).
void conMultiply(uint rows, uint cols, const double *A, const double *x, double *y) {
	const uint THREADS = 4; // Number of threads
	if (rows < THREADS)
		seqMultiply(rows, cols, A, x, y); // Do sequentially (too few rows)
	else {
		thread partition[THREADS - 1]; // Construct "empty" thread array
		uint rowsEach = rows / THREADS; // Rows of A and y per partition
		uint elmsEach = rowsEach * cols; // Elements of A per partition
		for (uint k = 0; k < THREADS - 1; k++) {
			// Start a thread to compute one partition of y = A * x:
			partition[k] = thread(seqMultiply, rowsEach, cols, A, x, y);
			A += elmsEach; // Base address of matrix A in next partition
			y += rowsEach; // Base address of vector y in next partition
			rows -= rowsEach; // Number of rows still to be computed
		}
		// Use main thread to compute last partition:
		seqMultiply(rows, cols, A, x, y);
		// Main thread waits for other threads to finish:
		for (uint k = 0; k < THREADS - 1; k++)
			partition[k].join();
	}
}
