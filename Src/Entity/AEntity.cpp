/*
** EPITECH PROJECT, 2018
** cpp_indie_studio
** File description:
** Created by sahel.lucas-saoudi@epitech.eu,
*/

#include <thread>
#include <irrlicht.h>
#include <algorithm>
#include "AEntity.hpp"
#include "Debug.hpp"
#include "ManageIrrlicht.hpp"
#include "ManageObject.hpp"

is::AEntity::AEntity(Entity_t &entities, ThreadPool_t &eventManager, irrl::ManageIrrlicht &irrlicht):
	_entities(entities), _eventManager(eventManager), _irrlicht(irrlicht)
{
	_spointer = std::shared_ptr<IEntity>(this, [&](IEntity *){});
	_entities->push_back(_spointer);
}

is::AEntity::~AEntity()
{
	if (!_locked) {
		_entities.lock();
		lock();
	}
	_locked = true;
	_irrlicht.deleteEntity(_spointer);
	auto tmp = std::find(_entities->begin(), _entities->end(), _spointer);
	if (tmp != _entities->end())
		_entities->erase(tmp);
	unlock();
	_entities.unlock();
}

irr::core::vector3df const is::AEntity::getPosition() const
{
	irr::core::vector3df tmp({0, 0, 0});
	_irrlicht.lock();
	auto node = _irrlicht.getNode(_spointer.get());
	if (_spointer.get() && node)
		tmp = node->getPosition();
	_irrlicht.unlock();
	return tmp;
}

std::string const& is::AEntity::getType() const
{
	return _type;
}

float is::AEntity::getX() const
{
	auto pos = getPosition();
	return pos.X;
}

float is::AEntity::getY() const
{
	auto pos = getPosition();
	return pos.Y;
}

float is::AEntity::getZ() const
{
	auto pos = getPosition();
	return pos.Z;
}

void is::AEntity::setX(float x)
{
	auto pos = getPosition();
	pos.X = x;
	setPosition(pos);
}

void is::AEntity::setY(float y)
{
	auto pos = getPosition();
	pos.Y = y;
	setPosition(pos);
}

void is::AEntity::setZ(float z)
{
	auto pos = getPosition();
	pos.Z = z;
	setPosition(pos);
}

void is::AEntity::setPosition(irr::core::vector3df position)
{
	_irrlicht.lock();
	auto node = _irrlicht.getNode(_spointer.get());
	if (node)
		node->setPosition(position);
	_irrlicht.unlock();
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

bool is::AEntity::isWalkable(std::shared_ptr<is::IEntity> &)
{
	return _walkable;
}

bool is::AEntity::isWallPassable() const
{
	return _wallPassable;
}

void is::AEntity::collide(IEntity *collider)
{
	//Debug::debug(_type, " collide with ", collider->getType());
}

void is::AEntity::explode()
{
	//Debug::debug(_type, " explode");
}

bool is::AEntity::isInCollisionWith(std::shared_ptr<is::IEntity> &entity)
{
	auto size = _irrlicht.getNodeSize(_spointer);
	return (((getX() >= entity->getX() && getX() < entity->getX() + size) || (getX() + size > entity->getX() && getX() + size < entity->getX() + size)) &&
		((getY() >= entity->getZ() && getZ() < entity->getZ() + size) || (getZ() + size > entity->getZ() && getZ() + size < entity->getZ() + size)));
}

std::vector<std::shared_ptr<is::IEntity>> is::AEntity::getEntitiesAt(float x, float z) const
{
	std::vector<std::shared_ptr<is::IEntity>> ret;
	_irrlicht.lock();
	float size = _irrlicht.getNodeSize(_spointer);
	irr::core::vector3df pos(x, 0, z);
	auto sceneManager = _irrlicht.getSceneManager();
	_irrlicht.unlock();
	if (!sceneManager) {
		return ret;
	}
	irr::scene::ISceneNode *node = sceneManager->addCubeSceneNode(size, 0, 1, pos);

	auto mesh1 = node->getTransformedBoundingBox();
	node->setVisible(false);

	auto f = [&](std::shared_ptr<is::IEntity> entity) {
		if (!dynamic_cast<AEntity *>(entity.get()))
			return false;
		entity->lock();
		_irrlicht.lock();
		auto tmp = _irrlicht.getNode(entity.get());
		entity->unlock();
		_irrlicht.unlock();
		if (!tmp)
			return false;
		auto mesh2 = tmp->getTransformedBoundingBox();
		return mesh1.intersectsWithBox(mesh2);
	};
	auto it = std::find_if(_entities->begin(), _entities->end(), f);
	while (it != _entities->end()) {
		ret.push_back(*(it.base()));
		it++;
		if (it != _entities->end()) {
			it = std::find_if(it, _entities->end(), f);
		}
	}
	_irrlicht.lock();
	node->remove();
	_irrlicht.unlock();
	return std::move(ret);
}

void is::AEntity::lock()
{
	_mutex.lock();
}

void is::AEntity::unlock()
{
	_mutex.unlock();
}

void is::AEntity::setCollidable(bool state)
{
	_collidable = state;
}

void is::AEntity::setPickable(bool state)
{
	_pickable = state;
}

void is::AEntity::setWallPassable(bool state)
{
	_wallPassable = state;
}

void is::AEntity::setWalkable(bool state)
{
	_walkable = state;
}
