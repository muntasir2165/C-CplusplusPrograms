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

#include "draw2.h"

int main(int argc, char * argv[]) {
    //declare FILE pointers for the input and executable files
    FILE* input = NULL;
    FILE* executable = NULL;

    //binary variables that act as flags indicating the status of open files,
    //executables and allocated memory
    int inputOpen = 0, executableOpen = 0, memoryAllocated = 0;

    //create a file called sketchpad.out based on the  commands from the
    //input file
    //const char exec_name[] = "java -jar Sketchpad.jar -d"; 

    //create a new graphical window and draws images (or more precisely, line
    //segments) based on the input file
    const char exec_name[] = "java -jar Sketchpad.jar";

    //create an array of strings to correspond to the six types of input lines
    char *keywords[] = {"#", "Image", "draw",  "print", "translate", "child",
     "rotate"};

    //determine the size of the keywords array
    int totalKeywords = sizeof(keywords)/sizeof(keywords[0]);

    //variables to store image array size, image information and the current
    //line number in the input file
    int arraySize = 0;
    struct imageInfo *images = NULL;
    int lineNumber = 0;

    //check if we have the right number of command line arguments
    if (argc < NUM_ARGS || argc > NUM_ARGS) {
        errorPrint(lineNumber, inputOpen, input, executableOpen, executable,
         memoryAllocated, images, arraySize);
    }

    //open the input file for reading
    input = fopen(argv[1], "r");
    if (input == NULL) {
        errorPrint(lineNumber, inputOpen, input, executableOpen, executable,
         memoryAllocated, images, arraySize);
    } else {
        inputOpen = 1;
    }

    //open the sketchpad executable to be run by the child process
    executable = popen(exec_name, "w");
    if (executable == NULL) {
        errorPrint(lineNumber, inputOpen, input, executableOpen, executable,
         memoryAllocated, images, arraySize);
    } else {
        executableOpen = 1;
    }

    //print a start-up message stating how and when this program is executed
    //and the name of the input file
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

            //lineType variable will be used to delegate line processing
            //operations to the relevant functions
            int lineType = -2; //Note: -1 is the default value
            //check if the word matches any of the keywords and outsource the
            //line processing job to the appropriate function
            for (int i = 0; i < totalKeywords; i++) {
                if (strncmp(word, keywords[i], LINE_LEN) == 0) {
                    lineType = i+1;
                    break;
                }
            }
            
            //check if the line is empty/contains only whitespace characters
            if (isspace(*line)) {
                lineType = -1;
            }

            //temporary variables used to determine the validity of input
            //lines
            int tokens, output, newArraySize;
            char imageName[LINE_LEN];
            double angle, translateX, translateY;
            switch (lineType){
                case -1:
                    break;
                case 1:
                    comment(line);
                    break;
                case 2:
                    newArraySize = addImage(input, line+strlen("Image "),
                     &images, arraySize, &lineNumber, &memoryAllocated);
                    if (arraySize != newArraySize) {
                        arraySize = newArraySize;
                    } else {
                        errorPrint(lineNumber, inputOpen, input,
                         executableOpen, executable, memoryAllocated, images,
                         arraySize);
                    }
                    break;
                case 3:
                    tokens = sscanf(line, "draw%s", imageName);
                    if (tokens != 1) {
                        errorPrint(lineNumber, inputOpen, input,
                         executableOpen, executable, memoryAllocated, images,
                         arraySize);
                    } else {
                        output = draw(line+strlen("draw "), images,
                         executable, arraySize);
                        if (output == -1) {
                            errorPrint(lineNumber, inputOpen, input,
                             executableOpen, executable, memoryAllocated,
                             images, arraySize);
                        }
                    }
                    break;
                case 4:
                    tokens = sscanf(line, "print%s", imageName);
                    if (tokens != 1) {
                        errorPrint(lineNumber, inputOpen, input,
                         executableOpen, executable, memoryAllocated, images,
                         arraySize);
                    } else {
                        output = print(line+strlen("print "), images,
                         arraySize);
                        if (output == -1) {
                            errorPrint(lineNumber, inputOpen, input, 
                             executableOpen, executable, memoryAllocated,
                             images, arraySize);
                        }
                    }
                    break;
                case 5:
                tokens = sscanf(line, "translate%s%lg%lg", imageName,
                 &translateX, &translateY);
                    if (tokens != 3) {
                        errorPrint(lineNumber, inputOpen, input,
                         executableOpen, executable, memoryAllocated, images,
                          arraySize);
                    } else {
                        output = translate(line+strlen("translate "), images,
                         arraySize);
                        if (output == -1) {
                            errorPrint(lineNumber, inputOpen, input,
                             executableOpen, executable, memoryAllocated,
                             images, arraySize);
                        }
                    }
                    break;
                case 6:
                    child(line, executable); 
                    break;
                case 7:
                    tokens = sscanf(line, "rotate%s%lg", imageName, &angle);
                    if (tokens != 2) {
                        errorPrint(lineNumber, inputOpen, input,
                         executableOpen, executable, memoryAllocated, images,
                         arraySize);
                    } else {
                        output = rotate(line+strlen("rotate "), images,
                         arraySize);
                        if (output == -1) {
                            errorPrint(lineNumber, inputOpen, input,
                             executableOpen, executable, memoryAllocated,
                             images, arraySize);
                        }
                    }
                    break;
                default:
                    errorPrint(lineNumber, inputOpen, input, executableOpen,
                     executable, memoryAllocated, images, arraySize);
                    break;
            }
    }

    //close the input file
    fclose(input);

    //close the executable
    pclose(executable);

    //free the allocated memory for images and the (x,y) coordinates
    if (images != NULL) {
        for (int i = 0; i < arraySize; i++) {
            free(images[i].x);
            free(images[i].y);
        }
        free(images);
    }
    
    return(0);
}