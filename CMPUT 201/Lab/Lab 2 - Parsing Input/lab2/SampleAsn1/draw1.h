/*
 * Header file for sample (but incorrect) draw1.c.  Thus not a correct
 * header file for Assignment 1 (e.g., yours needs to have more
 * declarations).
 *
 * Only contains prototypes for non-standard library pipe functions.
 *
 * Style is not necessarily the style required for Assignment 1.
 */

#ifndef DRAW1_H
#define DRAW1_H

FILE *popen(const char *command, const char *type);
int pclose(FILE *stream);

#endif
