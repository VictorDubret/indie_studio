/*
** EPITECH PROJECT, 2018
** cpp_indie_studio
** File description:
** Created by guillaume.berdeaux@epitech.eu
*/

#ifndef CPP_INDIE_STUDIO_UNBREAKABLEWALL_HPP
#define CPP_INDIE_STUDIO_UNBREAKABLEWALL_HPP

#include "AEntity.hpp"
#include "ACharacter.hpp"

namespace is {
	class UnbreakableWall: public AEntity {
	public:
		//Constructor
		UnbreakableWall(my::ItemLocker<std::vector<std::shared_ptr<IEntity>>> &entities,
				my::ItemLocker<my::ThreadPool> &eventManager, nts::ManageIrrlicht &irrlicht);

		// Destructor
		~UnbreakableWall() override = default;

		// Getter

		// Setter

		// Other

	private:
	};
};

#endif //CPP_INDIE_STUDIO_UNBREAKABLEWALL_HPP
