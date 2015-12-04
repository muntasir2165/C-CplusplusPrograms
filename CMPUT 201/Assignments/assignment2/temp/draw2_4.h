#ifndef DRAW1B_H
#define DRAW1B_H

/*macros declarations*/

/*the expected number of command line arguments*/
#define NUM_ARGS 2

/*the maximum length for the input file's name*/
#define PROG_LEN 128

/*the maximum length for an input line (including
the newline)*/
#define LINE_LEN 256

/*there will be no more than 16 distinct images*/
#define IMAGE_NUM 16 

/*an image will never define more than 16
 line segments*/
#define LINE_NUM 16 

/*for conversion of angles from degrees to radians*/
#define PI acos(-1.0)

/*a user-defined struct to store all the coordinate
information of an image*/
struct imageInfo {
    char name[LINE_LEN];
    int isImageEmpty;
    double x[LINE_NUM+1];
    double y[LINE_NUM+1];
    double currentX;
    double currentY;
    int totalPoints;
};

/*user-defined function prototypes*/
void comment(char line[]);
int addImage(FILE *input, char line[], struct imageInfo **imagesAddress, int arraySize);
void draw(char line[], struct imageInfo *images, FILE* executable, int arraySize);
void print(char line[], struct imageInfo *images, int arraySize);
void translate(char line[], struct imageInfo *images, int arraySize);
void child(char line[], FILE* executable);
void rotate(char line[], struct imageInfo *images, int arraySize);
int desiredImage(char imageName[], struct imageInfo *images, int arraySize);
//void errorPrint(int lineNumber, int inputOpen, int childOpen, int memoryAllocated);

/*creates a file called sketchpad.out based on the 
commands from the input file*/
//const char exec_name[] = "java -jar Sketchpad.jar -d"; 

/*creates a new graphical window and draws images (or
more precisely, line segments) based on the input file*/
const char exec_name[] = "java -jar Sketchpad.jar";

/*non-C99 compliant function prototypes.*/
FILE *popen(const char *command, const char *type);
int pclose(FILE *stream);
long int lround(double x);

#endif
