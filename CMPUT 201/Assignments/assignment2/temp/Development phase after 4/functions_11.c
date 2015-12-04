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
int addImage(FILE *input, char line[], struct imageInfo **imagesAddress,
 int arraySize, int *lineNumberAddress, int *memoryAllocatedAddress) {
    arraySize++;
    if (arraySize == 1) {
        *imagesAddress = (struct imageInfo *) malloc(arraySize *
         sizeof(struct imageInfo));
        *memoryAllocatedAddress = 1;
    } else {
        *imagesAddress = (struct imageInfo *) realloc(*imagesAddress,
         arraySize*sizeof(struct imageInfo));
    }

    struct imageInfo *images = *imagesAddress;
    char newImage[LINE_LEN];
    double newX;
    double newY;

    sscanf(line, "%s%lg%lg", newImage, &newX, &newY);

    strncpy(images[arraySize-1].name, newImage, LINE_LEN);
    images[arraySize-1].totalPoints = 1;
    images[arraySize-1].x = (double *) malloc(images[arraySize-1].totalPoints * 
         sizeof(double));
    images[arraySize-1].y = (double *) malloc(images[arraySize-1].totalPoints * 
         sizeof(double));
    *(images[arraySize-1].x) = newX;
    *(images[arraySize-1].y) = newY;
    images[arraySize-1].currentX = newX;
    images[arraySize-1].currentY = newY;
    //images[arraySize-1].totalPoints = 1;
    
    //define a flag variable to indicate when we are done
    //processing the image from the input file
    int imageProcessed = 0;

    //process subsequent lines after the first line an
    //image definition until we get to the definition end
    char endImage[LINE_LEN];
    //char word[LINE_LEN];
    while (imageProcessed == 0) {
        char line[LINE_LEN];
        char word[LINE_LEN];
        memset(word, 0, LINE_LEN);
        fgets(line, LINE_LEN, input);
        sscanf(line, "%s", word);
        (*lineNumberAddress)++;
        if (strncmp(word, "End", strlen("End")) == 0) {
            sscanf(line, "End Image %s\n", endImage);
            imageProcessed = 1;
            if (images[arraySize-1].totalPoints == 1) {
                images[arraySize-1].isImageEmpty = 1;
                images[arraySize-1].totalPoints = 0;
                free(images[arraySize-1].x);
                free(images[arraySize-1].y);
            }
            else {
                images[arraySize-1].isImageEmpty = 0;
            }
        } else if (strncmp(word, "lineTo", strlen("lineTo")) == 0) {
              char dummy[LINE_LEN];
              sscanf(line, "%s%lg%lg", dummy, &newX, &newY);
              images[arraySize-1].currentX += newX;
              images[arraySize-1].currentY += newY;
              /* 
              images[arraySize-1].x[images[arraySize-1].totalPoints] =
               images[arraySize-1].currentX;
              images[arraySize-1].y[images[arraySize-1].totalPoints] =
               images[arraySize-1].currentY;
              */
              images[arraySize-1].totalPoints++;
              images[arraySize-1].x = (double *) realloc(images[arraySize-1].x, images[arraySize-1].totalPoints * 
                sizeof(double));
              images[arraySize-1].y = (double *) realloc(images[arraySize-1].y, images[arraySize-1].totalPoints * 
                sizeof(double));
              images[arraySize-1].x[images[arraySize-1].totalPoints-1] = images[arraySize-1].currentX;
              images[arraySize-1].y[images[arraySize-1].totalPoints-1] = images[arraySize-1].currentY;
        } else if (strncmp(word, "#", strlen("#")) == 0) {
            comment(line);
        } else if (isspace(*line)) {
            //printf("fffffffffffffffffff\n");
            continue;
        } else {
            arraySize--;
            return arraySize;
        }
    }
    //if the starting and ending image name are not the same,
    //then do not record it in the struct imageInfo array
    if (strncmp(newImage, endImage, strlen(newImage)) != 0) {
        free(images[arraySize-1].x);
        free(images[arraySize-1].y);
        arraySize--;
        *imagesAddress = (struct imageInfo *) realloc(*imagesAddress,
         arraySize*sizeof(struct imageInfo));
    }
    return arraySize;
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
int draw(char line[], struct imageInfo *images, FILE* executable,
 int arraySize) {
    char imageName[LINE_LEN];
    sscanf(line, "%s", imageName);

    int desiredIndex = desiredImage(imageName, images, arraySize);
    
    if (desiredIndex != -1) {
        if (images[desiredIndex].isImageEmpty != 1) {
            for (int i = 0; i < images[desiredIndex].totalPoints-1; i++) {
                double x1 = images[desiredIndex].x[i];
                double y1 = images[desiredIndex].y[i];
                double x2 = images[desiredIndex].x[i+1];
                double y2 = images[desiredIndex].y[i+1];
                fprintf(executable, "drawSegment %ld %ld %ld %ld\n",
                 lround(x1), lround(y1), lround(x2), lround(y2));
            }
        }
    }
    return desiredIndex;
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
int print(char line[], struct imageInfo *images, int arraySize) {
    char imageName[LINE_LEN];
    sscanf(line, "%s", imageName);

    int desiredIndex = desiredImage(imageName, images, arraySize);

    if (desiredIndex != -1) {
        printf("Print Image %s =\n", imageName);
        if (images[desiredIndex].isImageEmpty != 1) {
            for (int i = 0; i < images[desiredIndex].totalPoints; i++)
                printf("%g %g\n", images[desiredIndex].x[i],
                 images[desiredIndex].y[i]);
        printf("End Image %s\n", imageName);
        }
    }
    return desiredIndex;
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
int translate(char line[], struct imageInfo *images, int arraySize) {
    char imageName[LINE_LEN];
    double deltaX;
    double deltaY;
    sscanf(line, "%s%lg%lg", imageName, &deltaX, &deltaY);

    int desiredIndex = desiredImage(imageName, images, arraySize);

    if (desiredIndex != -1) {
        if (images[desiredIndex].isImageEmpty != 1) {
            for (int i = 0; i < images[desiredIndex].totalPoints; i++) {
                images[desiredIndex].x[i] += deltaX;
                images[desiredIndex].y[i] += deltaY;
            }
        images[desiredIndex].currentX += deltaX;
        images[desiredIndex].currentY += deltaY;
        }
    }
    return desiredIndex;
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

int rotate(char line[], struct imageInfo *images, int arraySize) {
    char imageName[LINE_LEN];
    double angle_degrees;
    sscanf(line, "%s%lg", imageName, &angle_degrees);
    
    //convert the input angle in degrees to an angle in
    //radians
    double angle_radians = angle_degrees * PI / 180.0;

    int desiredIndex = desiredImage(imageName, images, arraySize);

    if (desiredIndex != -1) {
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
    return desiredIndex;
}

int desiredImage(char imageName[], struct imageInfo *images, int arraySize) {
    int desiredIndex = -1;
    for (int i = 0; i < arraySize; i++) {
        if (strncmp(imageName, images[i].name, LINE_LEN) == 0)
            desiredIndex = i;
    }
    return desiredIndex;
}

void errorPrint(int lineNumber, int inputOpen, FILE *input,
 int executableOpen, FILE *executable, int memoryAllocated,
  struct imageInfo *images, int arraySize) {
    fprintf(stderr, "draw2: %d, error.\n", lineNumber);
    if (inputOpen)
        fclose(input);
    if (executableOpen) {
        char* end = " end";
        fprintf(executable, "%s\n", end);
        pclose(executable);
    }
    if (memoryAllocated) {
        for (int i = 0; i < arraySize; i++) {
            free(images[i].x);
            free(images[i].y);
        }
        free(images);
    }
    exit(EXIT_FAILURE);
}