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

void setup_curses()
{
  // use return values.  see man pages.  likely just useful for error
  // checking (NULL or non-NULL, at least for init_scr)
  initscr();
  cbreak();
  noecho();
  // needed for cursor keys (even though says keypad)
  keypad(stdscr, true);
}

void unset_curses()
{
  keypad(stdscr, false);
  nodelay(stdscr, false);
  nocbreak();
  echo();
  endwin();
}

// /*
//     Add an image and its relevant information to the struct imageInfo images
//      array in the main function
// Input:
//     input - the input file pointer to gather further information about the
//      image for later storage 
//     line - a character array (string) with the image name and starting point
//      x- and y-coordinates
//     imagesAddress - a pointer to pointer for the dynamic images array
//      containing elements of type struct imageInfo where we will store all
//      relevant information about the image in question
//     arraySize - the current size of the dynamic array images
//     lineNumberAddress - a pointer to the lineNumberAddress indicating the
//      current line in the input file being parsed
//     memoryAllocatedAddress - a pointer to a flag variable that would be set to
//      1 when we allocate memory for the images dynamic array for the first
//      time. This variable will be used later in the errorPrint() function to
//      decide if the corresponding memory should be freed
// Output:
//     arraySize - the updated size of the images array which will be used by the
//      main function to assess whether an an invalid input was encountered
//     Nevertheless, as a side effect, this function stores all necessary
//      information about an image in a struct imageInfo array
// */
// int addImage(FILE *input, char line[], struct imageInfo **imagesAddress,
//  int arraySize, int *lineNumberAddress, int *memoryAllocatedAddress) {
//     arraySize++;
//     //allocate/reallocate memory for the images array
//     if (arraySize == 1) {
//         *imagesAddress = (struct imageInfo *) malloc(arraySize *
//          sizeof(struct imageInfo));
//         *memoryAllocatedAddress = 1;
//     } else {
//         *imagesAddress = (struct imageInfo *) realloc(*imagesAddress,
//          arraySize*sizeof(struct imageInfo));
//     }

//     struct imageInfo *images = *imagesAddress;
//     char newImage[LINE_LEN];
//     double newX;
//     double newY;

//     sscanf(line, "%s%lg%lg", newImage, &newX, &newY);

//     strncpy(images[arraySize-1].name, newImage, LINE_LEN);
//     images[arraySize-1].totalPoints = 1;
//     images[arraySize-1].x = (double *) malloc(images[arraySize-1].totalPoints
//      * sizeof(double));
//     images[arraySize-1].y = (double *) malloc(images[arraySize-1].totalPoints
//      * sizeof(double));
//     *(images[arraySize-1].x) = newX;
//     *(images[arraySize-1].y) = newY;
//     images[arraySize-1].currentX = newX;
//     images[arraySize-1].currentY = newY;
    
//     //define a flag variable to indicate when we are done processing the image
//     //from the input file
//     int imageProcessed = 0;

//     //define a flag variable to indicate if an invalid token is encountered
//     //in the input file
//     int invalidInput = 0;

//     //process subsequent lines after the first line of
//     //image definition until we get to the definition end
//     char endImage[LINE_LEN];
//     char word[LINE_LEN];
//     while (imageProcessed == 0) {
//         char line[LINE_LEN];
//         memset(word, 0, LINE_LEN);
//         fgets(line, LINE_LEN, input);
//         sscanf(line, "%s", word);
//         (*lineNumberAddress)++;
//         if (strncmp(word, "End", strlen("End")) == 0) {
//             sscanf(line, "End Image %s\n", endImage);
//             imageProcessed = 1;
//             if (images[arraySize-1].totalPoints == 1) {
//                 images[arraySize-1].isImageEmpty = 1;
//                 images[arraySize-1].totalPoints = 0;
//                 free(images[arraySize-1].x);
//                 free(images[arraySize-1].y);
//             }
//             else {
//                 images[arraySize-1].isImageEmpty = 0;
//             }
//         } else if (strncmp(word, "lineTo", strlen("lineTo")) == 0) {
//               char dummy[LINE_LEN];
//               sscanf(line, "%s%lg%lg", dummy, &newX, &newY);
//               images[arraySize-1].currentX += newX;
//               images[arraySize-1].currentY += newY;
//               images[arraySize-1].totalPoints++;
//               images[arraySize-1].x = (double *)
//                realloc(images[arraySize-1].x, images[arraySize-1].totalPoints
//                * sizeof(double));
//               images[arraySize-1].y = (double *)
//                realloc(images[arraySize-1].y, images[arraySize-1].totalPoints
//                * sizeof(double));
//               images[arraySize-1].x[images[arraySize-1].totalPoints-1] =
//                 images[arraySize-1].currentX;
//               images[arraySize-1].y[images[arraySize-1].totalPoints-1] =
//                 images[arraySize-1].currentY;
//         } else if (strncmp(word, "#", strlen("#")) == 0) {
//             continue;
//         } else if (isspace(*line) && strlen(word) == 0) {
//             //if we encounter an empty/white spaced line inside the image
//             //definition, just ignore it
//             continue;
//         } else {
//             //if we encounter an invalid input line inside the image
//             //definiton, set the flag variable and break out of the while loop
//             invalidInput = 1;
//             break;
//         }
//     }
//     //if the starting and ending image name are not the same, then do not
//     //record it in the images array. Thus, free up the memory allocated
//     //earlier inside this function
//     if (strncmp(newImage, endImage, strlen(newImage)) != 0 || invalidInput) {
//         free(images[arraySize-1].x);
//         free(images[arraySize-1].y);
//         arraySize--;
//         *imagesAddress = (struct imageInfo *) realloc(*imagesAddress,
//          arraySize * sizeof(struct imageInfo));
//     }
//     return arraySize;
// }

