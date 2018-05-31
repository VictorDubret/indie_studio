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
			std::shared_ptr<ACharacter> Player, size_t time = 2
		);

		~Bomb() = default;

		void explode();

		void timer(size_t time);

		private:

		int _lenExplosion = 2;

		std::shared_ptr<ACharacter> _player;

		std::vector<std::shared_ptr<is::IEntity>> getEntitesInRange(int from, int to);

	};
}

#endif //CPP_INDIE_STUDIO_BOMB_HPP
