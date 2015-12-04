/*
    name:               Mohammad Sirajee
    ONE Card number:    1255986
    Unix id:            sirajee
    lecture section:    EB1
    instructor's name:  Jacqueline Smith
    lab section:        EH02
    TA's name:          Nicolas Barriga, Shida He,
                        and Rong Feng 
*/

#include "lander.h"

/*declare global variables*/
//create a new graphical window and draws images (or more precisely, line
//segments) based on the input file
const char exec_name[] = "java -jar Sketchpad.jar";
const char pipe[] = "dev/ttys000";
FILE* pipePointer = NULL;
//declare FILE pointers for the input and executable files
FILE* landscapeFile = NULL;
FILE* executable = NULL;

//flags for the landscape and executable pointers
int landscapeFileOpen = 0;
int executableOpen = 0;

//lander specific variables
struct landerInfo lander;
double gravity = 0;
double thrust = 0;
//x- and y-displacements
double x = 0;
double y = 0;
//x- and y-velocities
double vX = 0;
double vY = 0;
//net, i.e., overall x- and y-accelerations
double aX = 0;
double aY = 0;
double attitude_radians;

//flag for determining whether to draw the thruster
int thrusterOn = 0;
int rotationDirection;
int landed = 0;

//store the filename of the landscape.txt file
char fileName[PROG_LEN];
//memset(fileName, 0, PROG_LEN);

//flage for determining whether the game is over
int gameOver = 0;

int main(int argc, char * argv[]) {
    //check if we have the right number of command line arguments
    if (argc < NUM_ARGS || argc > NUM_ARGS) {
        fprintf(stderr, "need flags -g gravity, -t thrust, -f landscape.txt\n");
    }

    //process the user input parameters
    processInput(&argc, argv);

    //open the input file for reading
    landscapeFile = fopen(fileName, "r");
    if (landscapeFile == NULL) {
        fprintf(stderr, "Cannot open the landscape file\n");
        exit(EXIT_FAILURE);
    } else {
        landscapeFileOpen = 1;
    }

    //open the sketchpad executable to be run by the child process
    executable = popen(exec_name, "w");
    if (executable == NULL) {
        fprintf(stderr, "Cannot open the Sketchpad executable\n");
        exit(EXIT_FAILURE);
    } else {
        executableOpen = 1;
    }

    pipePointer = popen(pipe, "w");
    if (pipePointer == NULL) {
        fprintf(stderr, "Cannot open the pipe to a new terminal\n");
        exit(EXIT_FAILURE);
    }

    //draw the landscape
    drawLandscape();

    //initialize and draw the lander
    initializeLander();
    drawLander();

    initializeTimer(block_mask_g);

    //close the input file
    if (landscapeFileOpen) {
        fclose(landscapeFile);
    }

    fprintf(executable, "%s\n", " pause 3");

    //close the pipe to Sketchpad and close the executable
    if (executableOpen) {
        fprintf(executable, "%s\n", " end");
        pclose(executable);
    }

    if (pipePointer != NULL) {
        pclose(pipePointer);
    }

    return(0);
    //exit(EXIT_SUCCESS);
}