/*
** EPITECH PROJECT, 2018
** Created by victor on 22/05/18
** File description:
** main.cpp
*/

#include <IEntity.hpp>
#include <ACharacter.hpp>
#include "Tools/ThreadPool.hpp"
#include "Tools/ItemLocker.hpp"

int main()
{
	std::vector<std::shared_ptr<is::IEntity>> a;
	my::ThreadPool _threadPool(5);
	my::ItemLocker<my::ThreadPool> threadPool(_threadPool);
	my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> entities(a);

	is::ACharacter toto(entities, threadPool);
	toto.moveUp();
	return 0;
}