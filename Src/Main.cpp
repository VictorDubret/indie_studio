/*
** EPITECH PROJECT, 2018
** test
** File description:
** Created by martin.januario@epitech.eu,
*/

#include <memory>
#include <vector>
#include <MapGenerator/MapGenerator.hpp>
#include <cstdlib>
#include <irrlicht.h>
#include "Bomb.hpp"
#include "ThreadPool.hpp"
#include "ItemLocker.hpp"
#include "ManageIrrlicht.hpp"
#include "ManageObject.hpp"
#include "ACharacter.hpp"
#include "Wall.hpp"

int main(int ac, char **)
{
	my::ThreadPool thpool(100);
	std::vector<std::shared_ptr<is::IEntity>> list;

	my::ItemLocker<my::ThreadPool> pool(thpool);
	my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> lockList(list);

	bool splitScreen = false;
	if (ac > 1) {
		splitScreen = true;
	}

	/* initialisation */
	std::pair<std::size_t, std::size_t> mapSize(8,8);
	nts::ManageIrrlicht tmp(lockList, pool, irr::core::vector2di(mapSize.first + 2, mapSize.second + 2), splitScreen);

	//auto a = new is::Wall(lockList, pool, tmp);
	//a->setX(5);
	//a->setZ(5);

	mg::MapGenerator generator(lockList, pool, tmp, mapSize);

	new is::ACharacter(lockList, pool, tmp);

	/* Création floor */
	irr::core::dimension2d<irr::f32> tileSize(1.0, 1.0); // taille dun bloc
	irr::core::dimension2d<irr::u32> tileCount(mapSize.first + 2, mapSize.second + 2); // taille de la map
	auto material = new irr::video::SMaterial();
	material->MaterialType = irr::video::E_MATERIAL_TYPE::EMT_SOLID;

	irr::core::dimension2d<irr::f32> textureRepeatCount(1.0, 1.0);
	material->Wireframe = false;
	material->Lighting = false;
	irr::scene::IMesh *cube = tmp.getSceneManager()->getGeometryCreator()->createPlaneMesh(tileSize, tileCount, material, textureRepeatCount);
	material->ColorMaterial = irr::video::E_COLOR_PLANE::ECP_BLUE;
	cube->setMaterialFlag(irr::video::EMF_WIREFRAME, false);
	irr::video::ITexture *texture = tmp.getDriver()->getTexture(irr::io::path("media/earth.jpg"));

	irr::scene::IMeshSceneNode *cubeNode = tmp.getSceneManager()->addMeshSceneNode(cube);
	cubeNode->setMaterialTexture(0, texture);
	cubeNode->setPosition(irr::core::vector3df(mapSize.first / 2, -1.0f, mapSize.second / 2));
	cubeNode->setMaterialFlag(irr::video::EMF_WIREFRAME, false);
	cubeNode->setMaterialFlag(irr::video::EMF_LIGHTING, false);


	/* Create cube */

//	nts::ManageObject::createCube(tmp, player_tmp2, 1);

	/* Set all positions */


	/* Set light and texture*/

	std::cout << "Jarrive avant loopDisplay" << std::endl;
	tmp.loopDisplay();
	//lockList->clear();
	return 0;
}