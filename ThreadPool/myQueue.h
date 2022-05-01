#pragma once

#include <queue>
#include <functional>
#include <mutex>
#include <iostream>

template<class T>
class MyQueue
{
	std::mutex lock;
	std::queue<T> _q;
public:
	void push(T& t) {
		std::lock_guard<std::mutex> lk(lock);
		_q.push(t);
		std::cout <<"&push  count:" << _q.size() << "\n";
	}

	void push(T&& t) {
		std::lock_guard lk(lock);
		_q.push(t);
		std::cout <<"&&push  count:" << _q.size() << "\n";
	}

	bool pop(T& t) {
		std::lock_guard lk(lock);
		if (_q.empty())
			return false;
		t = _q.front();
		_q.pop();
		std::cout <<"pop  count:" << _q.size() << "\n";
		return true;
	}

	size_t size() {
		std::lock_guard lk(lock);
		return _q.size();
	}

	bool empty()
	{
		std::lock_guard lk(lock);
		return _q.empty();
	}
};



