/*
** EPITECH PROJECT, 2018
** cpp_indie_studio
** File description:
** Created by sahel.lucas-saoudi@epitech.eu,
*/

#ifndef CPP_INDIE_STUDIO_DEBUG_HPP
#define CPP_INDIE_STUDIO_DEBUG_HPP

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
	}

	template<typename T, class ...Args>
	static void debug(T t, Args &&... args)
	{
		#ifdef DEBUG
		std::cerr << t;
		debug(args...);
		#endif

	}

	protected:

	private:
};

#endif //CPP_INDIE_STUDIO_DEBUG_HPP
