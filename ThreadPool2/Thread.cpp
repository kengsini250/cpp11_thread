#include "ThreadPool.h"


class Test1 :public Runnable
{
public:
	void run()override
	{
		int v = 0;
		for (int i = 0; i < 50000; i++)
			v++;
		std::cout << "A:" << v << "\n";
	}
};

class Test2 :public Runnable
{
public:
	void run()override
	{
		int v = 0;
		for (int i = 0; i < 50000; i++)
			v++;
		std::cout << "B:" << v << "\n";
	}
};

int main()
{
	ThreadPool pool(4);

	Test1* t1 = new Test1;
	Test2* t2 = new Test2;

	pool.push(t1);
	pool.push(t2);

	char a;
	std::cin >> a;
	pool.shutdown();
}

