/*
** EPITECH PROJECT, 2018
** cpp_indie_studio
** File description:
** Created by Sahel Lucas--Saoudi
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