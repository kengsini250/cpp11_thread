#pragma once

#include	"myQueue.h"
#include <vector>
#include <thread>
#include <future>



class ThreadPool
{
private:
	MyQueue< std::function<void()>> queue;

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
			std::function<void()> func;
			bool ok;
		
			while (!self->end) {
				{
					std::unique_lock<std::mutex> lock(self->lock);
					if (self->queue.empty()) {
						self->m_conditional_lock.wait(lock);
					}
					ok = self->queue.pop(func);
				}

				if (ok) { 
					std::cout << "-->id : " << id << " GO\n";
					func(); 
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

	template <typename F, typename... Args>
	auto push(F&& f, Args &&...args) 
		-> std::shared_future<decltype(f(args...))>
	{
		std::function<decltype(f(args...))()> func = 
			std::bind(
				std::forward<F>(f), 
				std::forward<Args>(args)...
			);

		auto task_ptr = std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);
		std::function<void()> warpper_func = [task_ptr]()
		{
			(*task_ptr)();
		};
		queue.push(warpper_func);
		m_conditional_lock.notify_one();
		return task_ptr->get_future();
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
