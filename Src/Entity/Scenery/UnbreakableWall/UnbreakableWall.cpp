/*
** EPITECH PROJECT, 2018
** cpp_indie_studio
** File description:
** Created by guillaume.berdeaux@epitech.eu
*/

#include <Debug.hpp>
#include "UnbreakableWall.hpp"
#include "ManageObject.hpp"

is::UnbreakableWall::UnbreakableWall(
	my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> &entities,
	my::ItemLocker<my::ThreadPool> &eventManager, irrl::ManageIrrlicht &irrlicht
) : AEntity(entities, eventManager, irrlicht)
{
	_type = "UnbreakableWall";

	_collidable = true;
	_pickable = false;
	_walkable = false;
	_wallPassable = false;
	texture();
}

is::UnbreakableWall::~UnbreakableWall()
{
	if (!_locked) {
		_entities.lock();
		lock();
	}
	_locked = true;
}

void is::UnbreakableWall::texture()
{
	irrl::ManageObject::createCube(_irrlicht, _sptr, 0.9999);
	irrl::ManageObject::setMaterialLight(_irrlicht, _sptr, false);
	irrl::ManageObject::setTexture(_irrlicht, _sptr, "media/wall.png");
}
