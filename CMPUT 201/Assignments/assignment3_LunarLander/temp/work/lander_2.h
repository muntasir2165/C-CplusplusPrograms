#ifndef LANDER_H
#define LANDER_H

/*include libraries*/
#include <stdio.h> //for sscanf(), fprintf(), printf()...
#include <stdlib.h> //for exit()...
#include <math.h> //for lround(), sin(), cos()...
#include <curses.h> //for the keyboard inputs
#include <signal.h>
#include <sys/time.h>

/*macros declarations*/
/*the expected number of command line arguments*/
#define NUM_ARGS 7

/*the maximum length for the input file's name*/
#define PROG_LEN 128

/*the maximum length for an input line (including
the newline character)*/
#define LINE_LEN 256

/*for conversion of angles from degrees to radians*/
#define PI acos(-1.0)

/*need to define this macro as functions we're using are not C99
compliant*/
#define _POSIX_C_SOURCE 200112L

// will store alarm signal to indicate we want it blocked
sigset_t block_mask_g;

/*user-defined function prototypes*/


/*functions given with assignment 3*/
void drawLander(FILE* executable, double *landerAddress, int *drawThruster);
void updateParameters();
void setupCurses();
void unsetCurses();
void updateGraphics(int signal);
void drawWithoutBlock();

/*non-C99 compliant function prototypes.*/
FILE *popen(const char *command, const char *type);
int pclose(FILE *stream);
long int lround(double x);

#endif