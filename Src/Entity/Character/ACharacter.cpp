/*
** EPITECH PROJECT, 2018
** cpp_indie_studio
** File description:
** Created by sahel.lucas-saoudi@epitech.eu,
*/

#include <algorithm>
#include <fstream>
#include <zconf.h>
#include <Entity/Scenery/Wall/Wall.hpp>
#include "ThreadPool.hpp"
#include "ACharacter.hpp"
#include "Debug.hpp"
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
	irrl::ManageObject::createAnim(_irrlicht, _sptr, "media/character.b3d",
		0.6);
	_irrlicht.getNode(_sptr.get())->setPosition(
		irr::core::vector3df(1.1f, -0.5f, 1.1f));
	irrl::ManageObject::setScale(_irrlicht, _sptr,
		irr::core::vector3df(0.9, 0.9, 0.9));
	irrl::ManageObject::setRotation(_irrlicht, _sptr,
		irr::core::vector3df(0, 90, 0));
	irrl::ManageObject::setMaterialLight(_irrlicht, _sptr, false);
	irrl::ManageObject::setAnimation(_irrlicht, _sptr, irr::scene::EMAT_RUN);
	irrl::ManageObject::setTexture(_irrlicht, _sptr, (std::string("media/") + std::to_string(_id) + std::string("_character.png")).c_str());
}

is::ACharacter::~ACharacter()
{
	if (!_locked) {
		_entities.lock();
		lock();
	}
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
	lock();
	_wallPass = wallPass;
	unlock();
}

void is::ACharacter::setBomb(size_t bomb)
{
	lock();
	if (bomb <= _bombMax)
		_bomb = bomb;
	unlock();
}

void is::ACharacter::setBombMax(size_t bombMax)
{
	lock();
	_bombMax = bombMax;
	unlock();
}

void is::ACharacter::setBombLength(size_t length)
{
	lock();
	_bombLength = length;
	unlock();
}

void is::ACharacter::setSpeed(float speed)
{
	if (_speed > 4)
		return;
	lock();
	if (_speedCoef * speed < 0.24)
		_speed = speed;
	else
		_speed = 0.24 / (float) _speedCoef;

	std::cerr << _speed << std::endl;
	unlock();
}

is::ACharacter &is::ACharacter::operator++()
{
	lock();
	if (_bomb < _bombMax)
		++_bomb;
	unlock();
	return *this;
}

bool is::ACharacter::checkCollision(std::vector<std::shared_ptr<is::IEntity>> &list)
{
	bool ret = false;

	for (auto &it : list) {
		if (it.get() != this && it->isCollidable()) {
					it->collide(this);
		}
	}
	return ret;
}

void is::ACharacter::move(float nextX, float nextY, float nextZ)
{
	auto list = getEntitiesAt(nextX, nextZ);

	for (auto &it: list) {
		it->lock();
		if (it.get() != this && it->isCollidable() && !it->isWalkable(_sptr) && ((it->isWallPassable() && !_wallPass) || !it->isWallPassable())) {
			it->unlock();
			return;
		}
		it->unlock();
	} // TODO Sahel
	while (abs(getX() - nextX) > 0.01 || abs(getY() - nextY) > 0.01 || abs(getZ() - nextZ) > 0.01) {
		if (abs(getX() - nextX) > 0.01)
			setX(getX() > nextX ? getX() - 0.01 : getX() + 0.01);
		if (abs(getY() - nextY) > 0.01)
			setY(getY() > nextY ? getY() - 0.01 : getY() + 0.01);
		if (abs(getZ() - nextZ) > 0.01)
			setZ(getZ() > nextZ ? getZ() - 0.01 : getZ() + 0.01);
	}
	checkCollision(list);
}

void is::ACharacter::moveUp()
{
	if (_lastMove != MoveCharacter::UP) {
		irrl::ManageObject::setAnimation(_irrlicht, _sptr,
			irr::scene::EMAT_RUN);
		irrl::ManageObject::setRotation(_irrlicht, _sptr,
			irr::core::vector3df(0, 270, 0));
		_lastMove = MoveCharacter::UP;
	}
	_entities.lock();
	if (!dynamic_cast<AEntity *>(_sptr.get())) {
		_entities.unlock();
		return;
	}
	lock();
	float next = getZ() + _speed * _speedCoef;
	unlock();

	move(getX(), getY(), next);
	_entities.unlock();
}

void is::ACharacter::moveDown()
{
	if (_lastMove != MoveCharacter::DOWN) {
		irrl::ManageObject::setAnimation(_irrlicht, _sptr,
			irr::scene::EMAT_RUN);
		irrl::ManageObject::setRotation(_irrlicht, _sptr,
			irr::core::vector3df(0, 90, 0));
		_lastMove = MoveCharacter::DOWN;
	}
	_entities.lock();
	if (!dynamic_cast<AEntity *>(_sptr.get())) {
		_entities.unlock();
		return;
	}
	lock();
	float next = getZ() - _speed * _speedCoef;
	unlock();
	move(getX(), getY(), next);
	_entities.unlock();
}

