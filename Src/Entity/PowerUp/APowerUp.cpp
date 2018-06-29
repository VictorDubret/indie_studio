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
) : AEntity(entities, eventManager, irrlicht)
{
	_pickable = true;
	_type = "PowerUp";
}

is::APowerUp::~APowerUp()
{

	_locked = true;
}

void is::APowerUp::collide(is::IEntity *entity)
{
	auto character = dynamic_cast<ACharacter *>(entity);

	if (character) {
		auto tmp = dynamic_cast<is::AEntity *>(entity);
		if (!tmp) {
			return;
		}
		action(character);
		character->setScore(character->getScore() + _score);
		if (!dynamic_cast<APowerUp *>(_spointer.get())) {
			return;
		}
		this->~APowerUp();
	}
}

void is::APowerUp::explode()
{
	if (!dynamic_cast<APowerUp *>(_spointer.get())) {
		return;
	}
	this->~APowerUp();
}

void is::APowerUp::texture()
{
	irrl::ManageObject::createCube(_irrlicht, _spointer, 1);
	irrl::ManageObject::setMaterialLight(_irrlicht, _spointer, false);
	irrl::ManageObject::setTexture(_irrlicht, _spointer, "media/help.png");
}
