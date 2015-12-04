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

/*amount of rotation (in degrees) per key click*/
#define ANGLE 10

/*difference in time*/
#define t 0.05

/*need to define this macro as functions we're using are not C99
compliant*/
#define _POSIX_C_SOURCE 200112L

/*a user-defined struct to store all the (x,y) coordinate
information of a lander*/
struct landerInfo {
    //the initial lunar lander parameters
    double bottomLeftX;
    double bottomLeftY;
    double topLeftX;
    double topLeftY;
    double topRightX;
    double topRightY;
    double bottomRightX;
    double bottomRightY;
    double thrusterX;
    double thrusterY;
    double a;
    double b;
    double height;
    double centreX;
    double centreY;
};

// will store alarm signal to indicate we want it blocked
sigset_t block_mask_g;

/*user-defined function prototypes*/
void processInput(int *argcAddress, char *argv[]);
void drawLandscape();
void initializeLander();
void drawLander();
void eraseLander();
double rotateX(double x, double y);
double rotateY(double x, double y);
void rotateLander();
void drawThruster();
void eraseThruster();
double newY(double oldY, double oldVelocity);
// double newVelocity(double oldVelocity);
void updateParameters();

/*functions given with assignment 3 (with some modificaitons)*/
void setupCurses();
void unsetCurses();
void initializeTimer(sigset_t block_mask_g);
void updateGraphics(int signal);
void executeWithoutBlock();

/*non-C99 compliant function prototypes.*/
FILE *popen(const char *command, const char *type);
int pclose(FILE *stream);
long int lround(double x);

#endif