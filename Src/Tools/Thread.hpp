/*
** EPITECH PROJECT, 2018
** cpp_plazza
** File description:
** Created by sahel.lucas-saoudi@epitech.eu,
*/

#ifndef INDIE_THREAD_HPP
# define INDIE_THREAD_HPP

#include <functional>
#include <thread>
#include <iostream>

namespace my {

	class Thread {
		public:
		// Constructor
		template<class Function, class... Args>
		explicit Thread(Function &&f, Args &&... args) {
			_thread = std::thread(f, args...);
		}

		// Destructor
		~Thread();

		// Getter
		std::thread::id getId() const;

		// Other
		void join();
		bool joinable();
		void detach();

		private:
		std::thread _thread;
	};
}

#endif /* !INDIE_THREAD_HPP */
