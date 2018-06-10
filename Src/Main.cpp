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
	/* 1 argument == Jeu Normal, 2 argumeirrl == SplitScreen, 3 argumeirrl == Load sauvegarde */
	my::ThreadPool thpool(100);
	std::vector<std::shared_ptr<is::IEntity>> list;

	my::ItemLocker<my::ThreadPool> pool(thpool);
	my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> lockList(list);

	//irrl::ManageIrrlicht tmp(lockList, pool, irr::core::vector2di(50, 50));

	std::pair<std::size_t, std::size_t> mapSize(11,13);
	bool splitScreen = false;
	if (ac == 2) {
		splitScreen = true;
	}

	/* initialisation */
	irrl::ManageIrrlicht tmp(lockList, pool, irr::core::vector2di(mapSize.first, mapSize.second), splitScreen);

	/* Chargement textures */
/*	tmp.getDriver()->getTexture(irr::io::path("media/fire.jpg"));
	tmp.getDriver()->getTexture(irr::io::path("media/1_character.png"));
	tmp.getDriver()->getTexture(irr::io::path("media/2_character.png"));
	tmp.getDriver()->getTexture(irr::io::path("media/3_character.png"));
	tmp.getDriver()->getTexture(irr::io::path("media/4_character.png"));
	tmp.getDriver()->getTexture(irr::io::path("media/bomb.png"));
	tmp.getDriver()->getTexture(irr::io::path("media/map_ob000_item_AL.png"));*/
	//	is::ACharacter *tata = new is::ACharacter (lockList, pool, tmp);

	if (ac == 3) {

		is::ACharacter *player2;

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
				if (tmpVector.size() == 12 || tmpVector.size() == 14) {
					if (tmpVector[0] == "Character") {
						std::cout << "J'ai trouvé un character" << std::endl;

						player2 = new is::ACharacter(lockList, pool, tmp);
						std::shared_ptr<is::IEntity> player_tmp2 = std::shared_ptr<is::IEntity>(player2, [](is::IEntity *) {});

						irr::core::vector3df tmpPos(std::stoi(tmpVector[1]), std::stoi(tmpVector[2]), std::stoi(tmpVector[3]));
						tmp.getNode(player_tmp2.get())->setPosition(irr::core::vector3df(tmpPos));
						std::cout << "Mon joueur est en :" << tmp.getNode(player_tmp2.get())->getPosition().X << "][" << tmp.getNode(player_tmp2.get())->getPosition().Z;



					} else if (tmpVector[0] == "Wall") {
						is::IEntity *tmp_data = new is::Wall(lockList, pool, tmp);
						std::shared_ptr<is::IEntity> wall = std::shared_ptr<is::IEntity>(tmp_data, [](is::IEntity *) {});
						setEntity(tmpVector, wall, tmp);
					} else if (tmpVector[0] == "UnbreakableWall") {
						is::IEntity *tmp_data = new is::UnbreakableWall(lockList, pool, tmp);
						std::shared_ptr<is::IEntity> wall = std::shared_ptr<is::IEntity>(tmp_data, [](is::IEntity *) {});
						setEntity(tmpVector, wall, tmp);
					} else if (tmpVector[0] == "SpeedUp") {
						is::IEntity *tmp_data = new is::SpeedUp(lockList, pool, tmp);
						std::shared_ptr<is::IEntity> wall = std::shared_ptr<is::IEntity>(tmp_data, [](is::IEntity *) {});
						setEntity(tmpVector, wall, tmp);
					} else if (tmpVector[0] == "WallPass") {
						is::IEntity *tmp_data = new is::WallPass(lockList, pool, tmp);
						std::shared_ptr<is::IEntity> wall = std::shared_ptr<is::IEntity>(tmp_data, [](is::IEntity *) {});
						setEntity(tmpVector, wall, tmp);
					} else if (tmpVector[0] == "BombUp") {
						is::IEntity *tmp_data = new is::BombUp(lockList, pool, tmp);
						std::shared_ptr<is::IEntity> wall = std::shared_ptr<is::IEntity>(tmp_data, [](is::IEntity *) {});
						setEntity(tmpVector, wall, tmp);
					} else if (tmpVector[0] == "FireUp") {
						is::IEntity *tmp_data = new is::FireUp(lockList, pool, tmp);
						std::shared_ptr<is::IEntity> wall = std::shared_ptr<is::IEntity>(tmp_data, [](is::IEntity *) {});
						setEntity(tmpVector, wall, tmp);
					} else if (tmpVector[0] == "Bomb") {

						is::IEntity *tmp_data = new is::Bomb(lockList, pool, reinterpret_cast<std::shared_ptr<is::IEntity> &>(player2), tmp);
						std::shared_ptr<is::IEntity> wall = std::shared_ptr<is::IEntity>(tmp_data, [](is::IEntity *) {});
						setEntity(tmpVector, wall, tmp);

						std::cout << "Ma bombe est en :" << tmp.getNode(wall.get())->getPosition().X << "][" << tmp.getNode(wall.get())->getPosition().Z;

					//	is::IEntity *tmp_data = new is::Bomb(lockList, pool, player_tmp2, tmp, 5);
					//	std::shared_ptr<is::IEntity> bomb = std::shared_ptr<is::IEntity>(tmp_data, [](is::IEntity *) {});
						//setEntity(tmpVector, bomb, tmp);
					}

				}
			}
			myfile.close();

		}
	}



	tmp.loopDisplay();
	pool->finishAll();
	return 0;
}
