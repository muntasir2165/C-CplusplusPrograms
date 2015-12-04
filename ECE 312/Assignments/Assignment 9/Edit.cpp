// ECE 312 LEC A1
// Assignment 9, Fall 2014
// Author: Dr. Dileepan Joseph
// Student ID: (Instructor)
// Percent Original: 100%
// Other Contributors: None

#include <math.h> // For standard C math functions

/* This function takes two integers, input and thresh, and returns an integer. It returns 1 if input is greater than the absolute value of thresh, -1 if it is less than minus the absolute value of thresh, and 0 otherwise. Consequently, the function returns the sign of input subject to threshold thresh.
*/
int sign(int input, int thresh) {
	thresh = abs(thresh);
	if (input > thresh) return 1;
	else if (input < -thresh) return -1;
	else return 0;
}

/* This function takes one integer, input, and has one integer state, called state, which is initially zero. The state is 1 if the most recent nonzero input was positive, and is -1 if it was negative. The function returns an integer, which is 1 if the input and state are nonzero and have opposite sign, and which is 0 otherwise. In this way, the function detects zero crossing of the input.
*/
int zeroCross(int input) {
	static int state = 0;
	// Product of two numbers is negative only for opposite signs:
	int output = input*state < 0;
	// Zero inputs do not alter state, only positive/negative inputs:
	if (input > 0) state = 1;
	else if (input < 0) state = -1;
	return output;
}

/* This function takes one integer, input, and has one integer state, called state, which is used as a counter to obtain the following behaviour. If the input is nonzero then the output, an integer, is the number of inputs, current one included, since the last nonzero input or since processing started (when there has been no previous nonzero). Otherwise, the output is zero.
*/
int counter(int input) {
	static int state = 0;
	// Assume first that the input is zero (simplifies logic):
	int output = 0;
	state++; // Increment the counter (state variable)
	if (input) {
		// Input is actually nonzero so revise the output and state:
		output = state;
		state = 0; // Reset the counter (state variable)
	}
	return output;
}

/* This function takes two integers, input and thresh, and returns an integer. First, the sign of input is determined subject to thresh, a threshold used to filter noise around the zero level. Next, zero crossings are detected whenever the thresholded input changes sign. Hysteresis is used to ignore zeros, which have no sign. At a zero crossing, the function returns the number of inputs, current one included, since the last crossing or since processing began (if there was no previous crossing). Otherwise, the output is zero.
*/
int zeroCrossCounter(int input, int thresh) {
	int output = sign(input, thresh); // Sign of input with thresholding
	output = zeroCross(output); // Detect zero crossing with hysteresis
	output = counter(output); // Count interval since last crossing
	return output; // Return zero-crossing interval (nonnegative)
}
