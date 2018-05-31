/*
** EPITECH PROJECT, 2018
** cpp_indie_studio
** File description:
** Created by sahel.lucas-saoudi@epitech.eu,
*/

#include "ThreadPool.hpp"
#include "ACharacter.hpp"
#include "Debug.hpp"

is::ACharacter::ACharacter(my::ItemLocker<std::vector<std::shared_ptr<IEntity>>> &entities, my::ItemLocker<my::ThreadPool> &eventManager, nts::ManageIrrlicht &irrlicht) :
	AEntity(entities, eventManager, irrlicht)
{
	_type = "Character";
	_walkable = false;
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
	auto list = getEntitiesAt((int) getX(), (int) getY(), (int) getZ());

	_entities.lock();
	for (auto &it: list) {
		if (it.get() != this && it->isCollidable()) {
			_eventManager.lock();
			_eventManager->enqueue([it, this]() {
				dynamic_cast<AEntity *>(it.get())->collide(this);
			});
			_eventManager.unlock();
		}
	}
	_entities.unlock();
	return ret;
}

void is::ACharacter::move(float nextX, float nextY, float nextZ)
{
	auto list = getEntitiesAt(nextX, nextY, nextZ);

	for (auto &it: list) {
		if (it.get() != this && it->isCollidable() && !it->isWalkable() && ((it->isWallPassable() && !_wallPass) || !it->isWallPassable())) {
			std::cout << "COLLIDE" << std::endl;
			return;
		}
	}
	std::cerr << "MOVE" << std::endl;
	setZ(nextZ);
	setY(nextY);
	setX(nextX);
	checkCollision();
}

void is::ACharacter::moveUp()
{
	float next = getZ() + _speed * _speedCoef;

	move(getX(), getY(), next);
}

void is::ACharacter::moveDown()
{
	float next = getZ() - _speed * _speedCoef;

	move(getX(), getY(), next);
}

void is::ACharacter::moveLeft()
{
	float next = getX() - _speed * _speedCoef;

	move(next, getY(), getZ());
}

void is::ACharacter::moveRight()
{
	float next = getX() + _speed * _speedCoef;

	move(next, getY(), getZ());
}

void is::ACharacter::dropBomb()
{
	Debug::debug("DROP BOMB");
	if (_bomb > 0) {
		// TODO create Bomb
	}
}