void is::ACharacter::moveLeft()
{
	if (_lastMove != MoveCharacter::LEFT) {
		irrl::ManageObject::setAnimation(_irrlicht, _sptr,
			irr::scene::EMAT_RUN);
		irrl::ManageObject::setRotation(_irrlicht, _sptr,
			irr::core::vector3df(0, 180, 0));
		_lastMove = MoveCharacter::LEFT;
	}
	_entities.lock();
	if (!dynamic_cast<AEntity *>(_sptr.get())) {
		_entities.unlock();
		return;
	}
	lock();
	float next = getX() - _speed * _speedCoef;
	unlock();

	move(next, getY(), getZ());
	_entities.unlock();
}

void is::ACharacter::moveRight()
{
	if (_lastMove != MoveCharacter::RIGHT) {
		irrl::ManageObject::setAnimation(_irrlicht, _sptr,
			irr::scene::EMAT_RUN);
		irrl::ManageObject::setRotation(_irrlicht, _sptr,
			irr::core::vector3df(0, 0, 0));
		_lastMove = MoveCharacter::RIGHT;
	}
	_entities.lock();
	if (!dynamic_cast<AEntity *>(_sptr.get())) {
		_entities.unlock();
		return;
	}
	lock();
	float next = getX() + _speed * _speedCoef;
	unlock();
	move(next, getY(), getZ());
	_entities.unlock();
}

void is::ACharacter::dropBomb()
{
	_entities.lock();
	if (!dynamic_cast<AEntity *>(_sptr.get())) {
		_entities.unlock();
		return;
	}
	if (_bomb <= 0) {
		_entities.unlock();
		return;
	}
	float size = _irrlicht.getNodeSize(_sptr);
	auto _entitiesAt = getEntitiesAt((int)(getX() + size / 2.0), (int)(getZ() + size / 2.0));

	for (auto &it: _entitiesAt) {
		auto checkCharacter = dynamic_cast<ACharacter *>(it.get());
		auto checkPowerUp = dynamic_cast<APowerUp *>(it.get());
		if (checkCharacter == nullptr && checkPowerUp == nullptr) {
			std::cout << "Can't drop bomb" << std::endl;
			_entities.unlock();
			return;
		}
	}
	lock();
	--_bomb;
	auto bomb = new is::Bomb(_entities, _eventManager, _sptr, _irrlicht);
	std::cerr << "Bomb" << std::endl;
	bomb->setX((int)(getX() + size / 2.0));
	bomb->setY((int)(getY()));
	bomb->setZ((int)(getZ() + size / 2.0));
	unlock();
	_entities.unlock();
}

void is::ACharacter::explode()
{
	--_pv;
	if (_pv == 0) {
		_eventManager.lock();
		_eventManager->enqueue([this]{
			_entities.lock();
			if (!dynamic_cast<AEntity *>(_sptr.get())) {
				_entities.unlock();
				return;
			}
			lock();
			_locked = true;
			this->~ACharacter();
		});
		_eventManager.unlock();
	}
}

void is::ACharacter::doNothing()
{
	if (_lastMove != MoveCharacter::NOTHING) {
		irrl::ManageObject::setAnimation(_irrlicht, _sptr,
			irr::scene::EMAT_STAND);
		_lastMove = MoveCharacter::NOTHING;
	}
}

void is::ACharacter::save()
{

	lock();
	remove(".save.indie");

	std::ofstream _filestr(".save.indie");

	sleep(1);
	std::cout << "Sauvegarde" << std::endl;
	for (auto &it : _entities.get()) {
		_filestr << it->getType() << " " << it->getX() << " " << it->getY() << " " << it->getZ() << " IsPickable " << it->isPickable() << " IsWalkable " << it->isWalkable() << " Iscollidable " <<  it->isCollidable() << " isWallPassable " << it->isWallPassable();
		auto isWall = dynamic_cast<is::Wall *>(it.get());
		if (isWall != nullptr)
			_filestr << " PowerUp " << isWall->getPowerUp();
		auto isCharacter = dynamic_cast<is::ACharacter *>(it.get());
		if (isCharacter != nullptr)
			_filestr << " bombMax " << isCharacter->getBombMax() << " speed " << isCharacter->getSpeed() << " bombLength " << isCharacter->getBombLength() << " wallPass " << isCharacter->getWallPass();
		_filestr << "\n";
		//std::cout << it->getType() << " " << it->getX() << " " << it->getY() << " " << it->getZ() << " IsPickable " << it->isPickable() << " IsWalkable " << it->isWalkable() << " Iscollidable " <<  it->isCollidable() << " isWallPassable " << it->isWallPassable() << std::endl;
		//std::cout << std::endl;
		//usleep(100000);
	}
	unlock();
}

void is::ACharacter::setHP(int life)
{
	_pv = static_cast<uint>(life);
}
