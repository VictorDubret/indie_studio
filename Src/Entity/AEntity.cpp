/*
** EPITECH PROJECT, 2018
** cpp_indie_studio
** File description:
** Created by sahel.lucas-saoudi@epitech.eu,
*/

#include <thread>
#include <chrono>
#include <functional>
#include "AEntity.hpp"

is::AEntity::AEntity(my::ItemLocker<std::vector<std::shared_ptr<IEntity>>> &entities,
	my::ItemLocker<my::ThreadPool> &eventManager):
	_entities(entities), _eventManager(eventManager)
{
	_entities->push_back(std::shared_ptr<is::IEntity>(this));
}

is::AEntity::~AEntity()
{
	_entities.lock();
	for (auto it = _entities->begin(); it != _entities->end(); it++) {
		if (it.base()->get() == this) {
			_entities->erase(it);
			_entities.unlock();
			break;
		}
	}
	_entities.unlock();
}

is::IEntity::Position& is::AEntity::getPosition()
{
	return _position;
}

std::string const& is::AEntity::getType() const
{
	return _type;
}

float& is::AEntity::getX()
{
	return _position.x;
}

float& is::AEntity::getY()
{
	return _position.y;
}

float &is::AEntity::getZ()
{
	return _position.z;
}

void is::AEntity::setX(float &x)
{
	_position.x = x;
}

void is::AEntity::setY(float &y)
{
	_position.y = y;
}

void is::AEntity::setZ(float &z)
{
	_position.z = z;
}

bool is::AEntity::isCollidable() const
{
	return _collidable;
}

bool is::AEntity::isPickable() const
{
	return _pickable;
}

bool is::AEntity::isWallPassable() const
{
	return _wallPassable;
}

void is::AEntity::collide(IEntity *collider)
{
	std::cout << "Collide with " << collider->getType() << std::endl;
}
