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
	irr::IReferenceCounted *cube = tmp.getSceneManager()->addCubeSceneNode(10.f, 0, -3, irr::core::vector3df(0.f, 0.f, 20.f));

	tmp.addEntity(player_tmp, cube);

	tmp.loopDisplay();
	return 0;
}