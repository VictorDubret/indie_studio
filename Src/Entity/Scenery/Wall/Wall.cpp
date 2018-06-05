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
	my::ItemLocker<my::ThreadPool> &eventManager, nts::ManageIrrlicht &irrlicht
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
	if (!_locked)
		_entities.lock();
	_locked = true;
}

void is::Wall::placePowerUp()
{
	is::APowerUp *powerUp = nullptr;
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
	if (powerUp) {
		powerUp->setPosition(getPosition());
	}
}

void is::Wall::explode()
{
	placePowerUp();
	std::cerr << "Explode " << _type << std::endl;
	this->~Wall();
}

void is::Wall::setPowerUp(char powerUp)
{
	_powerUp = powerUp;
}

void is::Wall::texture()
{
	nts::ManageObject::createCube(_irrlicht, _sptr, 0.9999);
	nts::ManageObject::setMaterialLight(_irrlicht, _sptr, false);
	nts::ManageObject::setTexture(_irrlicht, _sptr, "media/stones.jpg");
}