/* ECE 312 LEC A1
Assignment 1, Fall 2014
EDIT AND SUBMIT THIS FILE
Name: Dr. Dileepan JOSEPH
Student ID: (Instructor)
Percent Original: 100%
Other Contributors: None
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
	if (q.memPtr && q.count*q.bytesEach < q.bytesTotal) {
		memcpy(q.headPtr, msgPtr, q.bytesEach); // Put message at head position
		q.headPtr += q.bytesEach; // Increment head pointer by message size
		if (q.headPtr >= q.memPtr + q.bytesTotal) {
			q.headPtr = q.memPtr; // Wrap head pointer back to beginning
		}
		q.count++; // One more message
		return true; // Put succeeded
	}
	else return false; // Put failed
}

bool qGetMsg(QUEUE& q, void *msgPtr) {
	if (q.memPtr && q.count > q.thresh) {
		memcpy(msgPtr, q.tailPtr, q.bytesEach); // Get message at tail position
		q.tailPtr += q.bytesEach; // Increment tail pointer by message size
		if (q.tailPtr >= q.memPtr + q.bytesTotal) {
			q.tailPtr = q.memPtr; // Wrap tail pointer back to beginning
		}
		q.count--; // One less message
		return true; // Get succeeded
	}
	else return false; // Get failed
}

void qFree(QUEUE& q) {
	free(q.memPtr); // Deallocate heap memory
	q.memPtr = 0; // Signifies invalid queue
}
