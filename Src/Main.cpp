/*
** EPITECH PROJECT, 2018
** test
** File description:
** Created by martin.januario@epitech.eu,
*/

#include <memory>
#include <vector>
#include <cstdlib>
#include <irrlicht.h>
#include <csignal>
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
	/* 1 argument == Jeu Normal, 2 arguments == SplitScreen, 3 arguments == Load sauvegarde */
	my::ThreadPool thpool(100);
	std::vector<std::shared_ptr<is::IEntity>> list;

	my::ItemLocker<my::ThreadPool> pool(thpool);
	my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> lockList(list);

	//nts::ManageIrrlicht tmp(lockList, pool, irr::core::vector2di(50, 50));

	std::pair<std::size_t, std::size_t> mapSize(11,13);
	bool splitScreen = false;
	if (ac == 2) {
		splitScreen = true;
	}

	/* initialisation */
	nts::ManageIrrlicht tmp(lockList, pool, irr::core::vector2di(mapSize.first, mapSize.second), splitScreen);

	if (ac == 1) {
		is::ACharacter *toto = new is::ACharacter(lockList, pool, tmp);
		toto->setZ(1);
		toto->setX(1);
		toto->setBombMax(5);
		toto->setBomb(5);


		is::ACharacter *toto2 = new is::ACharacter(lockList, pool, tmp);
		toto2->setZ(13);
		toto2->setX(13);
		toto2->setBombMax(5);
		toto2->setBomb(5);

		mg::MapGenerator generator(lockList, pool, tmp, mapSize);
	//	is::IEntity *ai = new is::ArtificialIntelligence(lockList, pool, tmp);

	}
	/* Chargement textures */
	tmp.getDriver()->getTexture(irr::io::path("media/fire.jpg"));
	//is::IEntity *ai = new is::ArtificialIntelligence(lockList, pool, tmp);
	//	is::ACharacter *tata = new is::ACharacter (lockList, pool, tmp);

	/*if (ac == 3) {

		std::stringstream streamLine;
		std::string line;
		std::string temp;
		std::ifstream myfile("save.indie");
		if (myfile.is_open()) {
			while (getline(myfile, line)) {
				std::cout << line << std::endl;

				streamLine << line;
				std::vector<std::string> tmpVector;
				while (streamLine >> temp) {
					tmpVector.push_back(temp);
					std::cout << "J split ma commande: " << temp << std::endl;
				}
				streamLine.clear();
				if (tmpVector.size() == 12) {
					if (tmpVector[0] == "Character") {
						std::cout << "J'ai trouvé un character" << std::endl;

						is::IEntity *player2 = new is::ACharacter(lockList, pool, tmp);
						std::shared_ptr<is::IEntity> player_tmp2 = std::shared_ptr<is::IEntity>(player2, [](is::IEntity *) {});

						irr::core::vector3df tmpPos(std::stoi(tmpVector[1]), std::stoi(tmpVector[2]), std::stoi(tmpVector[3]));
						tmp.getNode(player_tmp2)->setPosition(irr::core::vector3df(tmpPos));
						std::cout << "Mon joueur est en :" << tmp.getNode(player_tmp2)->getPosition().X << "][" << tmp.getNode(player_tmp2)->getPosition().Z;

						 IsPickable [0] IsWalkable [0] Iscollidable [1] isWallPassable [0]


					} else if (tmpVector[0] == "Wall") {
						is::IEntity *tmp_data = new is::Wall(lockList, pool, tmp);
						std::shared_ptr<is::IEntity> wall = std::shared_ptr<is::IEntity>(tmp_data, [](is::IEntity *) {});
						setEntity(tmpVector, wall, tmp);
					} else if (tmpVector[0] == "UnbreakableWall") {
						is::IEntity *tmp_data = new is::UnbreakableWall(lockList, pool, tmp);
						std::shared_ptr<is::IEntity> wall = std::shared_ptr<is::IEntity>(tmp_data, [](is::IEntity *) {});
						setEntity(tmpVector, wall, tmp);
					} else if (tmpVector[0] == "Bomb") {



						is::IEntity *tmp_data = new is::Bomb(lockList, pool, player_tmp2, tmp, 5);
						std::shared_ptr<is::IEntity> bomb = std::shared_ptr<is::IEntity>(tmp_data, [](is::IEntity *) {});
						//setEntity(tmpVector, bomb, tmp);
					}

				}
			}
			myfile.close();

		}
	}*/



	tmp.loopDisplay();
	pool->finishAll();
	return 0;
}
