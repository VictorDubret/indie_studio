/*
** EPITECH PROJECT, 2018
** cpp_indie_studio
** File description:
** Created by sahel.lucas-saoudi@epitech.eu,
*/

#include "AEntity.hpp"

is::AEntity::AEntity(std::vector<is::IEntity *> &entities):
	_entities(entities)
{
	_entities.push_back(this);
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

void is::AEntity::event(std::string const &key)
{
	try {
		_events[key](nullptr);
	} catch (std::exception &e) {

	}
}

void is::AEntity::event(std::string const &key, is::IEntity &caller)
{
	try {
		(_events[key])(&caller);
	} catch (std::exception &e) {

	}
}