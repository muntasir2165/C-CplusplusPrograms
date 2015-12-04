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

#include <stdio.h> //for sscanf(), fprintf(), printf()...
#include <stdlib.h> //for exit()...
#include <math.h> //for lround(), sin(), cos()...
#include <string.h> //for strtok()...
#include "draw2.h"
#include <ctype.h>

int main(int argc, char * argv[]) {
    //declare FILE pointers for the input and
    //executable files
    FILE* input;
    FILE* executable;
    
    //test for memwatch
    //char* aByte = malloc(1);
    
    //lineType variable will be used to delegate line
    //processing operations to the relevant functions
    int lineType = -1; //-1 is the default value

    //create an array of strings to correspond
    //to the six types of input lines
    char *keywords[] = {"#", "Image", "draw", 
    "print", "translate", "child", "rotate"};

    //determine the size of the keywords array
    int totalKeywords = sizeof(keywords)/sizeof(keywords[0]);

    //array variables to store image information
    struct imageInfo images[5*LINE_NUM];
    int currentIndex = 0;

    //check if we have the right number of command line
    //arguments
    if (argc < NUM_ARGS) {
        printf("Not enough arguments\n");
        exit(-1);
    }

    //print a start-up message stating how and when
    //this program is executed and the name of the
    //input file
    printf("%s started on ", argv[0]); 
    fflush(stdout);
    system("date");
    printf("Input file: %s\n", argv[1]); 

    //open the input file for reading
    input = fopen(argv[1], "r");
    if (input == NULL) {
        printf("Could not open input file %s\n", argv[1]);
        exit(-1);
    }

    //open the sketchpad executable to be run by the child
    //process
    executable = popen(exec_name, "w");
    if (executable == NULL) {
        printf("Could not open pipe %s\n", exec_name);
        exit(-1);
    }
    
    //process the input file line by line
    while(!feof(input)) {
            char line[LINE_LEN];
            char word[LINE_LEN];
            fgets(line, LINE_LEN, input);
            sscanf(line, "%s", word);
            //check if the word matches any of the keywords
            //and outsource the line processing job to the
            //appropriate function
            for (int i = 0; i < totalKeywords; i++) {
                if (strncmp(word, keywords[i], LINE_LEN) == 0) {
                    lineType = i+1;
                    break;
                }
            }
            
            //check if the line is empty/contains only whitespace
            //characters
            if (isspace(*line)) {
                lineType = -1;
            }

            switch (lineType){
                case 1:
                    comment(line);
                    break;
                case 2:
                    addImage(input, line+strlen("Image "), images, currentIndex);
                    currentIndex++;
                    break;
                case 3:
                    draw(line+strlen("draw "), images, executable);
                    break;
                case 4:
                    print(line+strlen("print "), images);
                    break;
                case 5:
                    translate(line+strlen("translate "), images);
                    break;
                case 6:
                    child(line, executable); 
                    break;
                case 7:
                    rotate(line+strlen("rotate "), images);
                    break;
                default:
                    //printf("Invalid line type\n");
                    break;
            }
    }

    //close the input file
    fclose(input);

    //close the executable
    pclose(executable);
    
    return(0);
}

/*
    Print a comment to stdout
Input: 
    line - a character array (string) that
    is a comment
Output:
    None. However, as a side effect,
    this function takes the comment 
    in the line and prints it to stdout
*/
void comment(char line[]) {
    printf("%s", line);
}

/*
    Add an image and its relevant information
    to the imageInfo images array in the main
    function
Input:
    input - the input file pointer to gather
    further information about the image for
    later storage 
    line - a character array (string) with the
    image name and starting point x- and y-
    coordinates
    images - an array containing elements of
    type struct imageInfo where we will store
    all relevant information about image in
    question
    currentIndex - the current index in the
    images array to store the image information
Output:
    None. However, as a side effect,
    this function stores all necessary
    information about an image in a struct
    imageInfo array
*/
void addImage(FILE *input, char line[], struct imageInfo images[], int currentIndex) {
    char newImage[LINE_LEN];
    double newX;
    double newY;

    sscanf(line, "%s%lg%lg", newImage, &newX, &newY);

    strncpy(images[currentIndex].name, newImage, LINE_LEN);
    images[currentIndex].x[0] = newX;
    images[currentIndex].y[0] = newY;
    images[currentIndex].currentX = newX;
    images[currentIndex].currentY = newY;
    images[currentIndex].totalPoints = 1;
    
    //define a flag variable to indicate when we are done
    //processing the image from the input file
    int imageProcessed = 0;

    //process subsequent lines after the first line an
    //image definition until we get to the definition end
    while (imageProcessed == 0) {
        char word[LINE_LEN];
        fgets(line, LINE_LEN, input);
        sscanf(line, "%s", word);

        if (strncmp(word, "End", strlen("End")) == 0) {
            imageProcessed = 1;
            if (images[currentIndex].totalPoints == 1) {
                images[currentIndex].isImageEmpty = 1;
                images[currentIndex].totalPoints = 0;
            }
            else {
                images[currentIndex].isImageEmpty = 0;
            }
        } else if (strncmp(word, "lineTo", strlen("lineTo")) == 0) {
              char dummy[LINE_LEN];
              sscanf(line, "%s%lg%lg", dummy, &newX, &newY);
              images[currentIndex].currentX += newX;
              images[currentIndex].currentY += newY; 
              images[currentIndex].x[images[currentIndex].totalPoints] = images[currentIndex].currentX;
              images[currentIndex].y[images[currentIndex].totalPoints] = images[currentIndex].currentY;
              images[currentIndex].totalPoints++;
        } else if (strncmp(word, "#", strlen("#")) == 0) {
            comment(line);
        }
    }
}

