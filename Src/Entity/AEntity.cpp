/*
** EPITECH PROJECT, 2018
** cpp_indie_studio
** File description:
** Created by sahel.lucas-saoudi@epitech.eu,
*/

#include <thread>
#include <chrono>
#include "AEntity.hpp"

is::AEntity::AEntity(std::vector<is::IEntity *> &entities):
	_entities(entities)
{
	_entities.push_back(this);
	_thread = std::thread(&is::AEntity::ia, this);
}

is::AEntity::~AEntity()
{
	ended = true;
	if (_thread.joinable())
		_thread.join();
	for (auto it = _entities.begin(); it != _entities.end(); it++) {
		if (*it.base() == this) {
			_entities.erase(it);
			break;
		}
	}
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

void is::AEntity::event(std::string const &key, is::IEntity *caller)
{
	_mutex.lock();
	_poll.push(std::pair<std::string, is::IEntity *>(key, caller));
	_mutex.unlock();
}

void is::AEntity::execEvent()
{
	_mutex.lock();
	if (_poll.empty()) {
		_mutex.unlock();
		return;
	}
	auto event = _poll.back();
	_poll.pop();
	_mutex.unlock();
	_events[event.first](event.second);
}

bool is::AEntity::isPickable() const
{
	return _pickable;
}

bool is::AEntity::isWallPassable() const
{
	return _wallPassable;
}

void is::AEntity::ia()
{
	while (!ended) {
		execEvent();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}
