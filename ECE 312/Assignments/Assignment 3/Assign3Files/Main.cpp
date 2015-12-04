// ECE 312 LEC A1
// Assignment 3, Fall 2014
// Author: Dr. Dileepan Joseph

#define _CRT_SECURE_NO_WARNINGS // Allow unsafe file operations
#include <stdio.h> // For standard C file input/output
#include <stdlib.h> // For standard C random numbers
#include <thread> // For standard C++ multi-threading
#include <mutex> // For standard C++ mutual exclusion
#include <chrono> // For standard C++ time operations

using namespace std; // Avoid std:: prefixes below
typedef unsigned int uint; // Unsigned integer type

// See Edit.cpp for comment header. This is just a prototype.
void seqMultiply(uint, uint, const double *, const double *, double *);

// See Edit.cpp for comment header. This is just a prototype.
void conMultiply(uint, uint, const double *, const double *, double *);

// This function takes a matrix (array M) of given size (rows-by-cols) and initializes each matrix element to a floating-point random number between 0.0 and 1.0 inclusive. N.B. M must point to memory of sufficient size that has been already allocated.
void randMatrix(uint rows, uint cols, double *M) {
	for (uint k = 0; k < rows*cols; k++) {
		// Cast prevents integer division:
		*M = rand() / double(RAND_MAX);
		M++; // Next matrix element
	}
}

// This function takes two matrices (arrays M1 and M2), assumed to be of the same given size (rows-by-cols), and returns the root-mean-square error across all corresponding elements. The computation uses simple pointer arithmetic for efficiency.
double calcRMSE(uint rows, uint cols, const double *M1, const double *M2) {
	double SSE = 0; // Initialize sum of square errors (SSE)
	uint numel = rows*cols; // Total number of elements
	for (uint k = 0; k < numel; k++) {
		double err = *M1 - *M2; // Error in one element
		SSE += err*err; // Accumulate square errors
		M1++; // Next element of matrix 1
		M2++; // Next element of matrix 2
	}
	return sqrt(SSE / numel); // Root-mean-square error
}

// This function takes the name (name), size (rows-by-cols), and elements (array M in row-major order) of a matrix and outputs it, in MATLAB format, to a file called "Debugging.m". Thanks to a static boolean variable, the function creates/overwrites the file when it is first called. Thereafter, it appends to the file. Also, using a static mutual exclusion object, the function is safe for multi-threading use.
void dispMatrix(const char *name, uint rows, uint cols, const double *M) {
	static bool invoked = false; // Has the function been invoked before?
	static mutex critSection; // In case function is invoked concurrently
	critSection.lock(); // Guarantee file is manipulated sequentially
	FILE *mout; // Pointer to output file used to display matrices
	if (invoked) {
		mout = fopen("Debugging.m", "a"); // Append to existing text file
	}
	else {
		mout = fopen("Debugging.m", "w"); // Create/overwrite text file
		invoked = true; // Append to file on next invocation
	}
	if (mout) { // Was file opened successfully?
		fprintf(mout, "%s = [\n", name); // Start of matrix
		for (uint i = 0; i < rows; i++) {
			for (uint j = 0; j < cols; j++) {
				fprintf(mout, "%12.4g", *M); // Output element
				M++; // Advance pointer to next element
			}
			fprintf(mout, ";\n"); // End of row
		}
		fprintf(mout, "]\n"); // End of matrix
	}
	fclose(mout); // Close output file
	critSection.unlock(); // Release mutex
}

