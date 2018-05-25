/*
** EPITECH PROJECT, 2018
** cpp_indie_studio
** File description:
** Created by sahel.lucas-saoudi@epitech.eu,
*/

#ifndef CPP_INDIE_STUDIO_ABOMB_HPP
#define CPP_INDIE_STUDIO_ABOMB_HPP

#include <AEntity.hpp>

namespace is {
	class ABomb: is::AEntity {
	public:
		// Constructor
		ABomb(std::vector<IEntity *> &entities, IEntity *parent);

		// Destructor
		~ABomb() = default;

		// Getter

		// Setter

		// Other


	protected:
		std::shared_ptr<IEntity> _parent;
	private:
	};
}

#endif //CPP_INDIE_STUDIO_ABOMB_HPP
