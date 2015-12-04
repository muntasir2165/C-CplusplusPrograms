/* ECE 312 LEC A1
Assignment 1, Fall 2014
EDIT AND SUBMIT THIS FILE
Name: 
Student ID: 
Percent Original: 
Other Contributors: 
*/
#include <stdlib.h> // Use for memory allocation
#include <string.h> // Use for memory copying
#include "Queue.h" // Use custom queue code

bool qInit(QUEUE& q, size_t each, unsigned int max, unsigned int min) {
	q.bytesTotal = each*max; // Compute heap memory required
	if (q.bytesTotal) { // Allocate heap memory if needed
		q.memPtr = (char *)malloc(q.bytesTotal); // Cast to typed pointer
		if (q.memPtr) {
			q.bytesEach = each; // Save message size for puts/gets
			q.thresh = min; // Minimum message count (limits gets)
			q.count = 0; // Initially, the queue has no messages
			q.headPtr = q.memPtr; // Position for first message
			q.tailPtr = q.memPtr; // Position for first message
		}
		else return false; // Memory allocation failed
	}
	else q.memPtr = 0; // No heap memory is needed
	return true; // Initialization succeeded
}

bool qPutMsg(QUEUE& q, const void *msgPtr) {
	return false; // Put failed
}

bool qGetMsg(QUEUE& q, void *msgPtr) {
	memset(msgPtr, 0, q.bytesEach); // Zero message
	return true; // Get succeeded
}

void qFree(QUEUE& q) {
	free(q.memPtr); // Deallocate heap memory
	q.memPtr = 0; // Signifies invalid queue
}
