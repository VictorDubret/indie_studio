/*
** EPITECH PROJECT, 2018
** cpp_indie_studio
** File description:
** Created by sahel.lucas-saoudi@epitech.eu,
*/

#include "WallPass.hpp"
#include "Debug.hpp"
#include "ManageObject.hpp"

is::WallPass::WallPass(
	my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> &entities,
	my::ItemLocker<my::ThreadPool> &eventManager, nts::ManageIrrlicht &irrlicht
) : APowerUp(entities, eventManager, irrlicht)
{
	_type = "WallPass";
	texture();
}

is::WallPass::~WallPass()
{
	if (!_locked) {
		_entities.lock();
		lock();
	}
	_locked = true;
}


void is::WallPass::texture()
{
	nts::ManageObject::createAnim(_irrlicht, _sptr, "media/wallpass.b3d",
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

void is::WallPass::action(is::ACharacter *character)
{
	_irrlicht.getSoundDevice()->play2D("media/sound/PowerUp/wallpass.wav", false, false, true, irrklang::ESM_AUTO_DETECT, true);
	character->setWallPass(true);
	Debug::debug("Character have now WallPass ON");
}
