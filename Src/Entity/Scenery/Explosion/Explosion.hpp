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
			my::ItemLocker<my::ThreadPool> &eventManager, irrl::ManageIrrlicht &irrlicht);

		// Destructor
		~Explosion() override;

		// Getter

		// Setter

		// Other
		void explode() override;
		void collide(IEntity *) override;
		void timer();

		void texture() override;
		void setIsPaused(const bool isPaused);

		private:
		bool _isPaused = false;

		size_t _time = 1;
		ssize_t _startedAt;
		ssize_t _lastTime;

	};
};

#endif //CPP_INDIE_STUDIO_EXPLOSION_HPP
