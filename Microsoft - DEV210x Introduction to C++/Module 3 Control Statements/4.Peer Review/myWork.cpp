#include <iostream>
using namespace std;

int main () {
	
	double base = 2;
	int exponent = 8;
	double result = 1;
	for (int i = 1; i <= exponent; i++)
	{
		result *= base;
	}
	cout << base << "^" << exponent << " = " << result << endl;
	cout << endl;

	char response = 'Y';
	switch (response)
	{
		case 'y':
			cout << "You chose y" << endl;
			break;
		case 'Y':
			cout << "You chose Y" << endl;
			break;
		case 'n':
			cout << "You chose n" << endl;
			break;
		case 'N':
			cout << "You chose N" << endl;
			break;
		default:
			cout << "You didn't choose a valid option" << endl;
			break;

	}

	return 0;
}