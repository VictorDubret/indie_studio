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

	_collidable = true;
	_pickable = false;
	_walkable = true;
	_wallPassable = true;
	texture();
	_eventManager.lock();
	_eventManager->enqueue([this]() {
		std::this_thread::sleep_for(std::chrono::seconds(1));
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
	nts::ManageObject::createCube(_irrlicht, _sptr, 0.9999);
	nts::ManageObject::setMaterialLight(_irrlicht, _sptr, false);
	nts::ManageObject::setTexture(_irrlicht, _sptr, "media/water.jpg");
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