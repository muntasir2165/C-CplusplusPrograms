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
                //printf("file:%s\n", argv[i+1]);
                sscanf(argv[i+1], "%s", fileName);
                //printf("i before:%d\n", i);
                if (i < argc-2) {
                    i++;
                }
                //printf("i after:%d\n", i);
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
    setup_curses();

    double x = 0;
    double y = 0;
    double currentX = x;
    double currentY = y;
    char line[LINE_LEN];
    fgets(line, LINE_LEN, landscapeFile);
    sscanf(line, "%lg%lg", &currentX, &currentY);
    //draw the landscape
     while(!feof(landscapeFile)) {
        //printf("line:%s, currentX:%lg, currentY:%lg, x:%lg, y:%lg\n", line, currentX, currentY, x, y);
        //char line[LINE_LEN];
        fgets(line, LINE_LEN, landscapeFile);
        sscanf(line, "%lg%lg", &x, &y);
        //x = currentX;
        //y = currentY;
        //printf("line:%s, currentX:%lg, currentY:%lg, x:%lg, y:%lg\n", line, currentX, currentY, x, y);
        fprintf(executable, "drawSegment %ld %ld %ld %ld\n", lround(currentX), 
            lround(currentY), lround(x), lround(y));
        currentX = x;
        currentY = y;
    }
    
    //the lunar lander parameters
    double top_left[2] = {310, 5};
    double top_right[2] = {330, 5};
    double bottom_left[2] = {300, 40};
    double bottom_right[2] = {340, 40};
    double thruster_intersect[2] = {320, 48};

    fprintf(executable, "drawSegment %ld %ld %ld %ld\n", lround(top_left[0]), 
            lround(top_left[1]), lround(top_right[0]), lround(top_right[1])); 
    fprintf(executable, "drawSegment %ld %ld %ld %ld\n", lround(bottom_left[0]), 
            lround(bottom_left[1]), lround(bottom_right[0]), lround(bottom_right[1]));
    fprintf(executable, "drawSegment %ld %ld %ld %ld\n", lround(top_left[0]), 
            lround(top_left[1]), lround(bottom_left[0]), lround(bottom_left[1]));
    fprintf(executable, "drawSegment %ld %ld %ld %ld\n", lround(top_right[0]), 
            lround(top_right[1]), lround(bottom_right[0]), lround(bottom_right[1]));
    fprintf(executable, "drawSegment %ld %ld %ld %ld\n", lround(bottom_left[0]), 
            lround(bottom_left[1]), lround(thruster_intersect[0]), lround(thruster_intersect[1])); 
    fprintf(executable, "drawSegment %ld %ld %ld %ld\n", lround(bottom_right[0]), 
            lround(bottom_right[1]), lround(thruster_intersect[0]), lround(thruster_intersect[1]));

    /*
    fprintf(executable, "drawSegment %ld %ld %ld %ld\n", lround(310), 
            lround(5), lround(330), lround(5)); 
    fprintf(executable, "drawSegment %ld %ld %ld %ld\n", lround(300), 
            lround(40), lround(340), lround(40));
    fprintf(executable, "drawSegment %ld %ld %ld %ld\n", lround(310), 
            lround(5), lround(300), lround(40));
    fprintf(executable, "drawSegment %ld %ld %ld %ld\n", lround(330), 
            lround(5), lround(340), lround(40));
    fprintf(executable, "drawSegment %ld %ld %ld %ld\n", lround(300), 
            lround(40), lround(320), lround(48)); 
    fprintf(executable, "drawSegment %ld %ld %ld %ld\n", lround(340), 
            lround(40), lround(320), lround(48));   
    */
    
    //close the input file
    if (landscapeFileOpen) {
        fclose(landscapeFile);
    }

    fprintf(executable, "%s\n", " pause 3");

    //close the pipe to Sketchpad and close the executable
    if (executableOpen) {
        //char* end = " end";
        fprintf(executable, "%s\n", " end");
        pclose(executable);
    }
    
    // must do this or else Terminal will be unusable
    // (if there are problems, it's not that big a deal though ... just
    // close the Terminal, and open a new one.)
    unset_curses();

    return(0);
    //exit(EXIT_SUCCESS);
}