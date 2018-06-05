/*
** EPITECH PROJECT, 2018
** cpp_plazza
** File description:
** Created by sahel.lucas-saoudi@epitech.eu,
*/

#ifndef CPP_INDIESTUDIO_THREADPOOL_HPP
#define CPP_INDIESTUDIO_THREADPOOL_HPP

#include <csignal>
#include <vector>
#include <condition_variable>
#include <queue>
#include <mutex>
#include "Thread.hpp"

namespace my {
	class ThreadPool {
		public:
		// Constructor
		explicit ThreadPool(unsigned int threadNumber);

		// Destructor
		~ThreadPool();

		// Getter

		// Setter
		template<class Callable, class... Args>
		void enqueue(Callable &&func, Args &&... args)
		{
			signal(SIGSEGV, SIG_IGN);
			auto f = std::bind(std::forward<Callable>(func),
				std::forward<Args>(args)...);

			_tasks.push(new std::function<void(void)>([f] {
				f();
			}));
			_cond.notify_one();
			signal(SIGSEGV, nullptr);
		};

		// Other
		bool empty() const;

		unsigned long getEnqueuedTaskNumber() const;

		protected:

		private:
		bool _stop = false;
		std::mutex _mutex;

		std::condition_variable _cond;
		std::queue<std::function<void(void)> *> _tasks;
		std::vector<std::shared_ptr<my::Thread>> _threads;
	};
}

#endif //CPP_INDIESTUDIO_THREADPOOL_HPP
