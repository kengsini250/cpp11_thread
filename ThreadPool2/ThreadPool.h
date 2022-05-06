#pragma once

#include	"myQueue.h"
#include <vector>
#include <thread>
#include <future>

class ThreadPool
{
private:
	MyQueue<Runnable*> queue;
	std::vector<std::thread> threads;
	std::mutex lock;
	std::condition_variable m_conditional_lock;
	volatile bool end = false;
	int max = 0;

	class Worker{
		int id;
		ThreadPool* self;
	public:
		Worker(ThreadPool* s,int _id)
			:self(s),id(_id) {
		}


		void operator()() {
		
			while (!self->end) {
				std::unique_lock<std::mutex> lock(self->lock);
				self->m_conditional_lock.wait(lock, [&] {
					if (self->end == true) return true;
					return !self->queue.empty(); 
					});
				Runnable* func;
				bool ok = self->queue.pop(func);
				if (ok) {
					func->run(); 
					delete func;
				}
			}
		}
	};

public:
	ThreadPool(int m):max(m)
	{
		for (int i = 0; i < max; i++) {
			threads.push_back(std::thread(Worker(this,i)));
		}
	}

	void push(Runnable*r) {
		queue.push(r);
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
