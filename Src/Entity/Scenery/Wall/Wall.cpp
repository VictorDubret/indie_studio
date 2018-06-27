/*
** EPITECH PROJECT, 2018
** cpp_indie_studio
** File description:
** Created by guillaume.berdeaux@epitech.eu
*/

#include <Debug.hpp>
#include "Wall.hpp"
#include "BombUp.hpp"
#include "FireUp.hpp"
#include "SpeedUp.hpp"
#include "WallPass.hpp"
#include "ManageIrrlicht.hpp"
#include "ManageObject.hpp"

is::Wall::Wall(
	my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> &entities,
	my::ItemLocker<my::ThreadPool> &eventManager, irrl::ManageIrrlicht &irrlicht
):
	AEntity(entities, eventManager, irrlicht)
{
	_type = "Wall";

	_collidable = true;
	_pickable = false;
	_walkable = false;
	_wallPassable = true;
	texture();
}

is::Wall::~Wall()
{
	if (!_locked) {
		_entities.lock();
		lock();
	}
	_locked = true;
}

void is::Wall::placePowerUp(irr::core::vector3df pos)
{
	is::APowerUp *powerUp = nullptr;
	_entities.lock();
	switch (_powerUp) {
	case 'b':
		powerUp = new is::BombUp(_entities, _eventManager, _irrlicht);
		break;
	case 'f':
		powerUp = new is::FireUp(_entities, _eventManager, _irrlicht);
		break;
	case 's':
		powerUp = new is::SpeedUp(_entities, _eventManager, _irrlicht);
		break;
	case 'w':
		powerUp = new is::WallPass(_entities, _eventManager, _irrlicht);
		break;
	default:
		break;
	}
	if (powerUp && dynamic_cast<AEntity *>(_spointer.get())) {
 		powerUp->setPosition(getPosition());
	}
	_entities.unlock();
}

void is::Wall::explode()
{
	irr::core::vector3df pos = getPosition();
	_eventManager->enqueue([this, pos]{
		placePowerUp(pos);
		_entities.lock();
		if (!dynamic_cast<is::Wall *>(_spointer.get())) {
			_entities.unlock();
			return;
		}
		this->lock();
		_locked = true;
		this->~Wall();
	});
}

void is::Wall::setPowerUp(char powerUp)
{
	_powerUp = powerUp;
}

void is::Wall::texture()
{
	irrl::ManageObject::createCube(_irrlicht, _spointer, 0.9999);
	irrl::ManageObject::setMaterialLight(_irrlicht, _spointer, false);
	irrl::ManageObject::setTexture(_irrlicht, _spointer, "media/caisse.png");
}

char is::Wall::getPowerUp() const
{
	return _powerUp;
}
