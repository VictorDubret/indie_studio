/*
** EPITECH PROJECT, 2018
** cpp_indie_studio
** File description:
** Created by sahel.lucas-saoudi@epitech.eu,
*/

#ifndef CPP_INDIE_STUDIO_APOWERUP_HPP
#define CPP_INDIE_STUDIO_APOWERUP_HPP

#include "AEntity.hpp"
#include "ACharacter.hpp"

namespace is {
	class APowerUp: public AEntity {
	public:
		// Constructor
		APowerUp(my::ItemLocker<std::vector<std::shared_ptr<IEntity>>> &entities,
			my::ItemLocker<my::ThreadPool> &eventManager);

		// Destructor
		~APowerUp() override = default;

		// Getter

		// Setter

		// Other
		void collide(is::IEntity *entity) override;
	protected:
		virtual void action(is::ACharacter *entity);

	private:
	};
}

#endif //CPP_INDIE_STUDIO_APOWERUP_HPP
