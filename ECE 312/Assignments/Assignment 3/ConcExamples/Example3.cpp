#include <iostream>
#include <thread>
#include <mutex>

using namespace std;
mutex critSection;

void taskA() {
	critSection.lock();
	for (int i = 0; i < 50; i++) {
		cout << 'A';
	}
	cout << endl;
	critSection.unlock();
}

void taskB() {
	critSection.lock();
	for (int i = 0; i < 50; i++) {
		cout << 'B';
	}
	cout << endl;
	critSection.unlock();
}

void taskC() {
	critSection.lock();
	for (int i = 0; i < 50; i++) {
		cout << 'C';
	}
	cout << endl;
	critSection.unlock();
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
