#ifndef PARSE_H
#define PARSE_H

#define PROG_LEN 128
#define LINE_LEN 256

/* Non-C99 compliant function prototypes. */
FILE *popen(const char *command, const char *type);
int pclose(FILE *stream);
long int lround(double x);

void convert(FILE* input, FILE* executable);

#endif
