#pragma once

#include	"myQueue.h"
#include <vector>
#include <thread>
#include <future>

class ThreadPool
{
private:
	MyQueue<std::function<void()>> queue;
	std::vector<std::thread> threads;
	std::mutex lock;
	std::condition_variable m_conditional_lock;
	volatile bool end = false;
	int max = 0;

	class Worker
	{
		int id;
		ThreadPool* self;
		std::function<void()> func;
	public:
		Worker(ThreadPool* s,int _id) :self(s),id(_id) {
					std::cout << id<<" new worker!\n";
		}

		void operator()() {
	
			while (!self->end) {
				std::unique_lock<std::mutex> lock(self->lock);
				self->m_conditional_lock.wait(lock, [&] {
					std::cout << id << " wait\n";
					if (self->end == true) return true;
					return !self->queue.empty(); 
					});
				std::cout << id << " go!\n";
				bool ok = self->queue.pop(func);
				if(ok)
					func();
			}

			std::cout << id << " end\n";
		}
	};

public:
	ThreadPool(int m):max(m)
	{
		for (int i = 0; i < max; i++) {
			threads.push_back(std::thread(Worker(this,i)));
		}
	}

	void push(std::function<void()> func)
	{
		queue.push(func);
		m_conditional_lock.notify_one();
	}

	void shutdown() {
		end = true;
		m_conditional_lock.notify_all();

		for (int i = 0; i < max; i++) {
			if (threads[i].joinable()) 
				threads[i].join();
		}
	}

};
