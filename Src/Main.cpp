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

	is::IEntity *player = new is::ACharacter(lockList, pool);

	nts::ManageIrrlicht tmp(lockList, pool);

	std::shared_ptr<is::IEntity> player_tmp = std::shared_ptr<is::IEntity>(player, [](is::IEntity *){});

	nts::ManageObject::createCube(tmp, player_tmp, 10);
	nts::ManageObject::setMaterialLight(tmp, player_tmp, false);
	nts::ManageObject::setTexture(tmp, player_tmp, "media/rockwall.jpg");

	tmp.loopDisplay();
	return 0;
}