/*
** EPITECH PROJECT, 2018
** test
** File description:
** Created by martin.januario@epitech.eu,
*/

#include <memory>
#include <vector>
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

	/* Création mur et joueur */
	is::IEntity *wall = new is::Wall(lockList, pool, tmp);
	is::IEntity *wall2 = new is::Wall(lockList, pool, tmp);
	is::IEntity *wall3 = new is::Wall(lockList, pool, tmp);
	is::IEntity *wall4 = new is::Wall(lockList, pool, tmp);
	is::IEntity *wall5 = new is::Wall(lockList, pool, tmp);

	is::IEntity *chinaWall = new is::Wall(lockList, pool, tmp);
	is::IEntity *chinaWall2 = new is::Wall(lockList, pool, tmp);
	is::IEntity *chinaWall3 = new is::Wall(lockList, pool, tmp);
	is::IEntity *chinaWall4 = new is::Wall(lockList, pool, tmp);
	is::IEntity *chinaWall5 = new is::Wall(lockList, pool, tmp);
	is::IEntity *chinaWall6 = new is::Wall(lockList, pool, tmp);


	is::IEntity *player2 = new is::ACharacter(lockList, pool, tmp);



	/* Création floor */
	irr::core::dimension2d<irr::f32> tileSize(1.0, 1.0); // taille dun bloc
	irr::core::dimension2d<irr::u32> tileCount(19, 19); // taille de la map
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


	/* Save */
	std::shared_ptr<is::IEntity> bitewall = std::shared_ptr<is::IEntity>(wall, [](is::IEntity *){});
	std::shared_ptr<is::IEntity> bitewall2 = std::shared_ptr<is::IEntity>(wall2, [](is::IEntity *){});
	std::shared_ptr<is::IEntity> bitewall3 = std::shared_ptr<is::IEntity>(wall3, [](is::IEntity *){});
	std::shared_ptr<is::IEntity> bitewall4 = std::shared_ptr<is::IEntity>(wall4, [](is::IEntity *){});
	std::shared_ptr<is::IEntity> bitewall5 = std::shared_ptr<is::IEntity>(wall5, [](is::IEntity *){});

	std::shared_ptr<is::IEntity> japanWall = std::shared_ptr<is::IEntity>(chinaWall, [](is::IEntity *){});
	std::shared_ptr<is::IEntity> japanWall2 = std::shared_ptr<is::IEntity>(chinaWall2, [](is::IEntity *){});
	std::shared_ptr<is::IEntity> japanWall3 = std::shared_ptr<is::IEntity>(chinaWall3, [](is::IEntity *){});
	std::shared_ptr<is::IEntity> japanWall4 = std::shared_ptr<is::IEntity>(chinaWall4, [](is::IEntity *){});
	std::shared_ptr<is::IEntity> japanWall5 = std::shared_ptr<is::IEntity>(chinaWall5, [](is::IEntity *){});
	std::shared_ptr<is::IEntity> japanWall6 = std::shared_ptr<is::IEntity>(chinaWall6, [](is::IEntity *){});

	std::shared_ptr<is::IEntity> player_tmp2 = std::shared_ptr<is::IEntity>(player2, [](is::IEntity *){});

	/* Create cube */
	nts::ManageObject::createCube(tmp, bitewall, 1);
	nts::ManageObject::createCube(tmp, bitewall2, 1);
	nts::ManageObject::createCube(tmp, bitewall3, 1);
	nts::ManageObject::createAnim(tmp, player_tmp2, "media/sydney.md2", 0.8);

	tmp.getNode(bitewall)->setPosition(irr::core::vector3df(2.3, 0, 0));
	tmp.getNode(bitewall3)->setPosition(irr::core::vector3df(3.f, 0, 10));
	tmp.getNode(player_tmp2)->setPosition(irr::core::vector3df(10, 0, 10));
	nts::ManageObject::createCube(tmp, bitewall4, 1);
	nts::ManageObject::createCube(tmp, bitewall5, 1);

	nts::ManageObject::createCube(tmp, japanWall, 1);
	nts::ManageObject::createCube(tmp, japanWall2, 1);
	nts::ManageObject::createCube(tmp, japanWall3, 1);
	nts::ManageObject::createCube(tmp, japanWall4, 1);
	nts::ManageObject::createCube(tmp, japanWall5, 1);
	nts::ManageObject::createCube(tmp, japanWall6, 1);

