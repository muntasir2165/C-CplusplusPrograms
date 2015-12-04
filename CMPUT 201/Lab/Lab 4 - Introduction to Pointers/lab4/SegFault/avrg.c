/* This is not necessarily a good example of quality or style. */

/* Reads from an input file with 1 int per line, and prints out the average.
 * 
 * Used to show a segfault (segmentation fault)
 * */
  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memwatch.h"

#define EXIT_ERROR -1
#define DEFAULT 10
#define MAX_LINE 256

/*
 * holds an unlimited amount of numbers.
 */
struct vector_type
{
  int max;
  int amount;
  int* numbers;
};
typedef struct vector_type vector_t;

/* see function definitions for more information */

/* find average of numbers in input (calls compute) */
double average(FILE* input, char[]);
/* returns average of numbers in  storage->numbers. */
double compute(vector_t* storage);
/* adds number to storage */
void add(vector_t* storage, int number);
/* initializes storage */
void init(vector_t* storage);
/* prints out error message and perhaps arg1 */
void error(char message[], char arg1[]);

/*
 * Opens input file specified at command prompt, calls average, calls print
 * average, then closes input file.*/
int main(int argc, char* argv[])
{
  if (argc != 2)
    error("usage: ./avrg ./input_file", NULL);

  FILE* input;

  input=fopen(argv[1], "r");
  if (input == NULL)
    error("problem opening file", argv[1]);

  double avg;
  avg = average(input, argv[1]);
  printf("The average is %g\n", avg);

  if (fclose(input) != 0)
    error("problem opening file", argv[1]);
      
  return 0;
}

/*
 * Reads from input, looking for 1 int per line, and returns average.
 * name is name of input file, and is used for error message.
 */
double average(FILE* input, char name[])
{
  char* line;
  vector_t storage;
  int number;
  
  init(&storage);
  
  while(fgets(line, MAX_LINE+1, input))
  {
    /* ignore errors and skip to next line */
    if (sscanf(line, "%d", &number) == 1)
      add(&storage, number);
  }
  if(ferror(input))
    error("error reading file", name);

  double avg;
  avg = compute(&storage);
  free(storage.numbers);
  return avg;
}

/*
 * initializes storage->numbers to size DEFAULT, and zeroes ->amount
 * and ->max
 */
void init(vector_t* storage)
{
  storage->amount = 0;
  storage->numbers = malloc(DEFAULT*sizeof(int));
  if (storage->numbers == NULL)
    error("init: malloc failed", NULL);
  memset(storage->numbers, 0, DEFAULT*sizeof(int));

  storage->max = DEFAULT;
  
}

/*
 * adds number to storage->numbers, growing ->numbers if needed.
 */
void add(vector_t* storage, int number)
{
  int init_max;
  init_max = storage->max;
  if (init_max <= storage->amount)
  {
    int* temp;
    temp = realloc(storage->numbers, init_max*2*sizeof(int));
    if (temp == NULL)
    {
      free(storage->numbers);
      error("add: realloc failed", NULL);
    }
    storage->numbers = temp;
    
    memset(&storage->numbers[init_max], 0, init_max*sizeof(int));
    storage->max *= 2;
  }
  int index;
  index = storage->amount++;
  storage->numbers[index] = number;
}

/*
 * computes and returns average of storage->size numbers in
 * storage->numbers.
 */
double compute(vector_t* storage)
{
  int i;
  int sum;
  sum = 0;
  for(i=0; i < storage->amount++; i++)
    sum += storage->numbers[i];
  return (double)sum/storage->amount;
}

/*
 * prints out error, including message, and arg1 if arg1 is not NULL
 */
void error(char message[], char arg1[])
{
  if (arg1 == NULL)
    fprintf(stderr, "avrg: %s\n", message);
  else
    fprintf(stderr, "avrg: %s %s\n", message, arg1);

  exit(EXIT_ERROR);
}
