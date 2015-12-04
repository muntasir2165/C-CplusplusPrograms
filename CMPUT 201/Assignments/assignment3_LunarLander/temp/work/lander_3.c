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

int main(int argc, char * argv[]) {

    //check if we have the right number of command line arguments
    if (argc < NUM_ARGS || argc > NUM_ARGS) {
        fprintf(stderr, "need flags -g gravity, -t thrust, -f landscape.txt\n");
    }

    processInput(&argc, argv, &gravity, &thrust, fileName);
    // for (int i = 1; i < argc-1; i++) {
    //     char flag;
    //     sscanf(argv[i], "-%c", &flag);

    //     switch(flag) {
    //         case 'g':
    //             sscanf(argv[i+1], "%lg", &gravity);
    //             break;
    //         case 't':
    //             sscanf(argv[i+1], "%lg", &thrust);
    //             break;
    //         case 'f':
    //             sscanf(argv[i+1], "%s", fileName);
    //             if (i < argc-2) {
    //                 i++;
    //             }
    //             break;
    //     }
    // }
    // printf("gravity: %lg\n", gravity);
    // printf("thrust: %lg\n", thrust);
    // printf("filename: %s\n", fileName);
    //printf("filename again!!!: %s\n", fileName);

    //open the input file for reading
    landscapeFile = fopen(fileName, "r");
    if (landscapeFile == NULL) {
        fprintf(stderr, "Cannot open the landscape file\n");
    } else {
        landscapeFileOpen = 1;
    }

    //open the sketchpad executable to be run by the child process
    executable = popen(exec_name, "w");
    if (executable == NULL) {
        fprintf(stderr, "Cannot open the Sketchpad executable\n");
    } else {
        executableOpen = 1;
    }

    //initialize curses
    setupCurses();

    double x = 0;
    double y = 0;
    double currentX = x;
    double currentY = y;
    char line[LINE_LEN];
    fgets(line, LINE_LEN, landscapeFile);
    sscanf(line, "%lg%lg", &currentX, &currentY);
    //draw the landscape
     while(!feof(landscapeFile)) {
        fgets(line, LINE_LEN, landscapeFile);
        sscanf(line, "%lg%lg", &x, &y);
        fprintf(executable, "drawSegment %ld %ld %ld %ld\n", lround(currentX), 
            lround(currentY), lround(x), lround(y));
        currentX = x;
        currentY = y;
    }
    
    /*
    //the initial lunar lander parameters
    double bottom_left[2] = {300, 40};
    double top_left[2] = {310, 5};
    double top_right[2] = {330, 5};
    double bottom_right[2] = {340, 40};
    double thruster_intersect[2] = {320, 48};
    
    double lander[10] = {300, 40, 310, 5, 330, 5, 340, 40, 320, 48};
    */
    //struct landerInfo lander;
    lander.bottomLeftX = 300;
    lander.bottomLeftY = 40;
    lander.topLeftX = 310;
    lander.topLeftY = 5;
    lander.topRightX = 330;
    lander.topRightY = 5;
    lander.bottomRightX = 340;
    lander.bottomRightY = 40;
    lander.thrusterX = 320;
    lander.thrusterY = 48;
    //printf("before:::::%lg\n", lander.bottomLeftX);
    // lander.bottomLeftX = 450;
    // printf("after:::::%lg\n", lander.bottomLeftX);//
    drawLander(executable, &lander, &drawThruster);
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