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


is::Wall::Wall(
	my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> &entities,
	my::ItemLocker<my::ThreadPool> &eventManager
):
	AEntity(entities, eventManager)
{
	_type = "Wall";

	_collidable = true;
	_pickable = false;
	_walkable = false;
	_wallPassable = true;
}

void is::Wall::placePowerUp()
{
	switch (_powerUp) {
	case 'b':
		is::BombUp(_entities, _eventManager).setPosition(_position);
		break;
	case 'f':
		is::FireUp(_entities, _eventManager).setPosition(_position);
		break;
	case 's':
		is::SpeedUp(_entities, _eventManager).setPosition(_position);
		break;
	case 'w':
		is::WallPass(_entities, _eventManager).setPosition(_position);
		break;
	default:
		break;
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