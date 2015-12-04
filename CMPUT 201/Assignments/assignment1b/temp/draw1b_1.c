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

/*
#define MAXIMUM 256 //the maximum number of characters
//per line from the input file (including the newline)
*/

/*
char program_name[PROG_LEN + 1];
*/
const char exec_name[] = "java -jar Sketchpad.jar -d";
//const char exec_name[] = "java -jar Sketchpad.jar";
int shiftX; 
int shiftY;

int main( int argc, char * argv[] )
{
    //declade FILE pointers for the input and
    //executable files
    FILE* input;
    FILE* executable;

    /*
    //create an array of strings to hold the keywords
    char *keywords[] = {"Image", "lineTo", "End", 
    "print", "draw", "translate", "child", "#"};
    //declare and initialize an array of ints that
    //correspond to the count of each keyword in the
    //input file
    int stats[] = {0, 0, 0, 0, 0, 0, 0, 0};
    //determine the size of the keywords and stats arrays
    int numberKeywords = sizeof(keywords)/sizeof(keywords[0]);
    //create an array of strings to store the message
    //fragments to output to stdout with the count statistics
    char *outputMessage[] = {"Image definition(s)",
    "lineTo command(s) within Figures", "End command(s)",
    "print command(s)", "draw command(s)",
    "translate command(s)", "child command(s)",
    "comment(s)"};
    */
    //print a start-up message stating how and when
    //this program is executed and the name of the
    //input file
    printf("%s started on ", argv[0]); 
    fflush( stdout );
    system("date");
    printf("Input file: %s\n", argv[1]); 

    //open the input file for reading
    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("Could not open file %s\n", argv[1]);
        exit(-1);
    } else {
          while(!feof(fp)) {
            char line[MAXIMUM];
            char *word;
            fgets(line, MAXIMUM, fp);
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

    //close the file after the program is done reading
    //from it
    fclose(fp);
  } 

    //print out some statistics in the stdout about the
    //number of the eight keyword instances in the input
    //file
    for (int i = 0; i < numberKeywords; i++) {
      printf("%i %s\n", stats[i], outputMessage[i]);
    }

    return(0);
}