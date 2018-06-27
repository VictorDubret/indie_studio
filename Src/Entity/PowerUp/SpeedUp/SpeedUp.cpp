/*
** EPITECH PROJECT, 2018
** cpp_indie_studio
** File description:
** Created by sahel.lucas-saoudi@epitech.eu,
*/

#include "SpeedUp.hpp"
#include "Debug.hpp"

is::SpeedUp::SpeedUp(
	my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> &entities,
	my::ItemLocker<my::ThreadPool> &eventManager, irrl::ManageIrrlicht &irrlicht
) : APowerUp(entities, eventManager, irrlicht)
{
	_type = "SpeedUp";
	texture();
}

is::SpeedUp::~SpeedUp()
{
	if (!_locked) {
		_entities.lock();
		lock();
	}
	_locked = true;
}

void is::SpeedUp::texture()
{
	irrl::ManageObject::createAnim(_irrlicht, _spointer, "media/speedup.b3d",
		0.75);
	_irrlicht.getNode(_spointer.get())->setPosition(
		irr::core::vector3df(1.1f, 0.1f, 1.1f));
	irrl::ManageObject::setScale(_irrlicht, _spointer,
		irr::core::vector3df(1, 1, 1));
	irrl::ManageObject::setRotation(_irrlicht, _spointer,
		irr::core::vector3df(30, 0, 0));
	irrl::ManageObject::setMaterialLight(_irrlicht, _spointer, false);
	irrl::ManageObject::setAnimation(_irrlicht, _spointer, irr::scene::EMAT_RUN);
	irrl::ManageObject::setTexture(_irrlicht, _spointer, "media/map_ob000_item_AL.png");
}

void is::SpeedUp::action(is::ACharacter *character)
{
	_irrlicht.getSoundDevice()->play2D("media/sound/PowerUp/speedup.wav", false, false, true, irrklang::ESM_AUTO_DETECT, true);
	character->setSpeed(character->getSpeed() + 0.2);
	Debug::debug("Character have now ", character->getSpeed(), " speed");
}
