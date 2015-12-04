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
	const int NUMTASKS = 3;
	thread tasks[NUMTASKS];
	for (int i = 0; i < NUMTASKS; i++)
		tasks[i] = thread(task, 'A' + i);
	for (int i = 0; i < NUMTASKS; i++)
		tasks[i].join();
	getchar();
	return 0;
}
