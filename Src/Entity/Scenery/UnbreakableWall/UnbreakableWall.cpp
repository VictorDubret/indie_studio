/*
** EPITECH PROJECT, 2018
** cpp_indie_studio
** File description:
** Created by guillaume.berdeaux@epitech.eu
*/

#include <Debug.hpp>
#include "UnbreakableWall.hpp"

is::UnbreakableWall::UnbreakableWall(
	my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> &entities,
	my::ItemLocker<my::ThreadPool> &eventManager
):
	AEntity(entities, eventManager)
{
	_type = "UnbreakableWall";

	_collidable = true;
	_pickable = false;
	_walkable = false;
	_wallPassable = false;
}