//
// EPITECH PROJECT, 2018
// Indie Studio
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
			irrl::ManageIrrlicht &irrlicht, size_t time = 2
		);

		~Bomb() override;

		void explode() override;

		void timer();

		void texture() override;

		size_t getLenExplosion() const;

		void setPaused(const bool isPaused);

	private:

		bool isWalkable(std::shared_ptr<is::IEntity> &entity) override;

		typedef enum {
			XAXES = 0,
			ZAXES = 1
		} Axes_t;

		size_t _lenExplosion = 1;

		bool check_arround(int lenExplosion, int actualPos, std::function<float(int)> f, Axes_t which_axes, float x, float z);

		void createExplosion(std::function<float(int)> &f, Axes_t &which_axes, int &actualPos, float x_bomb, float z_bomb);

		std::shared_ptr<IEntity> &_player;
		void doExplosions(float x, float z);

		bool _isPaused = false;
		bool _explosed = false;

		size_t _time = 2;
		long int _startedAt = 0;
		long int _lastTime = 0;
	};
}

#endif //CPP_INDIE_STUDIO_BOMB_HPP
