#include <stdio.h>
#include <stdlib.h>

#define LINE_LENGTH 256


int main(void) {
	FILE *input = fopen("input3.txt", "r");
	if (input == NULL) {
		printf("Could not open input file %s\n", "input3.txt");
		exit(1);
	}
	
	int arraySize = 10;
	long int *numberArray = (long int *) malloc(arraySize*sizeof(long int));
	
    int index = 0;
	char line[LINE_LENGTH];
	while (fgets(line, LINE_LENGTH, input) != NULL) {
		if (index == arraySize) {
			arraySize = arraySize * 2;
			numberArray = (long int *) realloc(numberArray, arraySize*sizeof(long int));
		}
		sscanf(line, "%ld", &numberArray[index]);
		index++;
		
	}
    index--;
                                                      
	for (int i = index; i >= 0; i--) {
		printf("%ld\n", numberArray[i]);
	}
	free(numberArray);
	fclose(input);  
}
