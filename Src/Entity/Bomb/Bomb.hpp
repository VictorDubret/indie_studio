//
// Created by victor on 27/05/18.
// File description:
// Bomb.hpp
//

#ifndef CPP_INDIE_STUDIO_BOMB_HPP
#define CPP_INDIE_STUDIO_BOMB_HPP

#include "ACharacter.hpp"
#include "AEntity.hpp"

namespace is {

	class Bomb : public AEntity {
		public:
		Bomb(my::ItemLocker<std::vector<std::shared_ptr<IEntity>>> &entities,
			my::ItemLocker<my::ThreadPool> &eventManager,
			std::shared_ptr<IEntity> &Player,
			nts::ManageIrrlicht &irrlicht, size_t time = 2
		);

		~Bomb() = default;

		void explode() override;

		void timer(size_t time);

		void texture() override;
		private:

		int _lenExplosion = 1;

		bool _stopTimer = false;

		std::shared_ptr<IEntity> &_player;
	};
}

#endif //CPP_INDIE_STUDIO_BOMB_HPP
