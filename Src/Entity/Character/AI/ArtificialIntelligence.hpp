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
	class ArtificialIntelligence: public ACharacter {
	public:
		ArtificialIntelligence(Entity_t &entities, ThreadPool_t &eventManage,
			 irrl::ManageIrrlicht &irrlicht, std::size_t id = 1);
		~ArtificialIntelligence() override = default;

		void 	AIsTurn();

	private:
		enum Type {
			WALL,
			CRATE,
			BOMB,
			EXPLOSION,
			POWERUP,
			DANGER,
			SAFE
		};

		enum Direction {
			LEFT,
			RIGHT,
			UP,
			DOWN,
			NONE
		};

		bool 	inDanger();
		void	getMapDimensions();
		void 	setWalls();
		void 	setDangerRec(std::size_t pos, std::size_t range, int dir);
		bool 	setDangerRec(std::size_t pos, std::size_t range, int dir,
			std::vector<std::pair<Type, IEntity *>> map);
		int 	getDist(int pos, std::vector<int> map, int dist);
		Direction breadthFirst(int pos, std::vector<int> &map);
		Direction lookForAZone(Type type);
		Direction lookForAZone(Type type, std::vector<std::pair<Type, IEntity *>>map);
		bool	safeBombDrop();
		bool	onCrate();
		void 	move(Direction dir);
		void 	headTowards(Direction dir);
		void 	addDangerZones();
		bool 	addDangerZones(std::vector<std::pair<Type, IEntity *>>map);
		void 	updateMap();

		Entity_t	&_entities;
		std::size_t	_height;
		std::size_t	_width;
		std::vector<std::pair<Type, IEntity *>>	_map;
		std::vector<std::pair<int, int>>	_ennemies;
		std::pair<float, float>	_position;
		std::pair<int, int>	_goal;
	};
}

#endif		/* !AI_LEVEL_ONE_HPP_ */
