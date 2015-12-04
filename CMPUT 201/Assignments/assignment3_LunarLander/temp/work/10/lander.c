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
double velocity = 0;
//flag for determining whether to draw the thruster
int drawThruster = 0;

//store the filename of the landscape.txt file
char fileName[PROG_LEN];
//memset(fileName, 0, PROG_LEN);

//flage for determining when the game is done
int isOver = 0;

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

    //initialize curses
    setupCurses();

    //draw the landscape
    drawLandscape();

    //initialize and draw the lander
    initializeLander();
    drawLander();


    struct timeval start_time;
    gettimeofday(&start_time, NULL);
    struct timeval current_time;
    gettimeofday(&current_time, NULL);
    // while (current_time.tv_sec-start_time.tv_sec < 10) {
    //     //printf("Done!!!\n");
    //     //fflush(stdout);
    //     //printf("start_time: %d------current-time:%d\n", start_time.tv_sec, current_time.tv_sec);
    //     initializeTimer(block_mask_g);
    //     gettimeofday(&current_time, NULL);
    // }


    //fflush(stdout);
    //fflush stuffs
    //fflush(executable);

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
    
    // must do this or else Terminal will be unusable
    unsetCurses();

    return(0);
    //exit(EXIT_SUCCESS);
}