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
	lock();
	try {
		_irrlicht.deleteEntity(_sptr);
		_entities->erase(
			std::find(_entities->begin(), _entities->end(),
				_sptr));
	} catch (std::exception &){

	}
	unlock();
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
	auto size = _irrlicht.getNodeSize(_sptr);
	return (((getX() >= entity->getX() && getX() < entity->getX() + size) || (getX() + size > entity->getX() && getX() + size < entity->getX() + size)) &&
		((getY() >= entity->getZ() && getZ() < entity->getZ() + size) || (getZ() + size > entity->getZ() && getZ() + size < entity->getZ() + size)));
}

std::vector<std::shared_ptr<is::IEntity>> is::AEntity::getEntitiesAt(float x, float y, float z) const
{
	std::vector<std::shared_ptr<is::IEntity>> ret;
	float size = _irrlicht.getNodeSize(_sptr);
	//irr::core::line3d<float> mesh1(x, y - 100, z, x, y + 100, z);
	auto a = _irrlicht.getSceneManager()->getGeometryCreator()->createCylinderMesh(0.1, 2, 10, 0, true, 0);
	auto node = _irrlicht.getSceneManager()->addMeshSceneNode(a);
	std::cout << x << " " << z << std::endl;
	irr::core::vector3df pos(x, 0, z);
	node->setPosition(pos);
	node->setPosition(pos);
	node->setPosition(pos);
	node->setPosition(pos);
	node->setPosition(pos);
	std::cout << "Abs Position " << node->getAbsolutePosition().X << " " << node->getAbsolutePosition().Z << " " << std::endl;

	node->setVisible(true);

	//auto mesh1 = node->getTransformedBoundingBox();
	auto mesh1 = node->getTransformedBoundingBox();
	std::cout << mesh1.getCenter().X << " " << mesh1.getCenter().Y << " " << mesh1.getCenter().Z << std::endl;
	auto f = [x, y, z, this, size, mesh1, node](std::shared_ptr<is::IEntity> entity) {
		static int i = 0;
		entity->lock();
		auto mesh2 = _irrlicht.getNode(entity)->getTransformedBoundingBox();
		bool a = false;

		if (mesh2.intersectsWithBox(mesh1))
		{
			std::cout << "Box: " << mesh2.getCenter().X << " " << mesh2.getCenter().Y << " " << mesh2.getCenter().Z << std::endl;
			std::cerr << entity->getX() << " " << entity->getY() << " " << entity->getZ() << std::endl;
			std::cerr << entity->getType() << " => true " << i <<  std::endl;
			a = true;
		}
		entity->unlock();
		i++;
		return a;
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