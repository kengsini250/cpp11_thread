#pragma once

#include <queue>
#include <functional>
#include <mutex>
#include <iostream>


class Runnable {
public:
	virtual ~Runnable(){}
	virtual void run() = 0;
};


template<class T>
class MyQueue
{
	std::mutex lock;
	std::queue<T> _q;
public:
	void push(T& t) {
		std::lock_guard<std::mutex> lk(lock);
		_q.push(t);
	}

	void push(T&& t) {
		std::lock_guard lk(lock);
		_q.push(t);
	}

	bool pop(T& t) {
		std::lock_guard lk(lock);
		if (_q.empty())
			return false;
		t = _q.front();
		_q.pop();
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



