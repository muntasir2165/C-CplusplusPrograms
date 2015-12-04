/*
 * Shows how to use MEMWATCH.  Otherwise, not useful for asn2.
 *
 * Style is not necessarily correct.
 */

#include <stdlib.h>
#include "memwatch.h" /*UNCOMMENT!*/

/*
 * MEMWATCH detects unfreed memory (and other things).  The following
 * purposely has a mistake to illustrate this.  Remember to always
 * free all memory.  (Uncomment free command below to do this for this
 * code.)
 */
int main(int argc, char* argv[])
{
  /* can insert a command like below to see if you have MEMWATCH set
   * up properly for your code. */
  char* c = malloc(1);
  /*free(c);*/ // normally want to free memory, but not when testing
               // memwatch
  return 0;
}
