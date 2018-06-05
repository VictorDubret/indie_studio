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
	std::cout << RED << __PRETTY_FUNCTION__ << " LOCK" << RESET << std::endl;
	_entities.lock();
	_sptr = std::shared_ptr<IEntity>(this, [&](IEntity *){});
	_entities->push_back(_sptr);
	_entities.unlock(); std::cout << GRN << __PRETTY_FUNCTION__ << " UNLOCK" << RESET << std::endl;
}

is::AEntity::~AEntity()
{
	std::cout << RED << __PRETTY_FUNCTION__ << " LOCK" << RESET << std::endl;
	_entities.lock();
	std::cout << RED << __PRETTY_FUNCTION__ << " TEST2" << RESET << std::endl;

	_irrlicht.deleteEntity(_sptr);
	std::cout << RED << __PRETTY_FUNCTION__ << " TeST3" << RESET << std::endl;
	auto tmp = std::find(_entities->begin(), _entities->end(), _sptr);
	if (tmp == _entities->end())
		std::cout << YEL << "NIKE BIEN TA MERE SALLE BATARD" << RESET << std::endl;
	_entities->erase(tmp);
	std::cout << RED << __PRETTY_FUNCTION__ << " TEST4" << RESET << std::endl;
	_entities.unlock();
	std::cout << GRN << __PRETTY_FUNCTION__ << " UNLOCK" << RESET << std::endl;
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
	std::cout << RED << __PRETTY_FUNCTION__ << " LOCK" << RESET << std::endl;
	_entities.lock();
	collider->lock();
	Debug::debug(_type, " collide with ", collider->getType());
	collider->unlock();
	_entities.unlock(); std::cout << GRN << __PRETTY_FUNCTION__ << " UNLOCK" << RESET << std::endl;
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
	std::cout << __PRETTY_FUNCTION__ <<" LOCK" << std::endl;
	_irrlicht.lock();
	std::cout << __PRETTY_FUNCTION__ <<" AFTER LOCK" << std::endl;
	float size = _irrlicht.getNodeSize(_sptr);
	std::cout << "a" << std::endl;
	irr::core::vector3df pos(x, 0, z);
	std::cout << "b" << std::endl;
	irr::scene::ISceneNode *node = _irrlicht.getSceneManager()->addCubeSceneNode(size, 0, 1, pos);

	std::cout << "c" << std::endl;
	auto mesh1 = node->getTransformedBoundingBox();
	std::cout << "d" << std::endl;
	node->setVisible(false);

	std::cout << "e" << std::endl;
	auto f = [&](std::shared_ptr<is::IEntity> entity) {
		std::cout << "f" << std::endl;

		auto tmp = _irrlicht.getNode(entity);
		std::cout << "g" << std::endl;
		if (!tmp) {
			std::cout << "h" << std::endl;
			return false;
		}
		std::cout << "i" << std::endl;
		auto mesh2 = _irrlicht.getNode(entity)->getTransformedBoundingBox();
		std::cout << "j" << std::endl;
		bool test = false;
		std::cout << "l" << std::endl;

		if (mesh1.intersectsWithBox(mesh2))
			test = true;
		std::cout << "m" << std::endl;
		std::cout << "n" << std::endl;
		return test;
	};
	std::cout << "o" << std::endl;
	std::cout << RED << __PRETTY_FUNCTION__ << " LOCK" << RESET << std::endl;
	_entities.lock();
	std::cout << "p" << std::endl;
	auto it = std::find_if(_entities->begin(), _entities->end(), f);
	std::cout << "q" << std::endl;
	while (it != _entities->end()) {
		std::cout << "r" << std::endl;

		ret.push_back(*it.base());
		std::cout << "s" << std::endl;
		it++;
		if (it != _entities->end()) {
			std::cout << "t" << std::endl;
			it = std::find_if(it, _entities->end(), f);
		}
	}
	std::cout << "u" << std::endl;
	_entities.unlock(); std::cout << GRN << __PRETTY_FUNCTION__ << " UNLOCK" << RESET << std::endl;
	std::cout << "v" << std::endl;
	node->removeAll();
	//node->remove();
	std::cout << __PRETTY_FUNCTION__ <<" UNLOCK" << std::endl;
	_irrlicht.unlock();
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