// /*
//     Draw an image on Sketchpad
// Input:
//     line - a character array (string) with the image name to be drawn on
//      Sketchpad
//     images - an array containing elements of type struct imageInfo that holds
//      all relevant information about the image to be drawn on Sketchpad
//     executable - the child process running the Sketchpad program
//     arraySize - the current size of the dynamic array images
// Output:
//     desiredIndex - the index of the image in the images array which will be
//      used by the main function to assess whether an an invalid input was
//      encountered
//     Nevertheless, as a side effect, this function sends the image point
//      coordinate information to Sketchpad via a child process such that all
//      line segments connecting the points are drawn and presented in a new
//       window (assuming the image is defined in the images array)
// */
// int draw(char line[], struct imageInfo *images, FILE* executable,
//  int arraySize) {
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
//                 fprintf(executable, "drawSegment %ld %ld %ld %ld\n",
//                  lround(x1), lround(y1), lround(x2), lround(y2));
//             }
//         }
//     }
//     return desiredIndex;
// }

// /*
//     Translate all the points corresponding to an image by some specified
//      amount in the x- and y-axes
// Input:
//     line - a character array (string) with the image name and the translation
//      amounts in the x- and y-axes
//     images - an array containing elements of type struct imageInfo that holds
//      all relevant coordinate information about the image to be translated
// Output:
//     desiredIndex - the index of the image in the images array which will be
//      used by the main function to assess whether an an invalid input was
//      encountered
//     Nevertheless, as a side effect, this function translates all the point
//      coordinates of the image in question by some specified amount (assuming
//      the image is defined in the images array)
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
//     line - a character array (string) with the child command to be sent to the
//      child process running the Sketchpad program
//     executable - the child process running the Sketchpad program
// Output:
//     None. However, as a side effect,
//     this function processes the child command and
//     sends it to Sketchpad via the child process
//     for execution
// */
// void child(char line[], FILE* executable) {
//     fprintf(executable, "%s\n", line+strlen("child"));
// }

// /*
//     Rotate an image by a specified amount of degrees
// Input:
//     line - a character array (string) with the name of the image to be rotated
//     images - an array containing elements of type struct imageInfo that holds
//      all relevant information about the image to be rotated
//     arraySize - the current size of the dynamic array images
// Output:
//     desiredIndex - the index of the image in the images array which will be
//      used by the main function to assess whether an an invalid input was
//      encountered
//     Nevertheless, as a side effect, this function updates all necessary
//      coordinate information about an image in a struct imageInfo array
//      (assuming the image is defined in the images array in the first place)
// */
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

// /*
//     Return the index of the input image in the struct imageInfo images array
// Input:
//     imageName - a character array (string) with the name of the image whose
//      index in the images array is to be determined
//     images - an array containing elements of type struct imageInfo that holds
//      all relevant information about all the input images
//     arraySize - the current size of the dynamic array images
// Output:
//     desiredIndex - the index of the input image in the images array. If the
//      image is not defined, -1 is returned
// */
// int desiredImage(char imageName[], struct imageInfo *images, int arraySize) {
//     int desiredIndex = -1;
//     for (int i = 0; i < arraySize; i++) {
//         if (strncmp(imageName, images[i].name, LINE_LEN) == 0)
//             desiredIndex = i;
//     }
//     return desiredIndex;
// }

// /*
//     Handle the execution of the program when an error is detected. This
//      includes printing an error message to stderr, closing open files and
//      pipes, freeing up previously allocated memory and exiting the process
//      with EXIT_FAILURE
// Input:
//     lineNumber - the current line in the input file where an invalid input was
//      detected
//     inputOpen - a flag variable that indicates whether the input file is open
//      or not
//     input - the input file pointer used to get input for our program
//     executableOpen - a flag variable that indicates whether the executable
//      (i.e., the pipe to Sketchpad) is open or not
//     executable - the child process running the Sketchpad program
//     memoryAllocated - a flag variable that indicates whether in this process
//      memory has been previously allocated or not
//     images - an array containing elements of type struct imageInfo that holds
//      all relevant information about all the input images
//     arraySize - the current size of the dynamic array images
// Output:
//     Exit the program with EXIT_FAILURE.
//     Moreover, print an error message to stderr, close open files and
//      pipes and free up allocated memory
// */
// void errorPrint(int lineNumber, int inputOpen, FILE *input,
//  int executableOpen, FILE *executable, int memoryAllocated,
//   struct imageInfo *images, int arraySize) {
//     fprintf(stderr, "draw2: %d, error.\n", lineNumber);
//     if (inputOpen)
//         fclose(input);
//     if (executableOpen) {
//         char* end = " end";
//         fprintf(executable, "%s\n", end);
//         pclose(executable);
//     }
//     if (memoryAllocated) {
//         for (int i = 0; i < arraySize; i++) {
//             free(images[i].x);
//             free(images[i].y);
//         }
//         free(images);
//     }
//     exit(EXIT_FAILURE);
// }
