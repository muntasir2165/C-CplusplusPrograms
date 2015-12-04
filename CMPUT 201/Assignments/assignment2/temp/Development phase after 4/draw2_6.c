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
#include <ctype.h> //for isspace()...

const char exec_name[] = "java -jar Sketchpad.jar";

int main(int argc, char * argv[]) {
    //declare FILE pointers for the input and
    //executable files
    FILE* input;
    FILE* executable;

    //test for memwatch
    //char* aByte = malloc(1);

    //create an array of strings to correspond
    //to the six types of input lines
    char *keywords[] = {"#", "Image", "draw", 
    "print", "translate", "child", "rotate"};

    //determine the size of the keywords array
    int totalKeywords = sizeof(keywords)/sizeof(keywords[0]);

    //variables to store image array size, image information
    //and the current line number in the input file
    int arraySize = 0;
    struct imageInfo *images;
    int lineNumber = 0;

    //check if we have the right number of command line
    //arguments
    if (argc < NUM_ARGS) {
        errorPrint(lineNumber);
    }

    //open the input file for reading
    input = fopen(argv[1], "r");
    if (input == NULL) {
        errorPrint(lineNumber);
    }

    //open the sketchpad executable to be run by the child
    //process
    executable = popen(exec_name, "w");
    if (executable == NULL) {
        errorPrint(lineNumber);
    }

    //print a start-up message stating how and when
    //this program is executed and the name of the
    //input file
    printf("%s started on ", argv[0]); 
    fflush(stdout);
    system("date");
    printf("Input file: %s\n", argv[1]); 
    
    //process the input file line by line
    while(!feof(input)) {
            char line[LINE_LEN];
            char word[LINE_LEN];
            fgets(line, LINE_LEN, input);
            sscanf(line, "%s", word);
            lineNumber++;

            //lineType variable will be used to delegate line
            //processing operations to the relevant functions
            int lineType = -2; //-1 is the default value
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
            //temporary variables used to determine the validity
            //of input lines
            int tokens, output, newArraySize;
            char imageName[LINE_LEN];
            double angle, translateX, translateY;
            char* end = " end";
            switch (lineType){
                case -1:
                    break;
                case 1:
                    comment(line);
                    break;
                case 2:
                    newArraySize = addImage(input, line+strlen("Image "), &images, arraySize, &lineNumber);
                    if (arraySize != newArraySize) {
                        arraySize = newArraySize;
                    } else {
                        fclose(input);
                        if (executable != NULL)
                            fprintf(executable, "%s\n", end);
                        pclose(executable);
                        if (images != NULL)
                            free(images);
                        errorPrint(lineNumber);
                    }
                    break;
                case 3:
                    //char imageName[LINE_LEN];
                    tokens = sscanf(line, "draw%s", imageName);
                    if (tokens != 1) {
                        fclose(input);
                        if (executable != NULL)
                            fprintf(executable, "%s\n", end);
                        pclose(executable);
                        if (images != NULL)
                            free(images);
                        errorPrint(lineNumber);
                    } else {
                        output = draw(line+strlen("draw "), images, executable, arraySize);
                        if (output == -1) {
                            fclose(input);
                            if (executable != NULL)
                                fprintf(executable, "%s\n", end);
                            pclose(executable);
                            if (images != NULL)
                                free(images);
                            errorPrint(lineNumber);
                        }
                    }
                    break;
                case 4:
                    tokens = sscanf(line, "print%s", imageName);
                    if (tokens != 1) {
                        fclose(input);
                        if (executable != NULL)
                            fprintf(executable, "%s\n", end);
                        pclose(executable);
                        if (images != NULL)
                            free(images);
                        errorPrint(lineNumber);
                    } else {
                        output = print(line+strlen("print "), images, arraySize);
                        if (output == -1) {
                            fclose(input);
                            if (executable != NULL)
                                fprintf(executable, "%s\n", end);
                            pclose(executable);
                            if (images != NULL)
                                free(images);
                            errorPrint(lineNumber);
                        }
                    }
                    break;
                case 5:
                tokens = sscanf(line, "translate%s%lg%lg", imageName, &translateX, &translateY);
                    if (tokens != 3) {
                        fclose(input);
                        if (executable != NULL)
                            fprintf(executable, "%s\n", end);
                        pclose(executable);
                        if (images != NULL)
                            free(images);
                        errorPrint(lineNumber);
                    } else {
                        output = translate(line+strlen("translate "), images, arraySize);
                        if (output == -1) {
                            fclose(input);
                            if (executable != NULL)
                                fprintf(executable, "%s\n", end);
                            pclose(executable);
                            if (images != NULL)
                                free(images);
                            errorPrint(lineNumber);
                        }
                    }
                    break;
                case 6:
                    child(line, executable); 
                    break;
                case 7:
                    //double angle = 0.0;
                    tokens = sscanf(line, "rotate%s%lg", imageName, &angle);
                    if (tokens != 2) {
                        fclose(input);
                        if (executable != NULL)
                            fprintf(executable, "%s\n", end);
                        pclose(executable);
                        if (images != NULL)
                            free(images);
                        errorPrint(lineNumber);
                    } else {
                    output = rotate(line+strlen("rotate "), images, arraySize);
                    if (output == -1) {
                            fclose(input);
                            if (executable != NULL)
                                fprintf(executable, "%s\n", end);
                            pclose(executable);
                            if (images != NULL)
                                free(images);
                            errorPrint(lineNumber);
                        }
                    }
                    break;
                default:
                    fclose(input);
                    if (executable != NULL)
                        fprintf(executable, "%s\n", end);
                    pclose(executable);
                    if (images != NULL)
                        free(images);
                    errorPrint(lineNumber);
                    break;
            }
    }

    //close the input file
    fclose(input);

    //close the executable
    pclose(executable);

    //free the allocated memory
    if (images != NULL)
        free(images);
    
    return(0);
}

