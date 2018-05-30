//
// Created by victor on 28/05/18.
// File description:
// timer.hpp
//

#ifndef CPP_INDIE_STUDIO_TIMER_HPP
#define CPP_INDIE_STUDIO_TIMER_HPP

#include <cstddef>

class Timer {
	typedef std::chrono::time_point<std::chrono::system_clock> chrono_t;

	public:
	Timer();
	~Timer() = default;

	void startTimer(size_t time);
	bool isOver();
	void timeElapse();

	private:
	bool _over = false;
	size_t _time;
	chrono_t  _beginTime;

};

#endif //CPP_INDIE_STUDIO_TIMER_HPP