/*
    Draw an image on Sketchpad
Input:
    line - a character array (string) with the
    image name to be drawn on Sketchpad
    images - an array containing elements of
    type struct imageInfo that holds all
    relevant information about the image to be
    drawn on Sketchpad
    executable - the child process running the
    Sketchpad program
Output:
    None. However, as a side effect,
    this function sends the image point
    coordinate information to Sketchpad via a
    child process such that all line segments
    connecting the points are drawn and presented
    in a new window
*/
void draw(char line[], struct imageInfo images[], FILE* executable) {
    char imageName[LINE_LEN];
    sscanf(line, "%s", imageName);

    int desiredIndex = desiredImage(imageName, images);
    
    if (images[desiredIndex].isImageEmpty != 1) {
        for (int i = 0; i < images[desiredIndex].totalPoints-1; i++) {
            double x1 = images[desiredIndex].x[i];
            double y1 = images[desiredIndex].y[i];
            double x2 = images[desiredIndex].x[i+1];
            double y2 = images[desiredIndex].y[i+1];
            fprintf(executable, "drawSegment %ld %ld %ld %ld\n", lround(x1), lround(y1), lround(x2), lround(y2));
        }
    }
}

/*
    Print image information to stdout
Input:
    line - a character array (string) with the
    image name whose information we have to
    print
    images - an array containing elements of
    type struct imageInfo that holds all
    relevant information about the image to
    be printed
Output:
    None. However, as a side effect,
    this function prints all the point (x, y)
    coordinate information of the image in
    question to stdout
*/
void print(char line[], struct imageInfo images[]) {
    char imageName[LINE_LEN];
    sscanf(line, "%s", imageName);

    int desiredIndex = desiredImage(imageName, images);

    printf("Print Image %s =\n", imageName);
    if (images[desiredIndex].isImageEmpty != 1) {
        for (int i = 0; i < images[desiredIndex].totalPoints; i++)
            printf("%g %g\n", images[desiredIndex].x[i], images[desiredIndex].y[i]);
    }
    printf("End Image %s\n", imageName);
}

/*
    Translate all the points corresponding to
    an image by some specified amount in the x-
    and y-axes
Input:
    line - a character array (string) with the
    image name and the translation amounts in
    the x- and y-axes
    images - an array containing elements of
    type struct imageInfo that holds all
    relevant coordinate information about the
    image to be translated
Output:
    None. However, as a side effect,
    this function translates all the point
    coordinates of the image in question by
    some specified amount
*/
void translate(char line[], struct imageInfo images[]) {
    char imageName[LINE_LEN];
    double deltaX;
    double deltaY;
    sscanf(line, "%s%lg%lg", imageName, &deltaX, &deltaY);

    int desiredIndex = desiredImage(imageName, images);

    if (images[desiredIndex].isImageEmpty != 1) {
        for (int i = 0; i < images[desiredIndex].totalPoints; i++) {
            images[desiredIndex].x[i] += deltaX;
            images[desiredIndex].y[i] += deltaY;
        }
    images[desiredIndex].currentX += deltaX;
    images[desiredIndex].currentY += deltaY;
    }
}

/*
    Send commands to Sketchpad
Input:
    line - a character array (string) with the
    child command to be sent to Sketchpad
    executable - the child process running the
    Sketchpad program
Output:
    None. However, as a side effect,
    this function processes the child command and
    sends it to Sketchpad via the child process
    for execution
*/
void child(char line[], FILE* executable) {
    fprintf(executable, "%s\n", line+strlen("child"));
}

void rotate(char line[], struct imageInfo images[]) {
    char imageName[LINE_LEN];
    double angle_degrees;
    sscanf(line, "%s%lg", imageName, &angle_degrees);
    //convert the input angle in degrees to an angle in
    //radians
    double angle_radians = angle_degrees * PI / 180.0;

    int desiredIndex = desiredImage(imageName, images);

    //translate the (x,y) coordinates of all the points
    //to rotate the overall image
    double x, y, rotated_x, rotated_y;
    if (images[desiredIndex].isImageEmpty != 1) {
        for (int i = 0; i < images[desiredIndex].totalPoints; i++) {
            x = images[desiredIndex].x[i];
            y = images[desiredIndex].y[i];
            rotated_x = x * cos(angle_radians) - y * sin(angle_radians);
            rotated_y = x * sin(angle_radians) + y * cos(angle_radians);
            images[desiredIndex].x[i] = rotated_x;
            images[desiredIndex].y[i] = rotated_y;
            }
    }
    x = images[desiredIndex].currentX;
    y = images[desiredIndex].currentY;
    rotated_x = x * cos(angle_radians) - y * sin(angle_radians);
    rotated_y = x * sin(angle_radians) + y * cos(angle_radians);
    images[desiredIndex].currentX = rotated_x;
    images[desiredIndex].currentY = rotated_y;
}

int desiredImage(char imageName[], struct imageInfo images[]) {
    int desiredIndex = -1;
    for (int i = 0; i < 5*LINE_NUM; i++) {
        if (strncmp(imageName, images[i].name, LINE_LEN) == 0)
            desiredIndex = i;
    }
    return desiredIndex;
}