// /*
//     Print a comment to stdout
// Input: 
//     line - a character array (string) that
//     is a comment
// Output:
//     None. However, as a side effect,
//     this function takes the comment 
//     in the line and prints it to stdout
// */
// void comment(char line[]) {
//     printf("%s", line);
// }

// /*
//     Add an image and its relevant information
//     to the imageInfo images array in the main
//     function
// Input:
//     input - the input file pointer to gather
//     further information about the image for
//     later storage 
//     line - a character array (string) with the
//     image name and starting point x- and y-
//     coordinates
//     images - an array containing elements of
//     type struct imageInfo where we will store
//     all relevant information about image in
//     question
//     currentIndex - the current index in the
//     images array to store the image information
// Output:
//     None. However, as a side effect,
//     this function stores all necessary
//     information about an image in a struct
//     imageInfo array
// */
// int addImage(FILE *input, char line[], struct imageInfo **imagesAddress, int arraySize, int *lineNumberAddress) {
//     arraySize++;
//     if (arraySize == 1) {
//         *imagesAddress = (struct imageInfo *) malloc(arraySize*sizeof(struct imageInfo));
//     } else {
//         *imagesAddress = (struct imageInfo *) realloc(*imagesAddress, arraySize*sizeof(struct imageInfo));
//     }

//     struct imageInfo *images = *imagesAddress;
//     char newImage[LINE_LEN];
//     double newX;
//     double newY;

//     sscanf(line, "%s%lg%lg", newImage, &newX, &newY);

//     strncpy(images[arraySize-1].name, newImage, LINE_LEN);
//     images[arraySize-1].x[0] = newX;
//     images[arraySize-1].y[0] = newY;
//     images[arraySize-1].currentX = newX;
//     images[arraySize-1].currentY = newY;
//     images[arraySize-1].totalPoints = 1;
    
//     //define a flag variable to indicate when we are done
//     //processing the image from the input file
//     int imageProcessed = 0;

