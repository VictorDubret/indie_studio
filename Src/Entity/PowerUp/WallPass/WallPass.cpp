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
	if (!_locked)
		_entities.lock();
	_locked = true;
}

/*
void is::WallPass::texture()
{
	nts::ManageObject::createAnim(_irrlicht, _sptr, "media/PokeBalls/MasterBall.SMD", 0.8);
	_irrlicht.getNode(_sptr)->setPosition(irr::core::vector3df(1.02f, 0.1f, 1.02f));
	nts::ManageObject::setScale(_irrlicht, _sptr, irr::core::vector3df(0.1, 0.1 , 0.1));
	nts::ManageObject::setRotation(_irrlicht, _sptr, irr::core::vector3df(0, 90, 0));
	nts::ManageObject::setMaterialLight(_irrlicht, _sptr, false);
	nts::ManageObject::setAnimation(_irrlicht, _sptr, irr::scene::EMAT_RUN);
	nts::ManageObject::setTexture(_irrlicht, _sptr, "media/PokeBalls/images/ob0225_00.png");
}*/

void is::WallPass::action(is::ACharacter *character)
{
	character->setWallPass(true);
	Debug::debug("Character have now WallPass ON");
}
