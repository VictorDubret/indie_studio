/*
** EPITECH PROJECT, 2018
** cpp_indie_studio
** File description:
** Created by sahel.lucas-saoudi@epitech.eu,
*/

#ifndef CPP_INDIE_STUDIO_DEBUG_HPP
#define CPP_INDIE_STUDIO_DEBUG_HPP

# include <iostream>

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

class Debug {
	public:
	// Constructor

	// Destructor

	// Getter

	// Setter

	// Other
	template<typename T>
	static void debug(T arg)
	{
		#ifdef DEBUG
		std::cerr << arg << std::endl;
		#endif
		(void)arg;
	}

	template<typename T, class ...Args>
	static void debug(T t, Args &&... args)
	{
		#ifdef DEBUG
		std::cerr << t;
		#endif
		debug(args...);
		(void)t;
	}

	protected:

	private:
};

#endif //CPP_INDIE_STUDIO_DEBUG_HPP
