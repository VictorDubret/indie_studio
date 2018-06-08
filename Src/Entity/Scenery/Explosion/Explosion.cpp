
/*
** EPITECH PROJECT, 2018
** cpp_indie_studio
** File description:
** Created by guillaume.berdeaux@epitech.eu
*/

#include <Tools/Debug.hpp>
#include "ManageIrrlicht.hpp"
#include "Explosion.hpp"
#include "ManageObject.hpp"

is::Explosion::Explosion(
	my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> &entities,
	my::ItemLocker<my::ThreadPool> &eventManager, nts::ManageIrrlicht &irrlicht
):
	AEntity(entities, eventManager, irrlicht)
{
	_type = "Explosion";

	_irrlicht.getSoundDevice()->play2D("media/explode.ogg", false, false, true, irrklang::ESM_AUTO_DETECT, true);
	_collidable = true;
	_pickable = false;
	_walkable = true;
	_wallPassable = true;
	texture();
	_eventManager.lock();
	_eventManager->enqueue([this]() {
		do {
			std::this_thread::sleep_for(std::chrono::seconds(1));
		} while(_isPaused);
		this->~Explosion();
	});
	_eventManager.unlock();
}

is::Explosion::~Explosion()
{
	if (!_locked) {
		_entities.lock();
		lock();
	}
	_locked = true;
}

void is::Explosion::texture()
{
	nts::ManageObject::createAnim(_irrlicht, _sptr, "media/explosion.b3d",
		0.999);
	nts::ManageObject::setScale(_irrlicht, _sptr,
		irr::core::vector3df(1.0, 1.0, 1.0));
	nts::ManageObject::setRotation(_irrlicht, _sptr,
		irr::core::vector3df(0, 0, 0));
	nts::ManageObject::setMaterialLight(_irrlicht, _sptr, false);
	nts::ManageObject::setAnimation(_irrlicht, _sptr, irr::scene::EMAT_RUN);
	nts::ManageObject::setTexture(_irrlicht, _sptr, "media/fire.jpg");
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

void is::Explosion::setIsPaused(const bool isPaused)
{
	_isPaused = isPaused;
}
