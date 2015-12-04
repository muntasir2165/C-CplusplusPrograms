/* Doesn't contain good style or quality, necessarily. (or robustness,
 * correctness ... it's basically a hacked up "C script". */

/* Generates input file for avrg. */

#include <stdio.h>
#include <stdlib.h>
#define MAX 100
#define SEED 2
#define AMOUNT 100

void srandom(unsigned long int);
long int random(void);

int main()
{
  srandom(2);
  int i;
  FILE* output;

  output=fopen("input", "w");
  for ( i=0; i< AMOUNT; i++)
    /* generates from 1 to MAX */
    fprintf(output, "%d\n", (int)random()%(MAX-1)+1);
  return 0;
  
}
