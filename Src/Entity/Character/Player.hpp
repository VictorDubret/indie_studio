/*
** EPITECH PROJECT, 2018
** cpp_indie_studio
** File description:
** Created by sahel.lucas-saoudi@epitech.eu,
*/

#ifndef CPP_INDIE_STUDIO_PLAYER_HPP
#define CPP_INDIE_STUDIO_PLAYER_HPP

#include <EventManagement/ThreadPool.hpp>
#include "ACharacter.hpp"

namespace is {
	class Player: is::ACharacter {
		public:
		// Constructor
		Player(std::vector<std::shared_ptr<IEntity>> &entities, my::ThreadPool &eventManager);

		// Destructor
		~Player() = default;

		// Getter

		// Setter

		// Other


		protected:

		private:
	};
}

#endif //CPP_INDIE_STUDIO_PLAYER_HPP
