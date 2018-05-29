/*
** EPITECH PROJECT, 2018
** cpp_indie_studio
** File description:
** Created by sahel.lucas-saoudi@epitech.eu,
*/

#ifndef CPP_INDIE_STUDIO_AAI_HPP
#define CPP_INDIE_STUDIO_AAI_HPP

#include "ACharacter.hpp"

namespace is {
	class AAI: public ACharacter {
	public:
		// Constructor
		AAI(my::ItemLocker<std::vector<std::shared_ptr<IEntity>>> &entities, my::ItemLocker<my::ThreadPool> &eventManager);

		// Destructor
		~AAI() override = default;

		// Getter

		// Setter

		// Other


	protected:

	private:
	};
}

#endif //CPP_INDIE_STUDIO_AAI_HPP
