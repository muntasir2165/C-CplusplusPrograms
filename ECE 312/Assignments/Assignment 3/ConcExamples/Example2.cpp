#include <iostream>
#include <thread>

using namespace std;

void taskA() {
	for (int i = 0; i < 50; i++) {
		cout << 'A';
	}
	cout << endl;
}

void taskB() {
	for (int i = 0; i < 50; i++) {
		cout << 'B';
	}
	cout << endl;
}

void taskC() {
	for (int i = 0; i < 50; i++) {
		cout << 'C';
	}
	cout << endl;
}

int main() {
	thread tA(taskA);
	thread tB(taskB);
	thread tC(taskC);
	tA.join();
	tB.join();
	tC.join();
	getchar();
	return 0;
}
