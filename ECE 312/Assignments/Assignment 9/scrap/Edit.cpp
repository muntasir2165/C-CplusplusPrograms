// ECE 312 LEC A1
// Assignment 9, Fall 2014
// Author: Mohammad Sirajee
// Student ID: 1255986
// Percent Original: 
// Other Contributors: 

#include <math.h> // For standard C math functions

/* A zero-crossing detector function, with hysteresis
for robustness, that also counts the number of samples
between zero crossings.

The two static local int variables x_zc and x_c are state
variables that keep track of the past states of the zero-
crossing detector and counter functions respectively. The
initial states are taken to be zero and thus, x_zc and x_c
are intialized to be 0.

Inputs:
u_in - an input (e.g., fron an ADC) passed by value
u_th - a threshold passed by value

Output:
y_zcc - the output from the Digital Signal Processor (DSP)
*/
int zeroCrossCounter(int u_in, int u_th) {
	
	//all initial states are taken to be zero
	static int x_zc = 0;
	static int x_c = 0;

	int y_sign = signFunction(u_in, u_th);
	int y_zc = zeroCrossDetector(y_sign, &x_zc);
	int y_zcc = counter(y_zc, &x_c);

	return y_zcc;
	
	/*
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
	

	int y_zc;
	static int x_zc = 0;
	if (y_sign*x_zc < 0) {
		y_zc = 1;
	}
	else {
		y_zc = 0;
	}

	if (y_sign > 0) {
		x_zc = 1;
	}
	else if (y_sign < 0) {
		x_zc = -1;
	}


	int y_zcc;
	static int x_c = 0;
	if (y_zc != 0) {
		y_zcc = x_c + 1;
	}
	else {
		y_zcc = 0;
	}

	if (y_zc != 0) {
		x_c = 0;
	}
	else {
		x_c = x_c + 1;
	}


	return y_zcc;
	*/
}


/* A sign function with threshold that
determines the sign

Inputs:
u_in - an input (e.g., fron an ADC) passed by value
u_th - a threshold passed by value

Output:
y_sign - the sign output with possible values - 1, -1, or 0
		 where 1 indicates positive, 
		 	  -1 indicates negative, and
		       0 indicates to sign change
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
y_sign - The output from the sign function
x_zc - a state variable for the zero-crossing detector
	   subsystem

Output:
y_zc - the binary output with possible values - 1 or 0
		 where 1 indicates ___, and
		       0 indicates ___
*/

int zeroCrossDetector(int y_sign, int* x_zc) {
	int y_zc;
	
	if (y_sign * *x_zc < 0) {
		y_zc = 1;
	}
	else {
		y_zc = 0;
	}

	//calculations to determine the state for the
	//next moment in time i.e., the (n+1)th moment
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
y_zc - The output from the zero-crossing detector function
x_c - a state variable for the counter subsystem

Output:
y_zcc - the output from the Digital Signal Processor (DSP)
*/
int counter(int y_zc, int* x_c) {
	int y_zcc;
	
	if (y_zc != 0) {
		y_zcc = *x_c + 1;
	}
	else {
		y_zcc = 0;
	}

	//calculations to determine the state for the
	//next moment in time i.e., the (n+1)th moment
	if (y_zc != 0) {
		*x_c = 0;
	}
	else {
		*x_c = *x_c + 1;
	}

	return y_zcc;
}
