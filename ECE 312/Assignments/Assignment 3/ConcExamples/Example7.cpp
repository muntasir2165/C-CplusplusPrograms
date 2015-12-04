#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

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
	int numtasks;
	cout << "Number of tasks? ";
	cin >> numtasks;
	getchar();
	vector<thread> tasks(numtasks);
	for (int i = 0; i < numtasks; i++)
		tasks[i] = thread(task, 'A' + i);
	for (int i = 0; i < numtasks; i++)
		tasks[i].join();
	getchar();
	return 0;
}
