/*
** EPITECH PROJECT, 2018
** cpp_indie_studio
** File description:
** Created by sahel.lucas-saoudi@epitech.eu,
*/

#include "WallPass.hpp"
#include "Debug.hpp"

is::WallPass::WallPass(
	my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> &entities,
	my::ItemLocker<my::ThreadPool> &eventManager
) : APowerUp(entities, eventManager)
{
	_type = "WallPass";
}

void is::WallPass::action(is::ACharacter *character)
{
	character->setWallPass(true);
	Debug::debug("Character have now WallPass ON");
}
