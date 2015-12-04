#include <iostream>
#include <thread>

using namespace std;

void userTask() {
	for (int i = 0; i < 5; i++) {
		cout << "Hello from user thread!" << endl;
	}
}

int main() {
	thread uT(userTask);
	for (int i = 0; i < 5; i++) {
		cout << "Hello from main thread!" << endl;
	}
	uT.join();
	getchar();
	return 0;
}
