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

#include <stdio.h> //for sscanf(), fprintf(), printf()
#include <stdlib.h>
#include <math.h> //
#include <string.h> //for strtok()
#include <ctype.h> //for lround()
#include "draw1b.h"

//function prototypes
void comment(char line[]);
//void image(char line[], struct imageInfo images[], imageIdx);
//int findDesiredIdx(struct imageInfo images[], int imageIdx, char imageName);
//void draw(char line[], struct imageInfo images[], imageName);
//void print(struct imageInfo images[], int desiredIdx);
//void translate(struct imageInfo images[], int desiredIdx, double x, double y);
void child(char line[], FILE* executable);

const char exec_name[] = "java -jar Sketchpad.jar -d";
//const char exec_name[] = "java -jar Sketchpad.jar";
struct imageInfo {
    char name;
    int pointsTotal;
    double x[LINE_NUM+1];
    double y[LINE_NUM+1];
    double currentX;
    double currentY;
};

int main( int argc, char * argv[] )
{
    //declade FILE pointers for the input and
    //executable files
    FILE* input;
    FILE* executable;
    
    //lineType variable will be used to delegate line
    //processing operations to the relevant functions
    int lineType = -1; //-1 is the default value
    
    //a struct to store all relevant information about
    //images from the input file (e.g., image name, point
    //coordinates)
    struct imageInfo images[IMAGE_NUM];
    
    //an index variable to index through the images array
    //int imageIdx = 0;

    //create an array of strings to correspond
    //to the six types of input lines
    char *keywords[] = {"#", "Image", "draw", 
    "print", "translate", "child"};
    //char *imageKeywords[] = {"lineTo"}, "End"};

    //determine the size of the keywords array
    int numberKeywords = sizeof(keywords)/sizeof(keywords[0]);

    //print a start-up message stating how and when
    //this program is executed and the name of the
    //input file
    printf("%s started on ", argv[0]); 
    fflush( stdout );
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

    while(!feof(input)) {
            char line[LINE_LEN];
            char *word;
            fgets(line, LINE_LEN, input);
            //get the first word in the line and store it
            //in the character string word
            word = strtok(line, " ");
            //check if the word matches any of the keywords
            //and increment its count accordingly
            for (int i = 0; i < numberKeywords; i++) {
                if (strncmp(word, keywords[i], LINE_LEN) == 0) {
                    lineType = i+1;
                    break;
                }
            }
            switch (lineType){
                case 1:
                    comment(line);
                    break;
                /*
                case 2:
                    double x, y;
                    char imageName;
                    sscanf(line, "Image %c %g %g", &imageName, &x, &y);
                    images[imageIdx].name = imageName;
                    images[imageIdx].x[images[imageIdx].pointsTotal] = x;
                    images[imageIdx].y[images[imageIdx].pointsTotal]) = y;
                    images[imadeIdx].currentX = x;
                    images[imadeIdx].currentY = y;
                    images[imageIdx].pointsTotal++;
                    int imageEnd = 0;
                    while (imageEnd != 1) {
                        char line2[LINE_LEN];
                        char *word2;
                        fgets(line2, LINE_LEN, input);
                        //get the first word in the line and store it
                        //in the character string word
                        word2 = strtok(line2, " ");
                        if (strncmp(word2, keywords[0], LINE_LEN) == 0) {
                            comment(line2[]);
                        } else if (strncmp(word2, "lineTo", 6) == 0) {
                            image(line2, images, imageIdx);
                        } else if (strncmp(word2, "End", 3) == 0) {
                            imageEnd = 1;
                        }
                    }
                    imageIdx++;
                    break;
                case 3:
                    draw();
                    break;
                case 4:
                    char imageName;
                    sscanf(line, "print %c", &imageName);
                    int desiredIdx = findDesiredIdx(images, imageIdx, imageName)
                    print(images, desiredIdx);
                    break;
                case 5:
                    char imageName;
                    double traslateX, translateY;
                    sscanf(line, "translate %c %g %g", &imageName, &traslateX, &translateY);
                    int desiredIdx = findDesiredIdx(images, imageIdx, imageName);
                    translate(images, desiredIdx, x, y);
                    break;
                    */
                case 6:
                    child(line, executable);
                    break;
                default:
                    printf("Invalid line type\n");
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
    line - a character array 
Output:
    None. However, as a side effect,
    this function takes the comment in
    line and prints it to stdout
*/
void comment(char line[]) {
    printf("%s\n", line);
}

/*
    Process and store point information about an image in a 
    pre-defined struct imageInfo array
Input: 
    line - a character array
    images - an array of type stuct imageInfo
    imageIdx - the current index in 
Output:
    None. However, as a side effect,
    this function takes the comment in
    line and prints it to stdout
*/
    /*
void image(char line[], struct imageInfo images[], imageIdx) {
    double currentX =  images[imadeIdx].currentX;
    double currentY = images[imageIdx].currentY;
    double deltaX, deltaY;
    sscanf(line, "lineTo %g %g", &deltaX, &deltaY);
    currentX += deltaX;
    currentY += deltaY;
    images[imadeIdx].currentX = currentX;
    images[imageIdx].currentY = currentY;
    images[imageIdx].x[images[imageIdx].pointsTotal] = currentX;
    images[imageIdx].y[images[imageIdx].pointsTotal] = currentY;
    images[imageIdx].pointsTotal++;
}

int findDesiredIdx(struct imageInfo images[], int imageIdx, char imageName) {
    int desiredIdx = -1;
    for (int i=0; i < imageIdx; i++) {
        if (images[i].name == imageName) {
            desiredIdx = i;
            break;
        }
    }
    return desiredIdx;
}
*/
/*
void draw(FILE* input, FILE* executable) {

}

void print(struct imageInfo images[], int desiredIdx) {
    printf("Print Image %c =\n", images[desiredIdx].name);
    for (int j=0; j < images[desiredIdx].pointsTotal; j++) {
        printf("%d %d\n", images[desiredIdx].x[j], images[desiredIdx].y[j]);
    }
    printf("End Image %c\n", images[desiredIdx].name);
}

void translate(struct imageInfo images[], int desiredIdx, double x, double y) {
    for (int i=0; j < images[desiredIdx].pointsTotal; j++) {
        images[desiredIdx].x[i] += x;
        images[desiredIdx].y[i]) += y;
    }
    images[desiredIdx].currentX[i] += x;
    images[desiredIdx].currentY[i]) += y;
}
*/
void child(char line[], FILE* executable) {
    char textToExecutable[LINE_LEN];
    sscanf(line, "child %s", textToExecutable);

    //copy textToExecutable to the executable
    fprintf(executable, "%s", textToExecutable);  
}

