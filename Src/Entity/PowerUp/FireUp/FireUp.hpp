/*
** EPITECH PROJECT, 2018
** cpp_indie_studio
** File description:
** Created by sahel.lucas-saoudi@epitech.eu,
*/

#ifndef CPP_INDIE_STUDIO_FIREUP_HPP
#define CPP_INDIE_STUDIO_FIREUP_HPP

#include "APowerUp.hpp"

namespace is {
	class FireUp: public APowerUp {
	public:
		// Constructor
		FireUp(my::ItemLocker<std::vector<std::shared_ptr<IEntity>>> &entities,
			my::ItemLocker<my::ThreadPool> &eventManager);

		// Destructor
		~FireUp() override = default;

		// Getter

		// Setter

		// Other

		void action(is::ACharacter *character) override;
	protected:

	private:
	};
}

#endif //CPP_INDIE_STUDIO_FIREUP_HPP