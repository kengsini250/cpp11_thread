#include "ThreadPool.h"
#include <Windows.h>
#include <random>

//模拟数据包
struct Data
{
	int id;
	int value;
};

//模拟多文件处理
void test1(int& s1, int& s2, int& s3, int& s4, const Data& data)
{
	if (data.id == 0) {
		s1 += data.value;
	}
	Sleep(100);
}

void test2(int& s1, int& s2, int& s3, int& s4, const Data& data)
{
	if (data.id == 1) {
		s2 += data.value;
	}
	Sleep(100);
}

void test3(int& s1, int& s2, int& s3, int& s4, const Data& data)
{
	if (data.id == 2) {
		s3 += data.value;
	}
	Sleep(100);
}

void test4(int& s1, int& s2, int& s3, int& s4, const Data& data)
{
	if (data.id == 3) {
		s4 += data.value;
	}
	Sleep(100);
}


int main()
{
	ThreadPool pool(4);

	int s1= 0;
	int s2 = 0;
	int s3 = 0;
	int s4 = 0;

	std::default_random_engine e(time(NULL));
	std::uniform_int_distribution d(0, 3);

	std::shared_future<void> out;

	//模拟服务器向客户端发送数据包
	std::thread mainThread([&out, &s1, &s2, &s3, &s4,&pool,&d,&e] {
		for (int i = 0; i < 100; i++) {
			//push
			//模拟客户端处理数据包
			Data data{ d(e), 1 };

			//模拟客户端处理多个文件
			out = pool.push(test1,std::ref(s1),std::ref(s2),std::ref(s3),std::ref(s4), data);
			out = pool.push(test2,std::ref(s1),std::ref(s2),std::ref(s3),std::ref(s4), data);
			out = pool.push(test3,std::ref(s1),std::ref(s2),std::ref(s3),std::ref(s4), data);
			out = pool.push(test4,std::ref(s1),std::ref(s2),std::ref(s3),std::ref(s4), data);

			//模拟客户端处理结果
			out.get();
			std::cout << "------------------------------------------\n";
			std::cout << s1 << "\n";
			std::cout << s2 << "\n";
			std::cout << s3 << "\n";
			std::cout << s4 << "\n";
			Sleep(350);
		}
		});
	mainThread.join();

	std::cout << "total : [" << s1 + s2 + s3 + s4 << "]\n";
	getchar();
	pool.shutdown();
}

