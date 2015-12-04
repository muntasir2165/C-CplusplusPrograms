#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

void task(char id) {
	static mutex critSection;
	critSection.lock();
	for (int i = 0; i < 50; i++) {
		cout << id;
	}
	cout << endl;
	critSection.unlock();
}

int main() {
	thread tA(task, 'A');
	thread tB(task, 'B');
	thread tC(task, 'C');
	tA.join();
	tB.join();
	tC.join();
	getchar();
	return 0;
}
