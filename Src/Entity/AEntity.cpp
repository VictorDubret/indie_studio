/*
** EPITECH PROJECT, 2018
** cpp_indie_studio
** File description:
** Created by sahel.lucas-saoudi@epitech.eu,
*/

#include <thread>
#include <chrono>
#include <functional>
#include <algorithm>
#include "AEntity.hpp"
#include "Debug.hpp"

is::AEntity::AEntity(Entity_t &entities, ThreadPool_t &eventManager):
	_entities(entities), _eventManager(eventManager)
{
	_entities.lock();
	_entities->push_back(std::shared_ptr<is::IEntity>(this, [&](IEntity *){}));
	_entities.unlock();
}

is::AEntity::~AEntity()
{
	_entities.lock();
	for (auto it = _entities->begin(); it != _entities->end(); it++) {
		if (it->get() == this) {
			_entities->erase(it);
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

double& is::AEntity::getX()
{
	return _position.x;
}

double& is::AEntity::getY()
{
	return _position.y;
}

double &is::AEntity::getZ()
{
	return _position.z;
}

void is::AEntity::setX(double &x)
{
	_position.x = x;
}

void is::AEntity::setY(double &y)
{
	_position.y = y;
}

void is::AEntity::setZ(double &z)
{
	_position.z = z;
}

void is::AEntity::setPosition(is::IEntity::Position &position)
{
	_position.x = position.x;
	_position.y = position.y;
	_position.z = position.z;
}

bool is::AEntity::isCollidable() const
{
	return _collidable;
}

bool is::AEntity::isPickable() const
{
	return _pickable;
}

bool is::AEntity::isWalkable() const
{
	return _walkable;
}

bool is::AEntity::isWallPassable() const
{
	return _wallPassable;
}

void is::AEntity::collide(IEntity *collider)
{
	Debug::debug(_type, " collide with ", collider->getType());
}

void is::AEntity::explode()
{
	Debug::debug(_type, " explode");
}

bool is::AEntity::isInCollisionWith(std::shared_ptr<is::IEntity> &entity)
{
	return ((int) _position.x == (int) entity->getX() &&
		(int) _position.y == (int) entity->getY() &&
		(int) _position.z == (int) entity->getZ());
}

std::vector<std::shared_ptr<is::IEntity>> is::AEntity::getEntitiesAt(
	int x, int y, int z
)
{
	std::vector<std::shared_ptr<is::IEntity>> ret;
	auto it = std::find_if(_entities->begin(), _entities->end(),
		[x, y, z](std::shared_ptr<is::IEntity> entity) {
			return (int) entity->getX() == x &&
				(int) entity->getY() == y &&
				(int) entity->getZ() == z;
		}
	);

	while (it != _entities->end()) {
		ret.push_back(*it.base());
		it++;
		if (it != _entities->end()) {
			it = std::find_if(it, _entities->end(), [x, y, z](std::shared_ptr<is::IEntity> entity) {
				return (int) entity->getX() == x &&
					(int) entity->getY() == y &&
					(int) entity->getZ() == z;
			});
		}
	}
	return ret;
}

void is::AEntity::explode()
{
}