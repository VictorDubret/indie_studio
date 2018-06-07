 /*
** EPITECH PROJECT, 2018
** cpp_indie_studio
** File description:
** Created by sahel.lucas-saoudi@epitech.eu,
*/

#ifndef CPP_INDIE_STUDIO_BOMBUP_HPP
#define CPP_INDIE_STUDIO_BOMBUP_HPP

#include "APowerUp.hpp"

namespace is {
	class BombUp: public APowerUp {
	public:
		// Constructor
		BombUp(my::ItemLocker<std::vector<std::shared_ptr<IEntity>>> &entities,
		my::ItemLocker<my::ThreadPool> &eventManager, nts::ManageIrrlicht &irrlicht);

		// Destructor
		~BombUp() override;

		// Getter

		// Setter

		// Other
		void action(is::ACharacter *character) override;

	protected:

	private:
	};
}

#endif //CPP_INDIE_STUDIO_BOMBUP_HPP
