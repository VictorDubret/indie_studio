/*
** EPITECH PROJECT, 2018
** cpp_indie_studio
** File description:
** Created by sahel.lucas-saoudi@epitech.eu,
*/

#include "ACharacter.hpp"

is::ACharacter::ACharacter(std::vector<is::IEntity *> &entities) :
	AEntity(entities)
{
}

bool const &is::ACharacter::getWallPass() const
{
	return _wallPass;
}

size_t const &is::ACharacter::getBomb() const
{
	return _bomb;
}

size_t const &is::ACharacter::getBombMax() const
{
	return _bombMax;
}

size_t const &is::ACharacter::getBombLength() const
{
	return _bombLength;
}

size_t const &is::ACharacter::getSpeed() const
{
	return _speed;
}

void is::ACharacter::setWallPass(bool wallPass)
{
	_wallPass = wallPass;
}

void is::ACharacter::setBomb(size_t bomb)
{
	if (bomb > _bombMax)
		throw std::out_of_range("Bomb number is out of range");
	_bomb = bomb;
}

void is::ACharacter::setBombMax(size_t bombMax)
{
	_bombMax = bombMax;
}

void is::ACharacter::setBombLength(size_t length)
{
	_bombLength = length;
}

void is::ACharacter::setSpeed(size_t speed)
{
	_speed = speed;
}

is::ACharacter &is::ACharacter::operator++()
{
	if (_bomb < _bombMax)
		_bomb++;

	return *this;
}

bool is::ACharacter::checkCollision()
{
	bool ret = false;
	for (auto &it: _entities) {
		if (it != this && it->getX() == _position.x &&
			it->getY() == _position.y &&
			it->getZ() == _position.z) {
			if (it->isCollidable()) {
				it->event("collide", this);
				ret = (it->isWallPassable() && _wallPass) ? ret : true;
			}
			if (it->isPickable())
				it->event("pickup", this);
		}
	}
	return ret;
}

void is::ACharacter::moveUp()
{
	_position.y--;
	if (checkCollision())
		_position.y++;
}

void is::ACharacter::moveDown()
{
	_position.y++;
	if (checkCollision())
		_position.y--;
}

void is::ACharacter::moveLeft()
{
	_position.x--;
	if (checkCollision())
		_position.x++;
}

void is::ACharacter::moveRight()
{
	_position.x++;
	if (checkCollision())
		_position.x--;
}