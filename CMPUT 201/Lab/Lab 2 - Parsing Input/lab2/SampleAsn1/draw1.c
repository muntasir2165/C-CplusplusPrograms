/*
 * Used to illustrate file structure of Assignment 1 (asn1) code, and
 * how it will look when run on online input.
 *
 * Code it not at all correct, and does not illustrate much how to
 * solve Assignment 1.  It does show how to open a pipe to sketchpad, how
 * to use fprintf to send information to sketchpad, how to handle return
 * values from library function indicating an error, but not much else.
 *
 * Style used is not necessarily the style required for Assignment 1.
 */

#include <stdio.h>
#include <stdlib.h>
#include "draw1.h"

int main()
{
  FILE* sketcher;

  sketcher = popen("java -jar Sketchpad.jar", "w");
  if (sketcher == NULL)
  {
    fprintf(stderr, "Problem opening pipe to java -jar Sketchpad.jar\n");
    exit(EXIT_FAILURE);
  }

  // What to send to sketchpad after interpreting online input file is
  // hardcoded below (this can't be hardcoded in your code, otherwise
  // it would only work for the online input file, and no other input).
  fprintf(sketcher, ""
                  "drawSegment 100 100 200 200\n"
                  "drawSegment 200 200 300 300\n"
                  "drawSegment 10 10 400 10\n"
                  "drawSegment 400 10 400 400\n"
                  "drawSegment 400 400 10 400\n"
                  "drawSegment 10 400 10 10\n"
                  "drawSegment 120 130 220 230\n"
                  "drawSegment 220 230 320 330\n"
                  "pause 2\n"
                  "clearScreen\n"
                  "end\n");


  if (pclose(sketcher) == -1)
  {
    fprintf(stderr, "Problem closing pipe to java -jar Sketchpad.jar\n");
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);

}
