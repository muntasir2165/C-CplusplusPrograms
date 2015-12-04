#include <stdio.h>
#include <stdlib.h>

#define LINE_LENGTH 256
#define LINE_NUMBER 100

int main(void) {
	long int numbers[LINE_NUMBER];
	int index = -1;
	FILE *input = fopen("input1.txt", "r");
	if (input == NULL) {
		printf("Could not open input file %s\n", "input1.txt");
		exit(1);
	}
	
	while (!feof(input)) {
		char line[LINE_LENGTH];
		long int number;
		fgets(line, LINE_LENGTH, input);
		sscanf(line, "%ld", &number);
		index++;
		numbers[index] = number;
	}                                                                            
	for (int i = index-1; i >= 0; i--) {
		printf("%ld\n", numbers[i]);
	}
	fclose(input);  
}
