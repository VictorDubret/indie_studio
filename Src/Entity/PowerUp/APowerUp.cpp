/*
** EPITECH PROJECT, 2018
** cpp_indie_studio
** File description:
** Created by sahel.lucas-saoudi@epitech.eu,
*/

#include "APowerUp.hpp"
#include "ACharacter.hpp"
#include "Debug.hpp"
#include "ManageIrrlicht.hpp"
#include "ManageObject.hpp"

is::APowerUp::APowerUp(
	my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> &entities,
	my::ItemLocker<my::ThreadPool> &eventManager,
	irrl::ManageIrrlicht &irrlicht
):
	AEntity(entities, eventManager, irrlicht)
{
	_pickable = true;
	_type = "PowerUp";
}

is::APowerUp::~APowerUp()
{
	if (!_locked) {
		_entities.lock();
		_mutex.lock();
	}
	_locked = true;
}

void is::APowerUp::collide(is::IEntity *entity)
{
	auto character = dynamic_cast<ACharacter *>(entity);

	if (character) {
		//_entities.lock();
		auto tmp = dynamic_cast<is::AEntity *>(entity);
		if (!tmp) {
			//_entities.unlock();
			return;
		}
		action(character);
		//_entities.unlock();
		/*_eventManager.lock();
		_eventManager->enqueue([this]{*/
			_entities.lock();
			if (!dynamic_cast<APowerUp *>(_spointer.get())) {
				_entities.unlock();
				return;
			}
			_mutex.lock();
			_locked = true;
			this->~APowerUp();
		/*});
		_eventManager.unlock();*/
	}
}

void is::APowerUp::explode()
{
	/*_eventManager.lock();
	_eventManager->enqueue([this]{*/
		_entities.lock();
		if (!dynamic_cast<APowerUp *>(_spointer.get())) {
			_entities.unlock();
			return;
		}
		_mutex.lock();
		_locked = true;
		this->~APowerUp();
	/*});
	_eventManager.unlock();*/
}

void is::APowerUp::texture()
{
	irrl::ManageObject::createCube(_irrlicht, _spointer, 1);
	irrl::ManageObject::setMaterialLight(_irrlicht, _spointer, false);
	irrl::ManageObject::setTexture(_irrlicht, _spointer, "media/help.png");
}
