/*
** EPITECH PROJECT, 2018
** test
** File description:
** Created by martin.januario@epitech.eu,
*/

#include <memory>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <irrlicht.h>
#include <csignal>
#include <Entity/PowerUp/SpeedUp/SpeedUp.hpp>
#include <Entity/PowerUp/WallPass/WallPass.hpp>
#include <Entity/PowerUp/BombUp/BombUp.hpp>
#include <Entity/PowerUp/FireUp/FireUp.hpp>
#include "MapGenerator.hpp"
#include "Bomb.hpp"
#include "ThreadPool.hpp"
#include "ItemLocker.hpp"
#include "ManageIrrlicht.hpp"
#include "ManageObject.hpp"
#include "ACharacter.hpp"
#include "AAI.hpp"
#include "ArtificialIntelligence.hpp"
#include "Wall.hpp"
#include "UnbreakableWall.hpp"

void setEntity(const std::vector<std::string> &tmpVector, const std::shared_ptr<is::IEntity> &player_tmp2, irrl::ManageIrrlicht &tmp)
{
	irr::core::vector3df tmpPos(std::stof(tmpVector[1]), std::stof(tmpVector[2]), std::stof(tmpVector[3]));
	tmp.getNode(player_tmp2.get())->setPosition(irr::core::vector3df(tmpPos));
//	std::cout << "Mon entite est en :" << tmp.getNode(player_tmp2.get())->getPosition().X << "][" << tmp.getNode(player_tmp2.get())->getPosition().Z;


	// IsPickable [0] IsWalkable [0] Iscollidable [1] isWallPassable [0]
	if (tmpVector[4] == "IsPickable")
		player_tmp2.get()->setPickable(static_cast<bool>(stoi(tmpVector[5])));
	if (tmpVector[6] == "IsWalkable")
		player_tmp2.get()->setWalkable(static_cast<bool>(stoi(tmpVector[7])));
	if (tmpVector[8] == "Iscollidable")
		player_tmp2.get()->setCollidable(static_cast<bool>(stoi(tmpVector[9])));
	if (tmpVector[10] == "IsWallPassable")
		player_tmp2.get()->setWallPassable(static_cast<bool>(stoi(tmpVector[11])));
	if (tmpVector.size() == 14) {
		auto isWall = dynamic_cast<is::Wall *>(player_tmp2.get());
		if (isWall != nullptr)
			isWall->setPowerUp(tmpVector[13][0]);
	}
}

int main(int ac, char **)
{
	my::ThreadPool thpool(100);
	std::vector<std::shared_ptr<is::IEntity>> list;
	my::ItemLocker<my::ThreadPool> pool(thpool);
	my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> lockList(list);
	std::pair<std::size_t, std::size_t> mapSize(11,13);
	bool splitScreen = false;
	if (ac == 2) {
		splitScreen = true;
	}
	irrl::ManageIrrlicht tmp(lockList, pool, irr::core::vector2di(mapSize.first, mapSize.second), splitScreen);
	tmp.loopDisplay();
	lockList.unlock();
	pool->finishAll();
	return 0;
}