//     //process subsequent lines after the first line an
//     //image definition until we get to the definition end
//     char endImage[LINE_LEN];
//     while (imageProcessed == 0) {
//         char word[LINE_LEN];
//         fgets(line, LINE_LEN, input);
//         sscanf(line, "%s", word);
//         (*lineNumberAddress)++;
//         //printf("sdsdsdsd::::%s\n", line);
//         if (strncmp(word, "End", strlen("End")) == 0) {
//             sscanf(line, "End Image %s\n", endImage);
//             //printf("Ending Image:: %s\n", endImage);
//             imageProcessed = 1;
//             if (images[arraySize-1].totalPoints == 1) {
//                 images[arraySize-1].isImageEmpty = 1;
//                 images[arraySize-1].totalPoints = 0;
//             }
//             else {
//                 images[arraySize-1].isImageEmpty = 0;
//             }
//         } else if (strncmp(word, "lineTo", strlen("lineTo")) == 0) {
//               char dummy[LINE_LEN];
//               sscanf(line, "%s%lg%lg", dummy, &newX, &newY);
//               images[arraySize-1].currentX += newX;
//               images[arraySize-1].currentY += newY; 
//               images[arraySize-1].x[images[arraySize-1].totalPoints] = images[arraySize-1].currentX;
//               images[arraySize-1].y[images[arraySize-1].totalPoints] = images[arraySize-1].currentY;
//               images[arraySize-1].totalPoints++;
//         } else if (strncmp(word, "#", strlen("#")) == 0) {
//             comment(line);
//         } else if (isspace(*line)) {
//             continue;
//         } else {
//             //printf("sdsdsdsd::::%s\n", line);
//             arraySize--;
//             return arraySize;
//         }
//     }
//     //if the starting and ending image name are not the same,
//     //then do not record it in the struct imageInfo array
//     if (strncmp(newImage, endImage, strlen(newImage)) != 0) {
//         arraySize--;
//     }
//     return arraySize;
// }

// /*
//     Draw an image on Sketchpad
// Input:
//     line - a character array (string) with the
//     image name to be drawn on Sketchpad
//     images - an array containing elements of
//     type struct imageInfo that holds all
//     relevant information about the image to be
//     drawn on Sketchpad
//     executable - the child process running the
//     Sketchpad program
// Output:
//     None. However, as a side effect,
//     this function sends the image point
//     coordinate information to Sketchpad via a
//     child process such that all line segments
//     connecting the points are drawn and presented
//     in a new window
// */
// int draw(char line[], struct imageInfo *images, FILE* executable, int arraySize) {
//     char imageName[LINE_LEN];
//     sscanf(line, "%s", imageName);

//     int desiredIndex = desiredImage(imageName, images, arraySize);
    
//     if (desiredIndex != -1) {
//         if (images[desiredIndex].isImageEmpty != 1) {
//             for (int i = 0; i < images[desiredIndex].totalPoints-1; i++) {
//                 double x1 = images[desiredIndex].x[i];
//                 double y1 = images[desiredIndex].y[i];
//                 double x2 = images[desiredIndex].x[i+1];
//                 double y2 = images[desiredIndex].y[i+1];
//                 fprintf(executable, "drawSegment %ld %ld %ld %ld\n", lround(x1), lround(y1), lround(x2), lround(y2));
//             }
//         }
//     }
//     return desiredIndex;
// }

// /*
//     Print image information to stdout
// Input:
//     line - a character array (string) with the
//     image name whose information we have to
//     print
//     images - an array containing elements of
//     type struct imageInfo that holds all
//     relevant information about the image to
//     be printed
// Output:
//     None. However, as a side effect,
//     this function prints all the point (x, y)
//     coordinate information of the image in
//     question to stdout
// */
// int print(char line[], struct imageInfo *images, int arraySize) {
//     char imageName[LINE_LEN];
//     sscanf(line, "%s", imageName);

//     int desiredIndex = desiredImage(imageName, images, arraySize);

