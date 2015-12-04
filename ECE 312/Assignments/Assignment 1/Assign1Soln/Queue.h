/* ECE 312 LEC A1
Assignment 1, Fall 2014
EDIT AND SUBMIT THIS FILE
Name: Dr. Dileepan JOSEPH
Student ID: (Instructor)
*/
#ifndef QUEUE_H // Has the preprocessor already included this file?
#define QUEUE_H // If not then prevent it from been included twice.

/* This structure encapsulates all parameters of a queue for messages of equal
but arbitrary size. The queue employs a circular buffer. Standard operations
are defined below. If memPtr is 0 (NULL) then the queue is invalid.
*/
struct QUEUE {
	size_t bytesTotal; // Number of bytes allocated for queue messages
	char *memPtr; // Pointer to memory allocated for queue messages
	size_t bytesEach; // Number of bytes for each queue message
	unsigned int thresh; // Threshold number of messages for gets
	unsigned int count; // Number of valid messages in the queue
	char *headPtr; // Pointer to queue head (for next put)
	char *tailPtr; // Pointer to queue tail (for next get)
};

/* This function initializes a QUEUE, q, passed by reference (output variable)
and returns true/false if successful/unsuccessful. Input variables each, max,
and min specify the message size in bytes, the maximum number of messages, and
a threshold number. To get a message, the number of messages must exceed the
threshold. Initialization fails if sufficient memory cannot be allocated.
*/
bool qInit(QUEUE& q, size_t each, unsigned int max, unsigned int min = 0);

/* This function copies a message, referenced by msgPtr (input pointer), into
the head of a queue, q (input/output structure), and returns true, unless the
queue is initially full, in which case the function returns false without even
touching the message. The function also returns false if the queue is invalid,
i.e., there is definitely no memory allocated for queue messages. A successful
put increments a message counter and advances a head pointer circularly.
*/
bool qPutMsg(QUEUE& q, const void *msgPtr);

/* This function copies a message, returned by msgPtr (output pointer), from
the tail of a queue, q (input/output structure), and returns true, unless the
number of messages in the queue is less than or equal to a threshold, set at
initialization, in which case the function returns false without copying any
message. If the queue is invalid, i.e., definitely no memory exists for queue
messages, the function also returns false. A successful get decrements a
message counter and advances a tail pointer circularly.
*/
bool qGetMsg(QUEUE& q, void *msgPtr);

/* This function frees the heap memory associated with a valid QUEUE, q, passed
by reference (input/output variable). The memory pointer is reset to 0 (NULL).
*/
void qFree(QUEUE& q);

#endif // QUEUE_H
