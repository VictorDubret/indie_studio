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
#include "Bomb.hpp"
#include "ThreadPool.hpp"
#include "ItemLocker.hpp"
#include "ManageIrrlicht.hpp"
#include "ManageObject.hpp"
#include "ACharacter.hpp"
#include "Wall.hpp"

int main()
{
	my::ThreadPool thpool(100);
	std::vector<std::shared_ptr<is::IEntity>> list;

	my::ItemLocker<my::ThreadPool> pool(thpool);
	my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> lockList(list);

	nts::ManageIrrlicht tmp(lockList, pool, irr::core::vector2di(50, 50));

	is::IEntity *player2 = new is::ACharacter(lockList, pool, tmp);


	std::pair<std::size_t, std::size_t> mapSize(11,13);

	/* Création floor */
	irr::core::dimension2d<irr::f32> tileSize(1.0, 1.0); // taille dun bloc
	irr::core::dimension2d<irr::u32> tileCount(mapSize.first, mapSize.second); // taille de la map
	auto material = new irr::video::SMaterial();
	material->MaterialType = irr::video::E_MATERIAL_TYPE::EMT_SOLID;

	irr::f32 hillHeight = 0;
	irr::core::dimension2d<irr::f32> countHills(0.0, 0.0);
	irr::core::dimension2d<irr::f32> textureRepeatCount(1.0, 1.0);
	material->Wireframe = false;
	material->Lighting = false;
	irr::scene::IMesh *cube = tmp.getSceneManager()->getGeometryCreator()->createPlaneMesh(
			tileSize,
			tileCount,
			material,
//			hillHeight,
//			countHills,
			textureRepeatCount);
	material->ColorMaterial = irr::video::E_COLOR_PLANE::ECP_BLUE;


	cube->setMaterialFlag(irr::video::EMF_WIREFRAME, false);

	irr::video::ITexture *texture = tmp.getDriver()->getTexture(irr::io::path("media/earth.jpg"));

	irr::scene::IMeshSceneNode *cubeNode = tmp.getSceneManager()->addMeshSceneNode(cube);
	cubeNode->setMaterialTexture(0, texture);
	cubeNode->setPosition(irr::core::vector3df(0, -1.0f, 0));
	cubeNode->setMaterialFlag(irr::video::EMF_WIREFRAME, false);
	cubeNode->setMaterialFlag(irr::video::EMF_LIGHTING, false);

	//cubeNode->setMaterialType(irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF);
	//cubeNode->getMaterial(0).setTexture(0, tmp.getDriver()->getTexture(texture));


	std::shared_ptr<is::IEntity> player_tmp2 = std::shared_ptr<is::IEntity>(player2, [](is::IEntity *){});

	/* Create cube */

//	nts::ManageObject::createCube(tmp, player_tmp2, 1);

	/* Set all positions */


	/* Set light and texture*/

	srand(time(NULL));
	mg::MapGenerator generator(lockList, pool, tmp, mapSize);

	tmp.loopDisplay();
	return 0;
}