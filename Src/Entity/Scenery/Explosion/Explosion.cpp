/*
** EPITECH PROJECT, 2018
** cpp_indie_studio
** File description:
** Created by guillaume.berdeaux@epitech.eu
*/

#include <Tools/Debug.hpp>
#include "ManageIrrlicht.hpp"
#include "Explosion.hpp"
#include "ManageObject.hpp"

is::Explosion::Explosion(
	my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> &entities,
	my::ItemLocker<my::ThreadPool> &eventManager, nts::ManageIrrlicht &irrlicht
):
	AEntity(entities, eventManager, irrlicht)
{
	_type = "Explosion";

	_collidable = true;
	_pickable = false;
	_walkable = true;
	_wallPassable = true;
}

void is::Explosion::texture()
{
	nts::ManageObject::createCube(_irrlicht, _sptr, 1);
	_irrlicht.getNode(_sptr)->setPosition(irr::core::vector3df(0, 0, 0));
	nts::ManageObject::setMaterialLight(_irrlicht, _sptr, false);
	nts::ManageObject::setTexture(_irrlicht, _sptr, "media/water.jpg");
}

void is::Explosion::explode()
{
}

void is::Explosion::collide(is::IEntity *entity)
{
	Debug::debug("Character type : ", entity->getType()," die at ", getX() , ", ", getY(), ", ", getZ());
	entity->explode();
	std::this_thread::sleep_for(std::chrono::seconds(1));
	delete this;
}