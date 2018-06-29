/*
** EPITECH PROJECT, 2018
** cpp_indie_studio
** File description:
** Created by sahel.lucas-saoudi@epitech.eu,
*/

#include <algorithm>
#include <fstream>
#include <Entity/Scenery/Wall/Wall.hpp>
#include "Bomb.hpp"
#include "ManageObject.hpp"
#include "APowerUp.hpp"

is::ACharacter::ACharacter(
	my::ItemLocker<std::vector<std::shared_ptr<IEntity>>> &entities,
	my::ItemLocker<my::ThreadPool> &eventManager,
	irrl::ManageIrrlicht &irrlicht, std::size_t id
) : AEntity(entities, eventManager, irrlicht), _id(id)
{
	_type = "Character";
	_walkable = true;
	texture();
}

void is::ACharacter::texture()
{
	irrl::ManageObject::createAnim(_irrlicht, _spointer,
		"media/character.b3d", 0.6);
	_irrlicht.getNode(_spointer.get())->setPosition(
		irr::core::vector3df(1.1f, -0.5f, 1.1f));
	irrl::ManageObject::setScale(_irrlicht, _spointer,
		irr::core::vector3df(0.9, 0.9, 0.9));
	irrl::ManageObject::setRotation(_irrlicht, _spointer,
		irr::core::vector3df(0, 90, 0));
	irrl::ManageObject::setMaterialLight(_irrlicht, _spointer, false);
	irrl::ManageObject::setAnimation(_irrlicht, _spointer,
		irr::scene::EMAT_RUN);
	irrl::ManageObject::setTexture(_irrlicht, _spointer,
		(std::string("media/") + std::to_string(_id) +
			std::string("_character.png")).c_str());
}

