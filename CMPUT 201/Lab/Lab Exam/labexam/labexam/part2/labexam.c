// Starter file for 201 lab exam 

// You may add other includes here.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


// You may add global variables, declarations and/or
// definitions, struct type definitions, macros or function declarations
// here

// e.g.
// #define MAXBUF 80

void do_section1();
void do_section2();
void do_section3();

// do not modify this main function in any way ... don't touch it.
// main checks for some errors, for your convenience - it may make 
// debugging easier.
int main(int argc, char *argv[]) {
    if (argc != 2) {
      fprintf(stderr, "error: required argument not given\n");
      fprintf(stderr, "       usage example: ./labexam 1 < input_file > output_file\n"); 
      exit(EXIT_FAILURE);
    }

    int which_section;
    int ret_val = sscanf(argv[1], "%d", &which_section); 
    if (ret_val != 1) {
        fprintf(stderr, "error: argument is not a number, %s\n",  argv[1]);
    exit(EXIT_FAILURE);
    } 

    switch(which_section) {
        case 1:
            do_section1();
            break;
        case 2:
            do_section2();
            break;
        case 3:
            do_section3();
            break;
        default:
            fprintf(stderr, "error: argument is not between 1 and 4, %s\n", argv[1]);
            exit(EXIT_FAILURE);
    }
  
} /* end of main */
 
// Add your code below this line.  If needed, create other helper functions.
//============================================================================


void do_section1() {
  // Add code here that does what is needed for Section 1.  Free up all
  // memory allocated (if you have allocated memory).  You may call
  // other functions you create.
} /* end of do_section1 */
 
void do_section2() {
  // Add code here that does what is needed for Section 2.  Free up all
  // memory allocated (if you have allocated memory).  You may call
  // other functions you create.
} /* end of do_section2 */
  
void do_section3() {
  // Add code here that does what is needed for Section 3.  Free up all
  // memory allocated (if you have allocated memory).  You may call
  // other functions you create.
} /* end of do_section3 */


