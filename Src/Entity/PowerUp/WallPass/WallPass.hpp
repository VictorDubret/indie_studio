/*
** EPITECH PROJECT, 2018
** cpp_indie_studio
** File description:
** Created by sahel.lucas-saoudi@epitech.eu,
*/

#ifndef CPP_INDIE_STUDIO_WALLPASS_HPP
#define CPP_INDIE_STUDIO_WALLPASS_HPP

#include "APowerUp.hpp"

namespace is {
	class WallPass: public APowerUp {
	public:
		// Constructor
		WallPass(my::ItemLocker<std::vector<std::shared_ptr<IEntity>>> &entities,
			my::ItemLocker<my::ThreadPool> &eventManager, nts::ManageIrrlicht &irrlicht);

		// Destructor
		~WallPass() override;

		// Getter

		// Setter

		// Other
		void action(is::ACharacter *character) override;
		//void texture() override;

	protected:

	private:
	};
}

#endif //CPP_INDIE_STUDIO_WALLPASS_HPP
