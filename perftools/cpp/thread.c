
#include <iostream>
#include <thread>

using namespace std;

void SayHello() {
	cout << "Hello World" << endl;
}

int main() {
	std::thread t1(SayHello);
	// 等待子线程结束才退出当前线程
	pthread_exit(nullptr);
	return 0;
}
