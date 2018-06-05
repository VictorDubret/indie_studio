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
	if (!_locked)
		_entities.lock();
	_locked = true;
}

void is::APowerUp::collide(is::IEntity *&entity)
{
	auto character = dynamic_cast<ACharacter *>(entity);

	if (character) {
		std::cout << RED << __PRETTY_FUNCTION__ << " LOCK" << RESET << std::endl;
		_entities.lock();
		Debug::debug("Character take ", _type, " in ", _irrlicht.getNode(_sptr)->getPosition().X , ", ", _irrlicht.getNode(_sptr)->getPosition().Y, ", ", _irrlicht.getNode(_sptr)->getPosition().Z);
		action(character);
		_entities.unlock(); std::cout << GRN << __PRETTY_FUNCTION__ << " UNLOCK" << RESET << std::endl;
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
	nts::ManageObject::setMaterialLight(_irrlicht, _sptr, false);
	nts::ManageObject::setTexture(_irrlicht, _sptr, "media/help.png");
}