//	nts::ManageObject::createCube(tmp, player_tmp2, 1);

	/* Set all positions */
	tmp.getNode(bitewall)->setPosition(irr::core::vector3df(3, 0, 3));
	tmp.getNode(bitewall3)->setPosition(irr::core::vector3df(3.f, 0, 1));
	tmp.getNode(bitewall4)->setPosition(irr::core::vector3df(5.f, 0, 5.f));
	tmp.getNode(bitewall5)->setPosition(irr::core::vector3df(7.f, 0, 1.f));

	tmp.getNode(japanWall)->setPosition(irr::core::vector3df(7.f, 0, 1.f));
	tmp.getNode(japanWall2)->setPosition(irr::core::vector3df(8.f, 0, 1.f));
	tmp.getNode(japanWall3)->setPosition(irr::core::vector3df(8.f, 0, 2.f));
	tmp.getNode(japanWall4)->setPosition(irr::core::vector3df(8.f, 0, 3.f));
	tmp.getNode(japanWall5)->setPosition(irr::core::vector3df(8.f, 0, 4.f));
	tmp.getNode(japanWall6)->setPosition(irr::core::vector3df(8.f, 0, 5.f));


	nts::ManageObject::setScale(tmp, player_tmp2, irr::core::vector3df(0.05, 0.05 , 0.05));
	nts::ManageObject::setRotation(tmp, player_tmp2, irr::core::vector3df(0, 90, 0));
	nts::ManageObject::setMaterialLight(tmp, player_tmp2, false);
	nts::ManageObject::setAnimation(tmp, player_tmp2, irr::scene::EMAT_RUN);
	nts::ManageObject::setTexture(tmp, player_tmp2, "media/sydney.bmp");

	tmp.getNode(player_tmp2)->setPosition(irr::core::vector3df(10.f, 0, 10.f));

	/* Set light and texture*/
	nts::ManageObject::setMaterialLight(tmp, bitewall, false);
	nts::ManageObject::setTexture(tmp, bitewall, "media/redSquare.png");
	nts::ManageObject::setMaterialLight(tmp, bitewall3, false);
	nts::ManageObject::setTexture(tmp, bitewall3, "media/redSquare.png");
	nts::ManageObject::setMaterialLight(tmp, bitewall2, false);
	nts::ManageObject::setTexture(tmp, bitewall2, "media/redSquare.png");
	nts::ManageObject::setMaterialLight(tmp, bitewall4, false);
	nts::ManageObject::setTexture(tmp, bitewall4, "media/redSquare.png");
	nts::ManageObject::setMaterialLight(tmp, bitewall5, false);
	nts::ManageObject::setTexture(tmp, bitewall5, "media/redSquare.png");

	nts::ManageObject::setMaterialLight(tmp, japanWall, false);
	nts::ManageObject::setTexture(tmp, japanWall, "media/redSquare.png");
	nts::ManageObject::setMaterialLight(tmp, japanWall2, false);
	nts::ManageObject::setTexture(tmp, japanWall2, "media/redSquare.png");
	nts::ManageObject::setMaterialLight(tmp, japanWall3, false);
	nts::ManageObject::setTexture(tmp, japanWall3, "media/redSquare.png");
	nts::ManageObject::setMaterialLight(tmp, japanWall4, false);
	nts::ManageObject::setTexture(tmp, japanWall4, "media/redSquare.png");
	nts::ManageObject::setMaterialLight(tmp, japanWall5, false);
	nts::ManageObject::setTexture(tmp, japanWall5, "media/redSquare.png");
	nts::ManageObject::setMaterialLight(tmp, japanWall6, false);
	nts::ManageObject::setTexture(tmp, japanWall6, "media/redSquare.png");

	tmp.loopDisplay();
	return 0;
}