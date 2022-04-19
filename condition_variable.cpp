#include <iostream>
#include <condition_variable>
#include <thread>

std::mutex lock;
std::condition_variable go;
bool flag = false;

void func()
{
	for (int i = 0; i < 10000; i++) {
		{
			std::lock_guard<std::mutex> lk(lock);
			if (i == 9000) {
				flag = true;
				go.notify_one();
				std::cout << i << std::endl;
			}
		}
	}
}


int main()
{
	std::thread t1([]() {
		std::cout << "thread 1 waiting" << std::endl;
		std::unique_lock<std::mutex> lk(lock);
		go.wait(lk, []() {return flag; });
		std::cout << "thread 1 start" << std::endl;
		});

	std::thread t2(func);

	t1.join();
	t2.join();
}
