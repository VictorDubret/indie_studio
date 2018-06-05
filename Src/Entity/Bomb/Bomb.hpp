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

		~Bomb() override;

		void explode() override;

		void timer(size_t time);

		void texture() override;

		private:

		bool isWalkable(std::shared_ptr<is::IEntity> &entity) const override;

		typedef enum {
			XAXES = 0,
			ZAXES = 1
		} Axes_t;

		size_t _lenExplosion = 1;

		bool _stopTimer = false;

		bool check_arround(int lenExplosion, int actualPos, std::function<float(int)> f, Axes_t which_axes);

		void createExplosion(std::function<float(int)> &f, Axes_t &which_axes, int &actualPos);

		std::shared_ptr<IEntity> &_player;
		void doExplosions();
	};
}

#endif //CPP_INDIE_STUDIO_BOMB_HPP
