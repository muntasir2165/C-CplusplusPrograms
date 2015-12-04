#ifndef DRAW1B_H
#define DRAW1B_H

/*include libraries*/
#include <stdio.h> //for sscanf(), fprintf(), printf()...
#include <stdlib.h> //for exit()...
#include <math.h> //for lround(), sin(), cos()...
#include <string.h> //for strtok()...
#include <ctype.h> //for isspace()...
#include "memwatch.h" //for the MEMWATCH memory application

/*macros declarations*/

/*the expected number of command line arguments*/
#define NUM_ARGS 2

/*the maximum length for the input file's name*/
#define PROG_LEN 128

/*the maximum length for an input line (including
the newline character)*/
#define LINE_LEN 256

/*for conversion of angles from degrees to radians*/
#define PI acos(-1.0)

/*a user-defined struct to store all the (x,y) coordinate
information of an image*/
struct imageInfo {
    char name[LINE_LEN];
    int isImageEmpty;
    double *x;
    double *y;
    double currentX;
    double currentY;
    int totalPoints;
};

/*user-defined function prototypes*/
void comment(char line[]);
int addImage(FILE *input, char line[], struct imageInfo **imagesAddress,
    int arraySize, int *lineNumberAddress, int *memoryAllocatedAddress);
int draw(char line[], struct imageInfo *images, FILE* executable,
    int arraySize);
int print(char line[], struct imageInfo *images, int arraySize);
int translate(char line[], struct imageInfo *images, int arraySize);
void child(char line[], FILE* executable);
int rotate(char line[], struct imageInfo *images, int arraySize);
int desiredImage(char imageName[], struct imageInfo *images, int arraySize);
void errorPrint(int lineNumber, int inputOpen, FILE *input,
    int executableOpen, FILE *executable, int memoryAllocated,
    struct imageInfo *images, int arraySize);

/*non-C99 compliant function prototypes.*/
FILE *popen(const char *command, const char *type);
int pclose(FILE *stream);
long int lround(double x);

#endif