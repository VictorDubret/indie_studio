/*
** EPITECH PROJECT, 2018
** cpp_indie_studio
** File description:
** Created by guillaume.berdeaux@epitech.eu
*/

#ifndef CPP_INDIE_STUDIO_WALL_HPP
#define CPP_INDIE_STUDIO_WALL_HPP

#include "AEntity.hpp"
#include "ACharacter.hpp"

namespace is {
	class Wall: public AEntity {
	public:
		//Constructor
		Wall(my::ItemLocker<std::vector<std::shared_ptr<IEntity>>> &entities,
			my::ItemLocker<my::ThreadPool> &eventManager, irrl::ManageIrrlicht &irrlicht);

		// Destructor
		~Wall() override;

		// Getter
		char getPowerUp() const;
		// Setter

		// Other
		void explode() override;
		void setPowerUp(char powerUp);
		void texture() override;
	private:
		void placePowerUp(irr::core::vector3df );
		char _powerUp = 0;
	};
};

#endif //CPP_INDIE_STUDIO_WALL_HPP
