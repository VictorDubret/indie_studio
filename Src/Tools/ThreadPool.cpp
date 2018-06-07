/*
** EPITECH PROJECT, 2018
** cpp_plazza
** File description:
** Created by sahel.lucas-saoudi@epitech.eu,
*/

#include <csignal>
#include "ThreadPool.hpp"

my::ThreadPool::ThreadPool(unsigned int threadNumber)
{
	for (unsigned int i = 0; i < threadNumber; i++) {
		std::shared_ptr<my::Thread> thread(new my::Thread([this](){
			while (true) {
				std::function<void(void)> task;
				{
					task = std::function<void(void)>();
					std::unique_lock<std::mutex> lock(
						_mutex);
					_cond.wait(lock, [this]() {
						return _stop ||
							!_tasks.empty();
					});
					if (_stop)
						break;
					if (_tasks.front()) {
						task = std::move(_tasks.front());
						_tasks.pop();
					}
				}
				if (task) {
					task();
				}
			}
			return false;
		}));
		_threads.push_back(thread);
	}
}

my::ThreadPool::~ThreadPool()
{
	_stop = true;
	_cond.notify_all();
	for (auto &it: _threads) {
		if (it->joinable())
			it->join();
	}
}



bool my::ThreadPool::empty() const
{
	return _tasks.empty();
}

unsigned long my::ThreadPool::getEnqueuedTaskNumber() const
{
	return _tasks.size();
}

void my::ThreadPool::finishAll()
{
	_stop = true;
	_cond.notify_all();
	for (auto &it: _threads) {
		if (it->joinable())
			it->join();
	}
}
