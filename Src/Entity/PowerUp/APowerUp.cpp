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
	nts::ManageIrrlicht &irrlicht
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
		lock();
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
			_entities.unlock();
			return;
		}
		action(character);
		//_entities.unlock();
		_eventManager.lock();
		_eventManager->enqueue([this]{
			this->~APowerUp();
		});
		_eventManager.unlock();
	}
}

void is::APowerUp::explode()
{
	_eventManager.lock();
	_eventManager->enqueue([this]{
		this->~APowerUp();
	});
	_eventManager.unlock();
}

void is::APowerUp::texture()
{
	nts::ManageObject::createCube(_irrlicht, _sptr, 1);
	nts::ManageObject::setMaterialLight(_irrlicht, _sptr, false);
	nts::ManageObject::setTexture(_irrlicht, _sptr, "media/help.png");
}
