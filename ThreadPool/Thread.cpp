#include "ThreadPool.h"

int main()
{
	ThreadPool pool(4);

	pool.push([] {
		int v = 0;
		for (int i = 0; i < 50000; i++)
			v++;
		std::cout << "A:" << v << "\n";
		});


	pool.push([] {
		int v = 0;
		for (int i = 0; i < 50900; i++)
			v++;
		std::cout << "B:" << v << "\n";
		});


	char a;
	std::cin >> a;
	pool.shutdown();
}