// This function takes three integers (by value) -- rows, cols, and reps -- and returns three floating-point numbers (by reference) -- TSeq, TCon, and RMSE. Using randMatrix, the function creates a random rows-by-cols matrix A and a random cols-by-1 vector x. Using seqMultiply and conMultiply, the matrix-vector product y = A * x is computed sequentially and concurrently, respectively. TSeq and TCon return the time taken, in seconds, for these operations, averaged over reps repetitions. Using calcRMSE, RMSE returns the error between the sequential and concurrent values for vector y. (Time code is adapted from http://www.cplusplus.com/reference/chrono/steady_clock/.)
void testMultiply(uint rows, uint cols, uint reps, double& TSeq, double& TCon, double& RMSE) {
	// Allocate dynamic memory:
	double *A = new double[rows*cols];
	double *x = new double[cols];
	double *ySeq = new double[rows];
	double *yCon = new double[rows];
	// Initialize input matrices:
	randMatrix(rows, cols, A);
	randMatrix(cols, 1, x);
	// Declarations for time operations:
	using namespace chrono;
	steady_clock::time_point t1, t2;
	duration<double> tspan;
	// Sequential multiplication:
	t1 = steady_clock::now();
	for (uint k = 0; k < reps; k++)
		seqMultiply(rows, cols, A, x, ySeq);
	t2 = steady_clock::now();
	tspan = duration_cast<duration<double>>(t2 - t1);
	TSeq = tspan.count() / reps;
	// Concurrent multiplication:
	t1 = steady_clock::now();
	for (uint k = 0; k < reps; k++)
		conMultiply(rows, cols, A, x, yCon);
	t2 = steady_clock::now();
	tspan = duration_cast<duration<double>>(t2 - t1);
	TCon = tspan.count() / reps;
	// Compare output matrices:
	RMSE = calcRMSE(rows, 1, ySeq, yCon);
	// Free dynamic memory:
	delete[] A;
	delete[] x;
	delete[] ySeq;
	delete[] yCon;
}

// This function takes an input file pointer, fin, and an output file pointer, fout, that have been opened for text reading and writing, respectively. Until the input is exhausted, three parameters are read repeatedly from the input file. For each set of parameters, a simulation is run, using testMultiply above, and the parameters and simulation results are written to one line of the output file. The output file also has a heading row, as well as some information about hardware concurrency.
void testSolution(FILE *fin, FILE *fout) {
	srand(uint(time(0))); // Makes random numbers vary from run to run
	fprintf(fout, "Number of hardware thread contexts: %u\n\n",
		thread::hardware_concurrency()); // Need not be number of CPUs
	fprintf(fout, "%8s%8s%8s%12s%12s%12s\n",
		"Rows", "Cols", "Reps", // Headings for simulation parameters
		"TSeq", "TCon", "RMSE"); // Headings for simulation results
	while (!feof(fin)) { // Exit after end-of-file is detected
		uint rows, cols, reps; // For simulation parameters
		int read = fscanf(fin, "%u %u %u", &rows, &cols, &reps);
		if (read == 3) { // Parameters inputted successfully
			double TSeq, TCon, RMSE; // For simulation results
			testMultiply(rows, cols, reps, TSeq, TCon, RMSE);
			fprintf(fout, "%8u%8u%8u%12.4g%12.4g%12.4g\n",
				rows, cols, reps, // Output simulation parameters
				TSeq, TCon, RMSE); // Output simulation results
		}
	}
}

// This program may be called with zero, one, or two command-line arguments (extra arguments are ignored). First and second arguments are the names of input and output files, respectively, if given. Otherwise, standard input and output are used. See testSolution above, called on the opened files, for a summary of the program. Upon exit, the program returns zero if there are no file open/close errors.
int main(int argc, char *argv[]) {
	// Unless file(s) is/are opened, standard input/output is used by default.
	FILE *fin = stdin, *fout = stdout;
	// Including the command, argc and argv provide all command-line strings.
	if (argc > 1) {
		fin = fopen(argv[1], "r"); // Open text file for read
		if (!fin) return 1;
		if (argc > 2) {
			fout = fopen(argv[2], "w"); // Open text file for write
			if (!fout) return 2;
		}
	}
	testSolution(fin, fout); // Specific to the assignment
	if (argc > 1) {
		if (fclose(fin)) return 3;
		if (argc > 2 && fclose(fout)) return 4;
	}
	return 0; // No file open/close errors
}
