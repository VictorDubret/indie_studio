/*
** EPITECH PROJECT, 2018
** cpp_indie_studio
** File description:
** Created by sahel.lucas-saoudi@epitech.eu,
*/

#include "ACharacter.hpp"
#include "BombUp.hpp"
#include "Debug.hpp"

is::BombUp::BombUp(
	my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> &entities,
	my::ItemLocker<my::ThreadPool> &eventManager, nts::ManageIrrlicht &irrlicht
) : APowerUp(entities, eventManager, irrlicht)
{
	_type = "BombUp";
	texture();
}

is::BombUp::~BombUp()
{
	if (!_locked) {
		_entities.lock();
		lock();
	}
	_locked = true;
}

void is::BombUp::texture()
{
	nts::ManageObject::createAnim(_irrlicht, _sptr, "media/bombup.b3d",
		0.75);
	_irrlicht.getNode(_sptr.get())->setPosition(
		irr::core::vector3df(1.1f, 0.1f, 1.1f));
	nts::ManageObject::setScale(_irrlicht, _sptr,
		irr::core::vector3df(1, 1, 1));
	nts::ManageObject::setRotation(_irrlicht, _sptr,
		irr::core::vector3df(30, 0, 0));
	nts::ManageObject::setMaterialLight(_irrlicht, _sptr, false);
	nts::ManageObject::setAnimation(_irrlicht, _sptr, irr::scene::EMAT_RUN);
	nts::ManageObject::setTexture(_irrlicht, _sptr, "media/map_ob000_item_AL.png");
}

void is::BombUp::action(is::ACharacter *character)
{
	_irrlicht.getSoundDevice()->play2D("media/sound/PowerUp/bombup.wav", false, false, true, irrklang::ESM_AUTO_DETECT, true);
	character->setBombMax(character->getBombMax() + 1);
	character->operator++();
	Debug::debug("Character have now ", character->getBombMax(), " bombMax");
}

