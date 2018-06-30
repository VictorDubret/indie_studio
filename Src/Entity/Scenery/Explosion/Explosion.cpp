
/*
** EPITECH PROJECT, 2018
** cpp_indie_studio
** File description:
** Created by guillaume.berdeaux@epitech.eu
*/

#include <ctime>
#include <Tools/Debug.hpp>
#include "ManageIrrlicht.hpp"
#include "Explosion.hpp"
#include "ManageObject.hpp"

is::Explosion::Explosion(
	my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> &entities,
	my::ItemLocker<my::ThreadPool> &eventManager, irrl::ManageIrrlicht &irrlicht
):
	AEntity(entities, eventManager, irrlicht)
{
	_type = "Explosion";

	if (!_irrlicht.getDisplayGUI())
		_irrlicht.getSoundDevice()->play2D("media/explode.ogg", false, false, true, irrklang::ESM_AUTO_DETECT, true);
	_collidable = true;
	_pickable = false;
	_walkable = true;
	_wallPassable = true;
	texture();
	_startedAt = time(nullptr);
	_lastTime = _startedAt;
}

is::Explosion::~Explosion()
{
	_locked = true;
}

void is::Explosion::texture()
{
	irrl::ManageObject::createAnim(_irrlicht, _spointer, "media/explosion.b3d",
		0.999);
	irrl::ManageObject::setScale(_irrlicht, _spointer,
		irr::core::vector3df(1.0, 1.0, 1.0));
	irrl::ManageObject::setRotation(_irrlicht, _spointer,
		irr::core::vector3df(0, 0, 0));
	irrl::ManageObject::setMaterialLight(_irrlicht, _spointer, false);
	irrl::ManageObject::setAnimation(_irrlicht, _spointer, irr::scene::EMAT_RUN);
	irrl::ManageObject::setTexture(_irrlicht, _spointer, "media/fire.jpg");
}

void is::Explosion::explode()
{
}

void is::Explosion::collide(is::IEntity *entity)
{
	if (entity) {
		if (dynamic_cast<is::ACharacter *>(entity) == nullptr)
			return ;
		entity->explode();
	}
}

void is::Explosion::timer()
{
	long int now = time(nullptr);

	if (_isPaused && now > _lastTime) {
		_time++;
	}
	if (now >= _startedAt + _time) {
		this->~Explosion();
	}
}

void is::Explosion::setIsPaused(const bool isPaused)
{
	_isPaused = isPaused;
}
