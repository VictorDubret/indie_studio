/*
** EPITECH PROJECT, 2018
** test
** File description:
** Created by martin.januario@epitech.eu,
*/

#include <memory>
#include <vector>
#include <Bomb.hpp>
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
	is::IEntity *player2 = new is::ACharacter(lockList, pool, tmp);
	is::IEntity *wall = new is::Wall(lockList, pool, tmp);
	is::IEntity *wall2 = new is::Wall(lockList, pool, tmp);
	is::IEntity *wall3 = new is::Wall(lockList, pool, tmp);

	std::shared_ptr<is::IEntity> bitewall = std::shared_ptr<is::IEntity>(wall, [](is::IEntity *){});
	std::shared_ptr<is::IEntity> bitewall2 = std::shared_ptr<is::IEntity>(wall2, [](is::IEntity *){});
	std::shared_ptr<is::IEntity> bitewall3 = std::shared_ptr<is::IEntity>(wall3, [](is::IEntity *){});
	std::shared_ptr<is::IEntity> player_tmp2 = std::shared_ptr<is::IEntity>(player2, [](is::IEntity *){});
	std::shared_ptr<is::IEntity> bomb = std::shared_ptr<is::IEntity>(new is::Bomb(lockList, pool, player_tmp2, tmp));


	nts::ManageObject::createCube(tmp, bitewall, 1);
	nts::ManageObject::createCube(tmp, bitewall2, 1);
	nts::ManageObject::createCube(tmp, bitewall3, 1);
	nts::ManageObject::createCube(tmp, player_tmp2, 1);
	nts::ManageObject::createCube(tmp, bomb, 1);
	tmp.getNode(bitewall)->setPosition(irr::core::vector3df(2.3, 0, 0));
	tmp.getNode(bitewall3)->setPosition(irr::core::vector3df(3.f, 0, 10));
	tmp.getNode(player_tmp2)->setPosition(irr::core::vector3df(10, 0, 10));
	tmp.getNode(bomb)->setPosition(irr::core::vector3df(2.3, 0, 1));

	nts::ManageObject::setMaterialLight(tmp, bitewall, false);
	nts::ManageObject::setTexture(tmp, bitewall, "media/rockwall.jpg");
	nts::ManageObject::setMaterialLight(tmp, bitewall3, false);
	nts::ManageObject::setTexture(tmp, bitewall3, "media/rockwall.jpg");
	nts::ManageObject::setMaterialLight(tmp, bitewall2, false);
	nts::ManageObject::setTexture(tmp, bitewall2, "media/rockwall.jpg");

	tmp.loopDisplay();
	return 0;
}