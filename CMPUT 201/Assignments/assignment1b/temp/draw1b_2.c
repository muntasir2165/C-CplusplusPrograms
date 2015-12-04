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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include "draw1b.h"

//function prototypes
void comment(FILE* input, FILE* executable);
void image(FILE* input, FILE* executable);
void draw(FILE* input, FILE* executable);
void print(FILE* input, FILE* executable);
void translate(FILE* input, FILE* executable);
void child(FILE* input, FILE* executable);

const char exec_name[] = "java -jar Sketchpad.jar -d";
//const char exec_name[] = "java -jar Sketchpad.jar";
struct imageInfo {
    char imageName;
    double points[LINE_NUM]
};

int main( int argc, char * argv[] )
{
    //declade FILE pointers for the input and
    //executable files and lineType variable
    //to 0
    FILE* input;
    FILE* executable;
    int lineType = 0;

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

    //open the sketchpad executable to draw
    executable = popen(exec_name, "w");
    if (executable == NULL) {
        printf("Could not open pipe", exec_name);
        exit(-1);
    }

    while(!feof(fp)) {
            char line[LINE_LEN];
            char *word;
            fgets(line, LINE_LEN, fp);
            //get the first word in the line and store it
            //in the character string word
            word = strtok(line, " ");
            //check if the word matches any of the keywords
            //and increment its count accordingly
            for (int i = 0; i < numberKeywords; i++) {
                if (strncmp(word, keywords[i], MAXIMUM) == 0) {
                    stats[i] += 1;
                }
            }
    }

    //close the input file
    fclose(input);

    //close the executable
    pclose(executable);
    

    return(0);
}

