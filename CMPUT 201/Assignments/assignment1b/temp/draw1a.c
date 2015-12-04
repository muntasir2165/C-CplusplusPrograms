#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main( int argc, char * argv[] )
{ 
   FILE *fp;
   int rval;
   char Word[256];
   int image = 0;
   int lineTo = 0;
   int end = 0;
   int print = 0;
   int draw = 0;
   int translate = 0;
   int child = 0;
   int pound = 0;
   char *keywords[] = {"Image", "lineTo", "End", 
   "print", "draw", "translate", "child", "#"};
   char stats[] = {0, 0, 0, 0, 0, 0, 0, 0};
   int numberKeywords = sizeof(keywords)/sizeof(keywords[0]);
   
   for (int i = 0; i < numberKeywords; i++) {
    printf("%s %i\n", keywords[i], stats[i]);
   }
   printf("\n");
   //print a start-up message
  printf("%s started on ", argv[0]); 
  fflush( stdout );
  system("date");
  printf("Input file: %s\n", argv[1]); 

   fp = fopen(argv[1], "r");
   if (fp == NULL) {
      printf("Could not open file %s\n", argv[1]);
      exit(-1);
  } 
  else {
      while(!feof( fp )) {
                rval = fscanf(fp, "%s", Word);
                if (strncmp(Word, "Image", 256) == 0) {
                    image++;
                  } else if (strncmp(Word, "lineTo", 256) == 0) {
                    lineTo++;
                  } else if (strncmp(Word, "End", 256) == 0) {
                    end++;
                  } else if (strncmp(Word, "print", 256) == 0) {
                    print++;
                  } else if (strncmp(Word, "draw", 256) == 0) {
                    draw++;
                  } else if (strncmp(Word, "translate", 256) == 0) {
                    translate++;
                  } else if (strncmp(Word, "child", 256) == 0) {
                    child++;
                  } else if (strncmp(Word, "pound", 256) == 0) {
                    pound++;
                }
        }

      fclose(fp);
  } 

   printf("%i Image definition(s)\n", image); 
   printf("%i lineTo command(s) within Figures\n", lineTo); 
   printf("%i End command(s)\n", end);
   printf("%i print command(s)\n", print); 
   printf("%i draw command(s)\n", draw);
   printf("%i translate command(s)\n", translate);     
   printf("%i child command(s)\n", child);
   printf("%i comment(s)\n", pound);    
   return(0);
}
