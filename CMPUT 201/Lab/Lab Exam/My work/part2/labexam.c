// Starter file for 201 lab exam 

// You may add other includes here.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


// You may add global variables, declarations and/or
// definitions, struct type definitions, macros or function declarations
// here

// define a macro to indicate the maximum length of a word from stdin in
// section 3
#define WORD_LEN 32
// a struct to store all the number and word pairs from stdin in section 3
struct wordInfo {
	int number;
    char word[WORD_LEN];
};

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
//Helper function for section 3. Please note that this function was copied from
//the internet (http://www.tutorialspoint.com/c_standard_library/c_function_qsort.htm)
int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

void do_section1() {
  // Add code here that does what is needed for Section 1.  Free up all
  // memory allocated (if you have allocated memory).  You may call
  // other functions you create.

	int count = 0;
	int wordcount = 0;
	char letter;
	
	letter = getchar();
	while(letter != EOF) {
		count++;
		if (isdigit(letter) != 0) {
			continue;
		} else if (isspace(letter) != 0) {
			printf(",%d\n", count-wordcount);
			wordcount = 0;
		} else if (isalpha(letter) != 0) {
			putchar(letter);
			wordcount++;
		} else {
			wordcount++;
		}
		// carry on getting character from the standard input
		letter = getchar();
	}

} /* end of do_section1 */
 
void do_section2() {
  // Add code here that does what is needed for Section 2.  Free up all
  // memory allocated (if you have allocated memory).  You may call
  // other functions you create.
	char letter;
	letter = getchar();
	while(letter != EOF) {
		if (letter == '<') {
			while (letter != '>') {
				letter = getchar();
			}
			//read in the '>' character
			letter = getchar();
		} else {
			putchar(letter);
			letter = getchar();
		}
	}
} /* end of do_section2 */
  
void do_section3() {
  // Add code here that does what is needed for Section 3.  Free up all
  // memory allocated (if you have allocated memory).  You may call
  // other functions you create.
	struct wordInfo *input = NULL;
	int count = 0;
	int number = 0;
	char word[WORD_LEN];
	memset(word, 0, WORD_LEN);

	while (scanf("%d,%s\n", &number, word) != EOF) {
		count++;
		//allocate/reallocate memory for the images array
		if (count == 1) {
	        input = (struct wordInfo *) malloc(count * sizeof(struct wordInfo));
    	} else {
    	    input = (struct wordInfo *) realloc(input, count * sizeof(struct wordInfo));
	    }
    	input[count-1].number = number;
		strncpy(input[count-1].word, word, WORD_LEN);
	}	
	
	/*
	//code to verify that the inputs are being read into the structs correctly
	for (int i = 0; i < count; i++) {
		printf("Number:%d, Word:%s\n", input[i].number, input[i].word);
	}
	printf("Count:%d\n", count);
	*/
	
	//create and populate the values array with the input numbers that need sorting
	int values[count];
	for (int i = 0; i < count; i++) {
		values[i] = input[i].number;
	}
	
	/*
	printf("Before sorting:\n");
	for (int i = 0; i < count; i++) {
		printf("%d ", values[i]);
	}
	printf("\n");
	*/
	qsort(values, count, sizeof(int), cmpfunc);
	/*
	printf("After sorting:\n");
	for (int i = 0; i < count; i++) {
		printf("%d ", values[i]);
	}
	printf("\n");
	*/
	
	//print the words in order corresponding to the numbers in the sorted values array
	for (int i = 0; i < count; i++) {
		int current = values[i];
		for (int j = 0; j < count; j++) {
			if (input[j].number == current) {
				printf("%s", input[j].word);
				if (i < count-1) {
					printf(" ");
				}						
				break;
			}
		}
	}
	
	if (input != NULL) {
		free(input);
	}
	
} /* end of do_section3 */


