/*
** EPITECH PROJECT, 2018
** cpp_indie_studio
** File description:
** Created by sahel.lucas-saoudi@epitech.eu,
*/

#include "ThreadPool.hpp"
#include "ACharacter.hpp"
#include "Debug.hpp"
#include "Bomb.hpp"
#include "ManageObject.hpp"

is::ACharacter::ACharacter(my::ItemLocker<std::vector<std::shared_ptr<IEntity>>> &entities, my::ItemLocker<my::ThreadPool> &eventManager, nts::ManageIrrlicht &irrlicht) :
	AEntity(entities, eventManager, irrlicht)
{
	_type = "Character";
	_walkable = false;
	texture();
}

void is::ACharacter::texture()
{
	nts::ManageObject::createAnim(_irrlicht, _sptr, "media/sydney.md2", 0.8);
	_irrlicht.getNode(_sptr)->setPosition(irr::core::vector3df(1.02f, 0.1f, 1.02f));
	nts::ManageObject::setScale(_irrlicht, _sptr, irr::core::vector3df(0.05, 0.05 , 0.05));
	nts::ManageObject::setRotation(_irrlicht, _sptr, irr::core::vector3df(0, 90, 0));
	nts::ManageObject::setMaterialLight(_irrlicht, _sptr, false);
	nts::ManageObject::setAnimation(_irrlicht, _sptr, irr::scene::EMAT_RUN);
	nts::ManageObject::setTexture(_irrlicht, _sptr, "media/sydney.bmp");
}

is::ACharacter::~ACharacter()
{
	std::cout << "NIKE TA MERE CONNARD" << std::endl;
	this->unlock();
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

void is::ACharacter::setSpeed(size_t speed)
{
	lock();
	_speed = speed;
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

bool is::ACharacter::checkCollision()
{
	bool ret = false;
	auto list = getEntitiesAt(getX(), getY(), getZ());

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
		std::cout << getType() << " collidable: " << it->isCollidable() << " walkable: " << it->isWalkable() <<
			" wallpassable: " << it->isWallPassable() << std::endl;
		if (it.get() != this && it->isCollidable() && !it->isWalkable() && ((it->isWallPassable() && !_wallPass) || !it->isWallPassable())) {
			std::cout << "COLLIDE" << std::endl;
			return;
		}
	}
	_entities.lock();
	setZ(nextZ);
	setY(nextY);
	setX(nextX);
	_entities.unlock();
	checkCollision();
}

void is::ACharacter::moveUp()
{
	if (_lastMove != MoveCharacter::UP) {
		nts::ManageObject::setAnimation(_irrlicht, _sptr, irr::scene::EMAT_RUN);
		nts::ManageObject::setRotation(_irrlicht, _sptr, irr::core::vector3df(0, 270, 0));
		_lastMove = MoveCharacter::UP;
	}
	float next = getZ() + _speed * _speedCoef;

	move(getX(), getY(), next);
}

void is::ACharacter::moveDown()
{
	if (_lastMove != MoveCharacter::DOWN) {
		nts::ManageObject::setAnimation(_irrlicht, _sptr, irr::scene::EMAT_RUN);
		nts::ManageObject::setRotation(_irrlicht, _sptr, irr::core::vector3df(0, 90, 0));
		_lastMove = MoveCharacter::DOWN;
	}
	float next = getZ() - _speed * _speedCoef;

	move(getX(), getY(), next);
}

void is::ACharacter::moveLeft()
{
	if (_lastMove != MoveCharacter::LEFT) {
		nts::ManageObject::setAnimation(_irrlicht, _sptr, irr::scene::EMAT_RUN);
		nts::ManageObject::setRotation(_irrlicht, _sptr, irr::core::vector3df(0, 180, 0));
		_lastMove = MoveCharacter::LEFT;
	}
	float next = getX() - _speed * _speedCoef;

	move(next, getY(), getZ());
}

void is::ACharacter::moveRight()
{
	if (_lastMove != MoveCharacter::RIGHT) {
		nts::ManageObject::setAnimation(_irrlicht, _sptr, irr::scene::EMAT_RUN);
		nts::ManageObject::setRotation(_irrlicht, _sptr, irr::core::vector3df(0, 0, 0));
		_lastMove = MoveCharacter::RIGHT;
	}
	float next = getX() + _speed * _speedCoef;

	move(next, getY(), getZ());
}

void is::ACharacter::dropBomb()
{
	Debug::debug("DROP BOMB");
	if (_bomb > 0) {

		auto _entitiesAt = getEntitiesAt(getX(), getY(), getZ());

		_entities.lock();
		for (auto &it: _entitiesAt) {
			auto checkCharacter = dynamic_cast<ACharacter *>(it.get());
			auto checkPowerUp = dynamic_cast<ACharacter *>(it.get());
			if (checkCharacter == nullptr && checkPowerUp == nullptr) {
				std::cout << "Can't drop bomb" << std::endl;
				_entities.unlock();
				return;
			}
		}
		lock();
		--_bomb;
		unlock();
		_entities.unlock();
		auto bomb = new is::Bomb(_entities, _eventManager, _sptr, _irrlicht);
		std::cerr << "Bomb" << std::endl;
		bomb->setX((int) getX());
		bomb->setY((int) getY());
		bomb->setZ((int) getZ());
	}
}

void is::ACharacter::explode()
{
	lock();
	//--_pv; //TODO uncomment
	unlock();
	if (_pv == 0) {
		Debug::debug("A player die");
		this->~ACharacter();
	}
}

void is::ACharacter::doNothing()
{
	if (_lastMove != MoveCharacter::NOTHING) {
		nts::ManageObject::setAnimation(_irrlicht, _sptr, irr::scene::EMAT_STAND);
		_lastMove = MoveCharacter::NOTHING;
	}
}