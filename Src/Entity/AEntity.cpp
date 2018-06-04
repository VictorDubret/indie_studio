/*
** EPITECH PROJECT, 2018
** cpp_indie_studio
** File description:
** Created by sahel.lucas-saoudi@epitech.eu,
*/

#include <thread>
#include <chrono>
#include <functional>
# include <irrlicht.h>
#include <algorithm>
#include "AEntity.hpp"
#include "Debug.hpp"
#include "ManageIrrlicht.hpp"
#include "ManageObject.hpp"

is::AEntity::AEntity(Entity_t &entities, ThreadPool_t &eventManager, nts::ManageIrrlicht &irrlicht):
	_entities(entities), _eventManager(eventManager), _irrlicht(irrlicht)
{
	_entities.lock();
	_sptr = std::shared_ptr<IEntity>(this, [&](IEntity *){});
	_entities->push_back(_sptr);
	_entities.unlock();
}

is::AEntity::~AEntity()
{
	_entities.lock();
	_irrlicht.deleteEntity(_sptr);
	_entities->erase(
		std::find(_entities->begin(), _entities->end(),
			_sptr));
	_entities.unlock();
}

irr::core::vector3df const is::AEntity::getPosition() const
{
	return (!_irrlicht.getNode(_sptr)) ? irr::core::vector3df({0, 0, 0}) : _irrlicht.getNode(_sptr)->getPosition();
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
	_irrlicht.getNode(_sptr)->setPosition(pos);
}

void is::AEntity::setY(float y)
{
	auto pos = getPosition();
	pos.Y = y;
	_irrlicht.getNode(_sptr)->setPosition(pos);
}

void is::AEntity::setZ(float z)
{
	auto pos = getPosition();
	pos.Z = z;
	_irrlicht.getNode(_sptr)->setPosition(pos);
}

void is::AEntity::setPosition(irr::core::vector3df position)
{
	_irrlicht.getNode(_sptr)->setPosition(position);
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

bool is::AEntity::isWalkable(std::shared_ptr<is::IEntity> &) const
{
	return _walkable;
}

bool is::AEntity::isWallPassable() const
{
	return _wallPassable;
}

void is::AEntity::collide(IEntity *&collider)
{
	_entities.lock();
	collider->lock();
	Debug::debug(_type, " collide with ", collider->getType());
	collider->unlock();
	_entities.unlock();
}

void is::AEntity::explode()
{
	Debug::debug(_type, " explode");
}

bool is::AEntity::isInCollisionWith(std::shared_ptr<is::IEntity> &entity)
{
	auto size = _irrlicht.getNodeSize(_sptr);
	return (((getX() >= entity->getX() && getX() < entity->getX() + size) || (getX() + size > entity->getX() && getX() + size < entity->getX() + size)) &&
		((getY() >= entity->getZ() && getZ() < entity->getZ() + size) || (getZ() + size > entity->getZ() && getZ() + size < entity->getZ() + size)));
}

std::vector<std::shared_ptr<is::IEntity>> is::AEntity::getEntitiesAt(float x, float y, float z) const
{
	std::vector<std::shared_ptr<is::IEntity>> ret;
	float size = _irrlicht.getNodeSize(_sptr);
	irr::core::vector3df pos(x, 0, z);
	irr::scene::ISceneNode *node = _irrlicht.getSceneManager()->addCubeSceneNode(size, 0, 1, pos);

	auto mesh1 = node->getTransformedBoundingBox();
	node->setVisible(false);

	auto f = [&](std::shared_ptr<is::IEntity> entity) {
		static int i = 0;
		entity->lock();

		auto mesh2 = _irrlicht.getNode(entity)->getTransformedBoundingBox();
		bool test = false;

		if (mesh2.intersectsWithBox(mesh1))
			test = true;
		entity->unlock();
		i++;
		return test;
	};
	_entities.lock();
	auto it = std::find_if(_entities->begin(), _entities->end(), f);
	while (it != _entities->end()) {

		ret.push_back(*it.base());
		it++;
		if (it != _entities->end()) {
			it = std::find_if(it, _entities->end(), f);
		}
	}
	_entities.unlock();
	node->getSceneManager()->addToDeletionQueue(node);
	return ret;
}

void is::AEntity::lock()
{
	_mutex.lock();
}

void is::AEntity::unlock()
{
	_mutex.unlock();
}