//     if (desiredIndex != -1) {
//         printf("Print Image %s =\n", imageName);
//         if (images[desiredIndex].isImageEmpty != 1) {
//             for (int i = 0; i < images[desiredIndex].totalPoints; i++)
//                 printf("%g %g\n", images[desiredIndex].x[i], images[desiredIndex].y[i]);
//         printf("End Image %s\n", imageName);
//         }
//     }
//     return desiredIndex;
// }

// /*
//     Translate all the points corresponding to
//     an image by some specified amount in the x-
//     and y-axes
// Input:
//     line - a character array (string) with the
//     image name and the translation amounts in
//     the x- and y-axes
//     images - an array containing elements of
//     type struct imageInfo that holds all
//     relevant coordinate information about the
//     image to be translated
// Output:
//     None. However, as a side effect,
//     this function translates all the point
//     coordinates of the image in question by
//     some specified amount
// */
// int translate(char line[], struct imageInfo *images, int arraySize) {
//     char imageName[LINE_LEN];
//     double deltaX;
//     double deltaY;
//     sscanf(line, "%s%lg%lg", imageName, &deltaX, &deltaY);

//     int desiredIndex = desiredImage(imageName, images, arraySize);

//     if (desiredIndex != -1) {
//         if (images[desiredIndex].isImageEmpty != 1) {
//             for (int i = 0; i < images[desiredIndex].totalPoints; i++) {
//                 images[desiredIndex].x[i] += deltaX;
//                 images[desiredIndex].y[i] += deltaY;
//             }
//         images[desiredIndex].currentX += deltaX;
//         images[desiredIndex].currentY += deltaY;
//         }
//     }
//     return desiredIndex;
// }

// /*
//     Send commands to Sketchpad
// Input:
//     line - a character array (string) with the
//     child command to be sent to Sketchpad
//     executable - the child process running the
//     Sketchpad program
// Output:
//     None. However, as a side effect,
//     this function processes the child command and
//     sends it to Sketchpad via the child process
//     for execution
// */
// void child(char line[], FILE* executable) {
//     fprintf(executable, "%s\n", line+strlen("child"));
// }

// int rotate(char line[], struct imageInfo *images, int arraySize) {
//     char imageName[LINE_LEN];
//     double angle_degrees;
//     sscanf(line, "%s%lg", imageName, &angle_degrees);
    
//     //convert the input angle in degrees to an angle in
//     //radians
//     double angle_radians = angle_degrees * PI / 180.0;

//     int desiredIndex = desiredImage(imageName, images, arraySize);

//     if (desiredIndex != -1) {
//         //translate the (x,y) coordinates of all the points
//         //to rotate the overall image
//         double x, y, rotated_x, rotated_y;
//         if (images[desiredIndex].isImageEmpty != 1) {
//             for (int i = 0; i < images[desiredIndex].totalPoints; i++) {
//                 x = images[desiredIndex].x[i];
//                 y = images[desiredIndex].y[i];
//                 rotated_x = x * cos(angle_radians) - y * sin(angle_radians);
//                 rotated_y = x * sin(angle_radians) + y * cos(angle_radians);
//                 images[desiredIndex].x[i] = rotated_x;
//                 images[desiredIndex].y[i] = rotated_y;
//                 }
//         }
//         x = images[desiredIndex].currentX;
//         y = images[desiredIndex].currentY;
//         rotated_x = x * cos(angle_radians) - y * sin(angle_radians);
//         rotated_y = x * sin(angle_radians) + y * cos(angle_radians);
//         images[desiredIndex].currentX = rotated_x;
//         images[desiredIndex].currentY = rotated_y;
//     }
//     return desiredIndex;
// }

// int desiredImage(char imageName[], struct imageInfo *images, int arraySize) {
//     int desiredIndex = -1;
//     for (int i = 0; i < arraySize; i++) {
//         if (strncmp(imageName, images[i].name, LINE_LEN) == 0)
//             desiredIndex = i;
//     }
//     return desiredIndex;
// }

// void errorPrint(int lineNumber) {
//     fprintf(stderr, "draw2: %d, error.\n", lineNumber);
//     exit(EXIT_FAILURE);
// }
