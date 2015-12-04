#include <stdio.h>
#include <stdlib.h>

#define LINE_LENGTH 256


int main(void) {
	FILE *input = fopen("input2.txt", "r");
	if (input == NULL) {
		printf("Could not open input file %s\n", "input2.txt");
		exit(1);
	}
	
	//Determine the number of lines to be read
	char firstLine[LINE_LENGTH];
	int totalNumbers;
	fgets(firstLine, LINE_LENGTH, input);
	sscanf(firstLine, "%d", &totalNumbers);

	long int *numberArray = (long int *) malloc(totalNumbers*sizeof(long int));
	
    int index = 0;
	char line[LINE_LENGTH];
	while (fgets(line, LINE_LENGTH, input) != NULL) {	
		sscanf(line, "%ld", &numberArray[index]);
		index++;
	}	
                                                                       
	for (int i = totalNumbers-1; i >= 0; i--) {
		printf("%ld\n", numberArray[i]);
	}
	free(numberArray);
	fclose(input);  
}
