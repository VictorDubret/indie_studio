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
