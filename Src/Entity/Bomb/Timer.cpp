//
// EPITECH PROJECT, 2018
// Indie Studio
// File description:
// Timer.cpp
//

#include <ctime>
#include <chrono>
#include <iostream>
#include "Timer.hpp"

Timer::Timer()
{
}

void Timer::startTimer(size_t time)
{
	_time = time * 1000;
	_beginTime = std::chrono::system_clock::now();
}

void Timer::timeElapse()
{
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now() - _beginTime).count()
		<< std::endl;
}

bool Timer::isOver()
{
	size_t elapsedTime = static_cast<size_t >(std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now() - _beginTime).count());
	return elapsedTime >= _time;
}
