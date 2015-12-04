/* name:               Stef Nychka
 * ONE Card number:    9999999
 * Unix id:            stef
 * lecture section:    A1
 * instructor's name:  Dr. Davood Rafiei
 * lab section:        D01
 * TA's name:          Kiana Hajebi
 *
 *  Header file containing declarations, global variable definitions,
 *  and macros for parse.c, which reads input and converts it to input
 *  for sketchpad.pl (see parse.c for more information).
 */

#ifndef PARSE_H
#define PARSE_H

/* Macros and const string variable declarations. */

/* The expected number of command line arguments. */
#define NUM_ARGS 1

/* The maximum length for the program's name in an error message. */
#define PROG_LEN 128

/* The maximum length for an input line.*/
#define LINE_LEN 256

/* This indicates an error when returned by pclose. */
#define PCLOSE_ERR -1

/* The length of the Pause instruction. */
#define PAUSE_LEN 5

/* Global variables. */
/* See comments in parse.c for more information. */

/* The program's name. */
extern char program_g[];

/* The name of the program to which information is sent. */
extern const char Exec_name[];

/* Non-C99 compliant function prototypes. */
FILE *popen(const char *command, const char *type);
int pclose(FILE *stream);
long int lround(double x);

/* Function declarations for parse.c functions. */
/* See comments in parse.c for more information. */

/* Reads from input, sends to executable. */
void convert(FILE* input, FILE* executable);

/* Prints error msg, and maybe file_name. */
void print_err(const char msg[], const char file_name[]);

#endif
