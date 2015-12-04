// ECE 312 LEC A1
// Assignment 9, Fall 2014
// Author: Mohammad Sirajee
// Student ID: 1255986
// Percent Original: 100%
// Other Contributors: The TA (marker) helped me refactor
//my code to make it work

#include <math.h> // For standard C math functions

//The following three functions are helper functions for
//the zeroCrossCounter() function
//==========================================================
/* A sign function with threshold that determines the sign

Inputs:
u_in - an input (e.g., fron an ADC) passed by value
u_th - a threshold passed by value

Output:
y_sign - the sign output with possible values - 1, -1, or 0
*/
int signFunction(int u_in, int u_th) {
	int y_sign;

	if (u_in > abs(u_th)) {
		y_sign = 1;
	}
	else if (u_in < -abs(u_th)) {
		y_sign = -1;
	}
	else {
		y_sign = 0;
	}

	return y_sign;
}

/* A zero-crossing detector function with hysteresis
that determines whether the signal crosses zero on
the x_axis

Inputs:
y_sign - The output from signFunction()
x_zc - a static state variable for the zero-crossing
	   detector subsystem

Output:
y_zc - the binary output with possible values - 1 or 0
*/
int zeroCrossDetector(int y_sign, int* x_zc) {
	int y_zc;

	if (y_sign * *x_zc < 0) {
		y_zc = 1;
	}
	else {
		y_zc = 0;
	}

	//calculations to determine the state, x_zc, for
	//the next moment in time i.e., the (n+1)th moment
	if (y_sign > 0) {
		*x_zc = 1;
	}
	else if (y_sign < 0) {
		*x_zc = -1;
	}

	return y_zc;
}

/* A counter function that implements a counter with
non-zero trigger

Inputs:
y_zc - The output from zeroCrossDetector() function
x_c - a static state variable for the counter subsystem

Output:
y_zcc - the output from the Digital Signal Processor (DSP)
		that increments x_c if y_zc != 0 i.e., y_zcc
		counts the number samples between zero crossings
*/
int counter(int y_zc, int* x_c) {
	int y_zcc;

	if (y_zc != 0) {
		y_zcc = *x_c + 1;
	}
	else {
		y_zcc = 0;
	}

	//calculations to determine the state, x_c, for
	//the next moment in time i.e., the (n+1)th moment
	if (y_zc != 0) {
		*x_c = 0;
	}
	else {
		*x_c = *x_c + 1;
	}

	return y_zcc;
}
//==========================================================

/* A zero-crossing detector function, with hysteresis
for robustness, that also counts the number of samples
between zero crossings.

The two static local int variables x_zc and x_c are state
variables that keep track of the past states of the zero-
crossing detector and counter functions respectively. The
initial states are taken to be zero and thus, x_zc and x_c
are intialized to be 0.

Inputs:
u_in - an input (e.g., from an ADC) passed by value
u_th - a threshold passed by value

Output:
y_zcc - the output from the Digital Signal Processor (DSP)
		that counts the number samples between zero
		crossings
*/
int zeroCrossCounter(int u_in, int u_th) {

	//all initial states are taken to be zero
	static int x_zc = 0;
	static int x_c = 0;

	int y_sign = signFunction(u_in, u_th);
	int y_zc = zeroCrossDetector(y_sign, &x_zc);
	int y_zcc = counter(y_zc, &x_c);

	return y_zcc;
}
