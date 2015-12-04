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
	thread tasks[3] = {
		thread(task, 'A'),
		thread(task, 'B'),
		thread(task, 'C')
	};
	for (int i = 0; i < 3; i++)
		tasks[i].join();
	getchar();
	return 0;
}
