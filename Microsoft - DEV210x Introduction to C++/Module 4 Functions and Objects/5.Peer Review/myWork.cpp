#include <iostream>
using namespace std;

int pow (int base, int exponent)
{
	int result = 1;
	if (exponent < 0)
	{
		// -1 is returned to imply error since
		// negative exponent is not allowed
		return -1;
	}
	else if (exponent == 0)
	{
		return result;
	}
	else
	{
		for (int i = 1; i <= exponent; i++)
		{
			result *= base;
		}
		return result;
	}
}

double sine(double opposite, double hypotenuse)
{
	return opposite/hypotenuse;
}

double average(int values[], int length)
{	
	double total = 0.0;
	for (int i = 0; i < length; i++)
	{
		total += values[i];
		// cout << "values["<< i << "]: " << values[i] << endl;
		// cout << endl;
	}
	return total/length;
}

int main () {
	
	int base = 2;
	int exponent = 8;
	int result = pow(base, exponent);
	cout << base << "^" << exponent << " = " << result << endl;
	cout << endl;

	double opposite = 4.2;
	double hypotenuse = 2.1;
	double sineValue = sine(opposite, hypotenuse);
	cout << "Sine of a triangle with opposite " << opposite << " and hypotenuse " << hypotenuse << " is: " << sineValue << endl;
	cout << endl;

	int values[] = {1, 2, 3, 4, 5};
	int length = sizeof(values)/sizeof(values[0]);
	// debugging code
	// cout <<"length: " << length << endl;
	// cout << "sizeof(values): " << sizeof(values) <<endl;
	// cout << "sizeof(values[0]) " << sizeof(values[0]) <<endl;
	// cout << endl;
	double avg = average(values, length);
	cout << "The average of the values array is: " << avg << endl;
	cout << endl;

	return 0;
}