is::ACharacter::~ACharacter()
{
	_locked = true;
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

float const &is::ACharacter::getSpeed() const
{
	return _speed;
}

void is::ACharacter::setWallPass(bool wallPass)
{
	_wallPass = wallPass;
}

void is::ACharacter::setBomb(size_t bomb)
{
	if (bomb <= _bombMax)
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

void is::ACharacter::setSpeed(float speed)
{
	if (_speed > 4)
		return;
	if (_speedCoef * speed < 0.24)
		_speed = speed;
	else
		_speed = 0.24 / (float)_speedCoef;
}

is::ACharacter &is::ACharacter::operator++()
{
	if (_bomb < _bombMax)
		++_bomb;
	return *this;
}

bool is::ACharacter::checkCollision(
	std::vector<std::shared_ptr<is::IEntity>> &list
)
{
	bool ret = false;

	for (auto &it : list) {
		if (it.get() != this && it->isCollidable()) {
			it->collide(this);
		}
	}
	return ret;
}

bool is::ACharacter::move(float nextX, float nextY, float nextZ)
{
	if (_xmax == -1 || _zmax == -1)
		for (auto &it: _entities.get()) {
			if (_xmax < it->getX())
				_xmax = (int)it->getX();
			if (_zmax < it->getZ())
				_zmax = (int)it->getZ();
		}

	auto list = getEntitiesAt(nextX, nextZ);

	for (auto &it : list) {
		//	std::lock_guard<std::recursive_mutex> lk(it->getMutex());
		if (it.get() != this && it->isCollidable() &&
			!it->isWalkable(_spointer) &&
			((it->isWallPassable() && !_wallPass) ||
				!it->isWallPassable())) {
			return false;
		}
	}
	while (abs(getX() - nextX) > 0.01 || abs(getY() - nextY) > 0.01 ||
		abs(getZ() - nextZ) > 0.01) {
		if (abs(getX() - nextX) > 0.01)
			setX(getX() > nextX ? getX() - 0.01 : getX() + 0.01);
		if (abs(getY() - nextY) > 0.01)
			setY(getY() > nextY ? getY() - 0.01 : getY() + 0.01);
		if (abs(getZ() - nextZ) > 0.01)
			setZ(getZ() > nextZ ? getZ() - 0.01 : getZ() + 0.01);
	}
	checkCollision(list);
	return true;
}

void is::ACharacter::moveUp()
{
	if (_lastMove != MoveCharacter::UP) {
		irrl::ManageObject::setAnimation(_irrlicht, _spointer,
			irr::scene::EMAT_RUN);
		irrl::ManageObject::setRotation(_irrlicht, _spointer,
			irr::core::vector3df(0, 270, 0));
		_lastMove = MoveCharacter::UP;
	}
	if (!dynamic_cast<AEntity *>(_spointer.get()))
		return;
	float x, y, z, next;
	{
		x = getX();
		y = getY();
		z = getZ();
		next = z + _speed * _speedCoef;
	}

	if (!move(x, y, next)) {
		if (x < _xmax / 2)
			move(x + 0.1f, y, z);
		else
			move(x - 0.1f, y, z);
	}
}

void is::ACharacter::moveDown()
{
	if (_lastMove != MoveCharacter::DOWN) {
		irrl::ManageObject::setAnimation(_irrlicht, _spointer,
			irr::scene::EMAT_RUN);
		irrl::ManageObject::setRotation(_irrlicht, _spointer,
			irr::core::vector3df(0, 90, 0));
		_lastMove = MoveCharacter::DOWN;
	}
	if (!dynamic_cast<AEntity *>(_spointer.get())) {
		return;
	}
	float x, y, z, next;
	{
		x = getX();
		y = getY();
		z = getZ();
		next = z - _speed * _speedCoef;
	}

	if (!move(x, y, next)) {
		if (x < _xmax / 2)
			move(x + 0.1f, y, z);
		else
			move(x - 0.1f, y, z);
	}
}

void is::ACharacter::moveLeft()
{
	if (_lastMove != MoveCharacter::LEFT) {
		irrl::ManageObject::setAnimation(_irrlicht, _spointer,
			irr::scene::EMAT_RUN);
		irrl::ManageObject::setRotation(_irrlicht, _spointer,
			irr::core::vector3df(0, 180, 0));
		_lastMove = MoveCharacter::LEFT;
	}
	if (!dynamic_cast<AEntity *>(_spointer.get())) {
		return;
	}

	float x, y, z, next;
	{
		x = getX();
		y = getY();
		z = getZ();
		next = x - _speed * _speedCoef;
	}

	if (!move(next, y, z)) {
		if (z < _zmax / 2)
			move(x, y, z + 0.1f);
		else
			move(x, y, z - 0.1f);
	}
}

void is::ACharacter::moveRight()
{
	if (_lastMove != MoveCharacter::RIGHT) {
		irrl::ManageObject::setAnimation(_irrlicht, _spointer,
			irr::scene::EMAT_RUN);
		irrl::ManageObject::setRotation(_irrlicht, _spointer,
			irr::core::vector3df(0, 0, 0));
		_lastMove = MoveCharacter::RIGHT;
	}
	if (!dynamic_cast<AEntity *>(_spointer.get())) {
		return;
	}
	float x, y, z, next;
	{
		x = getX();
		y = getY();
		z = getZ();
		next = x + _speed * _speedCoef;
	}

	if (!move(next, y, z)) {
		if (z < _zmax / 2)
			move(x, y, z + 0.1f);
		else
			move(x, y, z - 0.1f);
	}
}

void is::ACharacter::dropBomb()
{
	if (!dynamic_cast<AEntity *>(_spointer.get())) {
		//	_entities.unlock();
		return;
	}
	if (_bomb <= 0) {
		return;
	}
	float size = _irrlicht.getNodeSize(_spointer);
	auto _entitiesAt = getEntitiesAt((int)(getX() + size / 2.0),
		(int)(getZ() + size / 2.0));

	for (auto &it: _entitiesAt) {
		auto checkCharacter = dynamic_cast<ACharacter *>(it.get());
		auto checkPowerUp = dynamic_cast<APowerUp *>(it.get());
		if (checkCharacter == nullptr && checkPowerUp == nullptr) {
			return;
		}
	}
	--_bomb;
	auto bomb = new is::Bomb(_entities, _eventManager, _spointer,
		_irrlicht);
	bomb->setX((int)(getX() + size / 2.0));
	bomb->setY((int)(getY()));
	bomb->setZ((int)(getZ() + size / 2.0));
}

void is::ACharacter::explode()
{
	--_pv;
	if (_pv == 0) {
		if (!dynamic_cast<AEntity *>(_spointer.get())) {
			return;
		}
		this->~ACharacter();
	}
}

void is::ACharacter::doNothing()
{
	if (_lastMove != MoveCharacter::NOTHING) {
		irrl::ManageObject::setAnimation(_irrlicht, _spointer,
			irr::scene::EMAT_STAND);
		_lastMove = MoveCharacter::NOTHING;
	}
}

void is::ACharacter::save()
{
}

void is::ACharacter::setHP(int life)
{
	_pv = static_cast<size_t>(life);
}
