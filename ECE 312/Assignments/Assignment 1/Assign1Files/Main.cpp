/* ECE 312 LEC A1
Assignment 1, Fall 2014
DO NOT EDIT/SUBMIT THIS FILE
*/
#define _CRT_SECURE_NO_WARNINGS // Allow unsafe file operations (VC++)
#include <stdio.h> // Use standard input/output code
#include <ctype.h> // Use standard ASCII-related code
#include "Queue.h" // Use custom queue code

QUEUE qVideo; // Global queue for producer-consumer cooperation

/* This function simulates a producer task that models downloading of video
packets from the Internet. Packets are represented by alphabetic characters.
The function is given a file pointer, fin, for text input. It also has three
states. The initial state ends when two integers are read from the input file,
which are used to initialize the maximum and minimum sizes of a global queue.
During the main state that follows, characters are read in sequence from the
input. An asterisk means no packet was received although more are expected.
Valid packets (letters) are put into the queue. Any other inputted character
means downloading is complete. A wait state follows, during which invalid
packets (zeros) are put into the queue to signal the consumer to free the
queue. Thereafter, the producer returns to the initial state.
*/
void producer(FILE *fin) {
	/* A static variable is used to represent task state. */
	static enum { INIT, MAIN, WAIT } state = INIT;
	int packet; // Used by two task states
	switch (state) {
	case INIT: // Initial state (establishing connection)
		unsigned int max, min; // For queue parameters
		if (fscanf(fin, "%u %u ", &max, &min) == 2) {
			qInit(qVideo, sizeof(int), max, min);
			state = MAIN; // Queue initialized
		}
		break;
	case MAIN: // Main state (downloading video packets)
		packet = fgetc(fin); // N.B. Return type is int
		/* Success/failure of qPutMsg is ignored. */
		if (isalpha(packet)) qPutMsg(qVideo, &packet);
		else if (packet != '*') state = WAIT; // End of video
		break;
	case WAIT: // Wait state (waiting for freed queue)
		packet = 0; // Null packet signals consumer
		if (qVideo.memPtr) qPutMsg(qVideo, &packet);
		else state = INIT; // Queue has been freed
		break;
	}
}

/* This function simulates a consumer task that models playback of downloaded
video packets. Packets, which consist of 4 frames, are represented by letters.
The function is given a file pointer, fout, for text output. It also has three
task states and two state variables, which store the last packet and the index
of the last frame played. The initial state ends when the producer suitably
initializes a global queue. Queue parameters are written to the file. During
the main state that follows, asterisks are written to the file when the task
gets no packets from the queue. The task frees the queue and returns to the
initial state if it gets an invalid packet. If it gets a valid packet, the
first frame is played (letter outputted) before switching to a loop state,
where other frames are played before returning to the main state.
*/
void consumer(FILE *fout) {
	/* Static variables are used to represent state information. */
	static enum { INIT, MAIN, LOOP } state = INIT; // Task state
	static int packet, frame; // Used by two task states
	switch (state) {
	case INIT: // Initial state (waiting for valid queue)
		if (qVideo.memPtr) {
			/* Begin playback (echo queue parameters to output). */
			unsigned int max = qVideo.bytesTotal / qVideo.bytesEach;
			fprintf(fout, "%u\t%u\t", max, qVideo.thresh);
			state = MAIN; // Queue initialized
		}
		break;
	case MAIN: // Main state (waiting for video packet)
		if (qGetMsg(qVideo, &packet)) {
			if (isalpha(packet)) {
				frame = 1; // Initialize frame state
				fputc(packet, fout); // Play first frame
				state = LOOP; // Continue playback
			}
			else {
				qFree(qVideo); // Free the queue
				fprintf(fout, "\n"); // End playback
				state = INIT; // Ready for next video
			}
		}
		else fputc('*', fout); // Playback delayed
		break;
	case LOOP: // Loop state (playing of video packet)
		frame++; // Increment frame state
		fputc(packet, fout); // Play next frame
		if (frame == 4) state = MAIN; // Wait for next packet
		break;
	}
}

/* This function takes an input file pointer, fin, and an output file pointer,
fout, that have been opened for text reading and writing, respectively. It
gives ownership of fin and fout to producer and consumer tasks, respectively,
which are repeatedly called in sequence to simulate cooperative multi-tasking.
After the producer task exhausts the input, this function terminates.
*/
void testSolution(FILE *fin, FILE *fout) {
	qInit(qVideo, 0, 0); // Initialize global queue to invalid state
	while (!feof(fin)) { // Exit after input end-of-file is detected
		producer(fin); // Simulate context switch to producer task
		consumer(fout); // Simulate context switch to consumer task
	}
}

/* This program simulates a video streaming application. It may be called with
zero, one, or two command-line arguments (extra arguments are ignored). First
and second arguments are the names of input and output files, respectively, if
given. Otherwise, standard input and output are used. Upon exit, the program
returns zero if there are no file open/close errors.
*/
int main(int argc, char *argv[]) {
	/* Unless file(s) is/are opened, standard input/output is used by default. */
	FILE *fin = stdin, *fout = stdout;
	/* Including the command, argc and argv provide all command-line strings. */
	if (argc > 1) {
		fin = fopen(argv[1], "r"); // Open text file for read
		if (fin == 0) return 1;
		if (argc > 2) {
			fout = fopen(argv[2], "w"); // Open text file for write
			if (fout == 0) return 2;
		}
	}
	testSolution(fin, fout);
	if (argc > 1) {
		if (fclose(fin)) return 3;
		if (argc > 2) {
			if (fclose(fout)) return 4;
		}
	}
	return 0;
}
