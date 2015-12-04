/* ECE 312 LEC A1
Assignment 1, Fall 2014
EDIT AND SUBMIT THIS FILE
Name: k.Sriaditya
Student ID: 1274639
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

bool qPutMsg(QUEUE& q, const void *msgPtr)
{

		if (q.memPtr) // Check whether memPtr = NULL //
		{
			if (q.bytesTotal > (q.count*q.bytesEach)) // Check whether queue has space where count *size//
			// is the total size of the messages entered, initially 0 //
			{
				memcpy(q.headPtr, msgPtr, q.bytesEach); // Copying the message from source (msgPtr)//
				//(to the destination (headPtr)//
				q.count++; // keeping track of messages by incrementing the pointer //
				q.headPtr = q.headPtr + q.bytesEach; // incrementing the pointer by each message size //
			}
			else
			{
				return false; // condition was not satisfied, put failed.
			}

		}
		else
		{
			return false; // condition was not satisfied, put failed.
		}
		if (q.headPtr >= (q.headPtr + q.bytesTotal))
		{
			q.headPtr = q.memPtr; //position for first message //
		}


}


bool qGetMsg(QUEUE& q, void *msgPtr)
{

	if (q.memPtr) // similar to the above, check to see if memPtr = NULL //
	{
		if (q.count > q.thresh) // check to see if the count is greater than the minimum number of messages //
		{
			memcpy(msgPtr, q.tailPtr, q.bytesEach); // as a new message is copied into the headPtr //
			// in the previous function, here the old message is copied (gets) from the tailPtr //
			// to the memPtr //
			q.count--; // counter is decremented to keep track of the maximum number of messages //
			q.tailPtr = q.tailPtr + q.bytesEach; // The tailPtr is incremented 
		}

		else
		{
			return false; // condition not satisfied //

		}


	}
	else
	{
		return false; //condition not satisfied //
	}

	if (q.tailPtr >= (q.tailPtr + q.bytesTotal))
	{
		q.tailPtr = q.memPtr; //position for first message //
	}
	
	memset(msgPtr, 0, q.bytesEach); // Zero message
	return true; // Get succeeded
}

void qFree(QUEUE& q) {
	free(q.memPtr); // Deallocate heap memory
	q.memPtr = 0; // Signifies invalid queue
}
