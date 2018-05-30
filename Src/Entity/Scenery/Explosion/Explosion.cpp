/*
** EPITECH PROJECT, 2018
** cpp_indie_studio
** File description:
** Created by guillaume.berdeaux@epitech.eu
*/

#include <Tools/Debug.hpp>
#include "Explosion.hpp"

is::Explosion::Explosion(
	my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> &entities,
	my::ItemLocker<my::ThreadPool> &eventManager
):
	AEntity(entities, eventManager)
{
	_type = "Explosion";

	_collidable = true;
	_pickable = false;
	_walkable = true;
	_wallPassable = true;
}

void is::Explosion::explode()
{
	this->~Explosion();
	std::cerr << "Explode " << _type << std::endl;
}

void is::Explosion::collide(is::IEntity *entity)
{
	entity->explode();
	Debug::debug("Character type : ", entity->getType()," die at ", _position.x , ", ", _position.y, ", ", _position.z);
}