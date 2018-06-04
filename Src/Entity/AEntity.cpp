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

std::vector<std::shared_ptr<is::IEntity>> is::AEntity::getEntitiesAt(
	float x, float, float z)
{
	std::vector<std::shared_ptr<is::IEntity>> ret;
	float size = _irrlicht.getNodeSize(_sptr);
	auto f = [x, z, this, size](std::shared_ptr<is::IEntity> entity) {
		if (dynamic_cast<IEntity *>(entity.get()) == nullptr) {
			std::cerr << "JE TAIME PAS" << std::endl;
			return false;
		}
		entity->lock();
		float esize = _irrlicht.getNodeSize(entity);

		std::pair<float, float> e1a(x, z);
		std::pair<float, float> e1b(x + size, z);
		std::pair<float, float> e1c(x , z + size);
		std::pair<float, float> e1d(x + size, z + size);

		std::pair<float, float> e2a(entity->getX(), entity->getZ());
		std::pair<float, float> e2b(entity->getX() + esize, entity->getZ());
		std::pair<float, float> e2c(entity->getX() , entity->getZ() + esize);
		std::pair<float, float> e2d(entity->getX() + esize, entity->getZ() + esize);

		bool a = (e1d.first > e2a.first && e1d.first < e2b.first &&
			e1d.second > e2a.second && e1d.second < e2c.second);

		bool b = (e2d.first > e1a.first && e2d.first < e1b.first &&
			e2d.second > e1a.second && e2d.second < e1c.second);

		bool c = (e2b.first > e1a.first && e2b.first < e1b.first &&
			e2b.second > e1a.second && e2b.second < e1c.second);

		bool d = (e1b.first > e2a.first && e1b.first < e2b.first &&
			e1b.second > e2a.second && e1b.second < e2c.second);

		bool e = (e1a.first > e2a.first && e1a.first < e2b.first &&
			e1a.second > e2a.second && e1a.second < e2c.second);

		if (a || b || c || d || e) {
			std::cout << entity->getType() << " MAIS MDR NIKE TOI" << std::endl;
		}
		entity->unlock();
		return (a || b || c || d || e);
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