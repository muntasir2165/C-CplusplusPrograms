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

int main(int argc, char * argv[]) {
    //declare FILE pointers for the input and executable files
    FILE* landscapeFile = NULL;
    FILE* executable = NULL;

    //flags for the landscape and executable pointers
    int landscapeFileOpen = 0;
    int executableOpen = 0;

    //create a new graphical window and draws images (or more precisely, line
    //segments) based on the input file
    const char exec_name[] = "java -jar Sketchpad.jar";

    //check if we have the right number of command line arguments
    if (argc < NUM_ARGS || argc > NUM_ARGS) {
        fprintf(stderr, "need flags -g gravity, -t thrust, -f landscape.txt\n");
    }

    double gravity = 0;
    double thrust = 0;
    char fileName[PROG_LEN];
    //memset(fileName, 0, PROG_LEN);

    for (int i = 1; i < argc-1; i++) {
        char flag;
        sscanf(argv[i], "-%c", &flag);

        switch(flag) {
            case 'g':
                sscanf(argv[i+1], "%lg", &gravity);
                break;
            case 't':
                sscanf(argv[i+1], "%lg", &thrust);
                break;
            case 'f':
                sscanf(argv[i+1], "%s", fileName);
                if (i < argc-2) {
                    i++;
                }
                break;
        }
    }
    printf("gravity: %lg\n", gravity);
    printf("thrust: %lg\n", thrust);
    printf("filename: %s\n", fileName);

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
    
    //the lunar lander parameters
    //double bottom_left[2] = {300, 40};
    //double top_left[2] = {310, 5};
    //double top_right[2] = {330, 5};
    //double bottom_right[2] = {340, 40};
    //double thruster_intersect[2] = {320, 48};
    double lander[10] = {300, 40, 310, 5, 330, 5, 340, 40, 320, 48};
    int drawThruster = 0;
    drawLander(executable, &lander, &drawThruster);
    /*
    //draw the lander
    fprintf(executable, "drawSegment %ld %ld %ld %ld\n", lround(lander[0][0]), 
            lround(lander[0][1]), lround(lander[1][0]), lround(lander[1][1])); 
    fprintf(executable, "drawSegment %ld %ld %ld %ld\n", lround(lander[1][0]), 
            lround(lander[1][1]), lround(lander[2][0]), lround(lander[2][1]));
    fprintf(executable, "drawSegment %ld %ld %ld %ld\n", lround(lander[2][0]), 
            lround(lander[2][1]), lround(lander[3][0]), lround(lander[3][1]));
    fprintf(executable, "drawSegment %ld %ld %ld %ld\n", lround(lander[3][0]), 
            lround(lander[3][1]), lround(lander[0][0]), lround(lander[0][1]));
    fprintf(executable, "drawSegment %ld %ld %ld %ld\n", lround(lander[0][0]), 
            lround(lander[0][1]), lround(lander[4][0]), lround(lander[4][1]));
    fprintf(executable, "drawSegment %ld %ld %ld %ld\n", lround(lander[3][0]), 
            lround(lander[3][1]), lround(lander[4][0]), lround(lander[4][1])); 
    */
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