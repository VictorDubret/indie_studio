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

is::AEntity::AEntity(Entity_t &entities, ThreadPool_t &eventManager, nts::ManageIrrlicht &irrlicht):
	_entities(entities), _eventManager(eventManager), _irrlicht(irrlicht)
{
	std::cout << std::this_thread::get_id() << " : " << __PRETTY_FUNCTION__ << " LOCK" << RESET << std::endl;
	_entities.lock();
	std::cout << std::this_thread::get_id() << " : " << __PRETTY_FUNCTION__ << " AFTER LOCK" << RESET << std::endl;
	_sptr = std::shared_ptr<IEntity>(this, [&](IEntity *){});
	_entities->push_back(_sptr);
	std::cout << std::this_thread::get_id() << " : " << __PRETTY_FUNCTION__ << " UNLOCK" << RESET << std::endl;
	_entities.unlock();
}

is::AEntity::~AEntity()
{
	if (!_locked) {
		_entities.lock();
		lock();
	}
	_locked = true;
	std::cout << "Je vais delete entity : " << _type << std::endl;
	_irrlicht.deleteEntity(_sptr);
	auto tmp = std::find(_entities->begin(), _entities->end(), _sptr);
//	if (tmp == _entities->end())
//		std::cout << YEL << "NIKE BIEN TA MERE SALLE BATARD" << RESET << std::endl;
	_entities->erase(tmp);
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
	auto node = _irrlicht.getNode(_sptr);
	if (node)
		node->setPosition(position);
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
	std::cout << BLU << std::this_thread::get_id() << " : " << __PRETTY_FUNCTION__ << " LOCK" <<  RESET << std::endl;
	_irrlicht.lock();
	std::cout << BLU << std::this_thread::get_id() << " : " << __PRETTY_FUNCTION__ << " AFTER LOCK" << RESET <<  std::endl;
	float size = _irrlicht.getNodeSize(_sptr);
	irr::core::vector3df pos(x, 0, z);
	auto sceneManager = _irrlicht.getSceneManager();
	if (!sceneManager) {
		std::cout << BLU << std::this_thread::get_id() << " : " << __PRETTY_FUNCTION__ <<" UNLOCK" << RESET << std::endl;
		_irrlicht.unlock();
		return ret;
	}
	irr::scene::ISceneNode *node = sceneManager->addCubeSceneNode(size, 0, 1, pos);

	auto mesh1 = node->getTransformedBoundingBox();
	node->setVisible(false);

	auto f = [&](std::shared_ptr<is::IEntity> entity) {
		std::cout << std::this_thread::get_id() << YEL << " " << entity.get() <<  " : " << __PRETTY_FUNCTION__ << " LOCK" << entity->getType() << RESET << std::endl;
		entity->lock();
		std::cout << std::this_thread::get_id() << YEL << " " << entity.get() <<  " : " << __PRETTY_FUNCTION__ << " AFTER LOCK" << RESET << std::endl;
		auto tmp = _irrlicht.getNode(entity);
		if (!tmp) {
			std::cout << std::this_thread::get_id() << YEL << " " << entity.get() <<  " : " << __PRETTY_FUNCTION__ << " UNLOCK" << RESET << std::endl;
			entity->unlock();
			return false;
		}
		auto mesh2 = _irrlicht.getNode(entity)->getTransformedBoundingBox();
		bool test = false;
		if (mesh1.intersectsWithBox(mesh2))
			test = true;
		std::cout << std::this_thread::get_id() << YEL << " " << entity.get() <<  " : " << __PRETTY_FUNCTION__ << " UNLOCK" << RESET << std::endl;
		entity->unlock();
		return test;
	};
	auto it = std::find_if(_entities->begin(), _entities->end(), f);
	while (it != _entities->end()) {
		ret.push_back(*(it.base()));
		it++;
		if (it != _entities->end()) {
			it = std::find_if(it, _entities->end(), f);
		}
	}
	node->removeAll();
	//node->remove();
	std::cout << BLU << std::this_thread::get_id() << " : " << __PRETTY_FUNCTION__ << " UNLOCK" << RESET << std::endl;
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