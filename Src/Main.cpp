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

int main()
{
	my::ThreadPool thpool(100);
	std::vector<std::shared_ptr<is::IEntity>> list;

	my::ItemLocker<my::ThreadPool> pool(thpool);
	my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> lockList(list);


	//auto pos = is::IEntity::Position({0, 0, 40});
	//player->setPosition(pos);
	//pos.z = 80;
	//player2->setPosition(pos);

	nts::ManageIrrlicht tmp(lockList, pool);
	is::IEntity *player = new is::ACharacter(lockList, pool, tmp);
	is::IEntity *player2 = new is::ACharacter(lockList, pool, tmp);

	std::shared_ptr<is::IEntity> player_tmp = std::shared_ptr<is::IEntity>(player, [](is::IEntity *){});
	std::shared_ptr<is::IEntity> player_tmp2 = std::shared_ptr<is::IEntity>(player2, [](is::IEntity *){});

	nts::ManageObject::createCube(tmp, player_tmp, 10);
	nts::ManageObject::createCube(tmp, player_tmp2, 10);

	tmp.getNode(player_tmp)->setPosition(irr::core::vector3df(0, 0, 40));
	tmp.getNode(player_tmp)->setPosition(irr::core::vector3df(0, 0, 80));

	nts::ManageObject::setMaterialLight(tmp, player_tmp, false);
	nts::ManageObject::setMaterialLight(tmp, player_tmp2, false);

	nts::ManageObject::setTexture(tmp, player_tmp, "media/rockwall.jpg");
	nts::ManageObject::setTexture(tmp, player_tmp2, "media/rockwall.jpg");

	tmp.loopDisplay();
	return 0;
}