/*
** EPITECH PROJECT, 2018
** cpp_indie_studio
** File description:
** Created by guillaume.berdeaux@epitech.eu
*/

#ifndef CPP_INDIE_STUDIO_EXPLOSION_HPP
# define CPP_INDIE_STUDIO_EXPLOSION_HPP

# include "AEntity.hpp"
# include "ACharacter.hpp"

namespace is {
	class Explosion: public AEntity {
	public:
		//Constructor
		Explosion(my::ItemLocker<std::vector<std::shared_ptr<IEntity>>> &entities,
			my::ItemLocker<my::ThreadPool> &eventManager);

		// Destructor
		~Explosion() override = default;

		// Getter

		// Setter

		// Other
		void explode() override;
		void collide(IEntity *) override;

	private:
	};
};

#endif //CPP_INDIE_STUDIO_EXPLOSION_HPP
