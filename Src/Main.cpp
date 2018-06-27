/*
** EPITECH PROJECT, 2018
** test
** File description:
** Created by martin.januario@epitech.eu,
*/

#include <chrono>
#include "ThreadPool.hpp"
#include "IEntity.hpp"
#include "ItemLocker.hpp"
#include "ManageIrrlicht.hpp"

int main()
{
	my::ThreadPool thpool(100);
	std::vector<std::shared_ptr<is::IEntity>> list;
	my::ItemLocker<my::ThreadPool> pool(thpool);
	my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> lockList(list);
	try {
		irrl::ManageIrrlicht tmp(lockList, pool,
			irr::core::vector2di(11, 13),
			false);
		tmp.loopDisplay();
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	//pool->finishAll();
	return 0;
}
