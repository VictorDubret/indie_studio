/*
** EPITECH PROJECT, 2018
** cpp_plazza
** File description:
** Created by anais.breant@epitech.eu,
*/

#include "Thread.hpp"

my::Thread::~Thread()
{
	if (joinable()) {
		join();
	}
}

void my::Thread::join()
{
	_thread.join();
}

bool my::Thread::joinable()
{
	return _thread.joinable();
}

void my::Thread::detach()
{
	_thread.detach();
}

std::thread::id my::Thread::getId() const
{
	return _thread.get_id();
}