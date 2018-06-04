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
	my::ItemLocker<my::ThreadPool> &eventManager, nts::ManageIrrlicht &irrlicht
) : AEntity(entities, eventManager, irrlicht)
{
	_type = "UnbreakableWall";

	_collidable = true;
	_pickable = false;
	_walkable = false;
	_wallPassable = false;
	texture();
}

void is::UnbreakableWall::texture()
{
	nts::ManageObject::createCube(_irrlicht, _sptr, 1);
	nts::ManageObject::setMaterialLight(_irrlicht, _sptr, false);
	nts::ManageObject::setTexture(_irrlicht, _sptr, "media/fire.bmp");
}
