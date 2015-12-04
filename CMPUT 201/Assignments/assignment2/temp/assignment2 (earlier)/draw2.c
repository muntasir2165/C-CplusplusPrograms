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
#include "functions.c"

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