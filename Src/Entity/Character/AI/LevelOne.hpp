//
// EPITECH PROJECT, 2018
// Indie Studio
// File description:
// AI class
//

#ifndef		AI_LEVEL_ONE_HPP_
#define		AI_LEVEL_ONE_HPP_

#include	"ACharacter.hpp"
#include 	"ManageIrrlicht.hpp"

namespace is {
	class LevelOne: public ACharacter {
	public:
		LevelOne(Entity_t &entities, ThreadPool_t &eventManage,
			 nts::ManageIrrlicht &irrlicht);
		~LevelOne() override = default;

		void 	beAnAI();

	private:
		enum Type {
			WALL,
			CRATE,
			BOMB,
			POWERUP,
			DANGER,
			SAFE
		};

		bool 	stillCrates();
		bool 	inDanger();
		void	getMapDimensions();
		void 	setWalls();
		void 	setDangerRec(std::size_t pos, int range, int dir);
		void 	setDanger(std::size_t pos, int range);
		void 	goToSafeSpot();
		void 	addDangerZones();
		void 	updateMap();

		Entity_t	&_entities;
		std::size_t	_height;
		std::size_t	_width;
		std::vector<std::pair<Type, IEntity *>>	_map;
	};
}

#endif		/* !AI_LEVEL_ONE_HPP_ */
