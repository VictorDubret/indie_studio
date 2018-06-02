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



void is::APowerUp::collide(is::IEntity *entity)
{
	auto character = dynamic_cast<ACharacter *>(entity);

	if (character) {
		Debug::debug("Character take ", _type, " in ", _irrlicht.getNode(_sptr)->getPosition().X , ", ", _irrlicht.getNode(_sptr)->getPosition().Y, ", ", _irrlicht.getNode(_sptr)->getPosition().Z);
		action(character);
		this->~APowerUp();
	}
}

void is::APowerUp::explode()
{
	this->~APowerUp();
}

void is::APowerUp::texture()
{
	nts::ManageObject::createCube(_irrlicht, _sptr, 1);
	_irrlicht.getNode(_sptr)->setPosition(irr::core::vector3df(0, 0, 0));
	nts::ManageObject::setMaterialLight(_irrlicht, _sptr, false);
	nts::ManageObject::setTexture(_irrlicht, _sptr, "media/help.png");
}
