#include <iostream>
#include <thread>
#include <future>

int func()
{
	for (int i = 0; i < 100; i++) {
		if (i == 90) {
			return i;
		}
	}
}


int main()
{
	bool flag = false;
	std::future<int> f(std::async(func));//func线程正常处理
	std::thread t([&f]() {
		for (int i = 0; i < 500; i++) {
			std::cout << "main : " << i << std::endl;
			if (i == 450) {//一定条件下，希望获取func的值时
				std::cout << "get value : " << f.get() << std::endl;//调用get()
			}
		}
		});
	t.join();
}
