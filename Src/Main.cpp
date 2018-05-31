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

	nts::ManageIrrlicht tmp(lockList, pool);
	is::IEntity *wall = new is::Wall(lockList, pool, tmp);
	is::IEntity *wall2 = new is::Wall(lockList, pool, tmp);
	is::IEntity *wall3 = new is::Wall(lockList, pool, tmp);
	is::IEntity *player2 = new is::ACharacter(lockList, pool, tmp);

	std::shared_ptr<is::IEntity> bitewall = std::shared_ptr<is::IEntity>(wall, [](is::IEntity *){});
	std::shared_ptr<is::IEntity> bitewall2 = std::shared_ptr<is::IEntity>(wall2, [](is::IEntity *){});
	std::shared_ptr<is::IEntity> bitewall3 = std::shared_ptr<is::IEntity>(wall3, [](is::IEntity *){});
	std::shared_ptr<is::IEntity> player_tmp2 = std::shared_ptr<is::IEntity>(player2, [](is::IEntity *){});

	nts::ManageObject::createCube(tmp, bitewall, 1);
	nts::ManageObject::createCube(tmp, bitewall2, 1);
	nts::ManageObject::createCube(tmp, bitewall3, 1);
	nts::ManageObject::createAnim(tmp, player_tmp2, "media/sydney.md2", 0.9);

	tmp.getNode(bitewall)->setPosition(irr::core::vector3df(2.3, 0, 0));
	tmp.getNode(bitewall3)->setPosition(irr::core::vector3df(3.f, 0, 10));
	tmp.getNode(player_tmp2)->setPosition(irr::core::vector3df(10, 0, 10));

	nts::ManageObject::setScale(tmp, player_tmp2, irr::core::vector3df(0.05, 0.05 , 0.05));
	nts::ManageObject::setRotation(tmp, player_tmp2, irr::core::vector3df(0, 90, 0));
	nts::ManageObject::setMaterialLight(tmp, player_tmp2, false);
	nts::ManageObject::setAnimation(tmp, player_tmp2, irr::scene::EMAT_RUN);
	nts::ManageObject::setTexture(tmp, player_tmp2, "media/sydney.bmp");

	nts::ManageObject::setMaterialLight(tmp, bitewall, false);
	nts::ManageObject::setTexture(tmp, bitewall, "media/rockwall.jpg");
	nts::ManageObject::setMaterialLight(tmp, bitewall3, false);
	nts::ManageObject::setTexture(tmp, bitewall3, "media/rockwall.jpg");
	nts::ManageObject::setMaterialLight(tmp, bitewall2, false);
	nts::ManageObject::setTexture(tmp, bitewall2, "media/rockwall.jpg");

	tmp.loopDisplay();
	return 0;
}