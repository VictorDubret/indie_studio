//
// Created by victor on 27/05/18.
// File description:
// Bomb.hpp
//

#ifndef CPP_INDIE_STUDIO_BOMB_HPP
#define CPP_INDIE_STUDIO_BOMB_HPP

#include "AEntity.hpp"

namespace is {

	class Bomb : public AEntity {
		public:
		Bomb(my::ItemLocker<std::vector<std::shared_ptr<IEntity>>> &entities,
			my::ItemLocker<my::ThreadPool> &eventManager, nts::ManageIrrlicht &irrlicht);
		~Bomb() = default;

		void explode();
		void timer(size_t time);
		private:
		size_t _len = 2;

	};
}

#endif //CPP_INDIE_STUDIO_BOMB_HPP
