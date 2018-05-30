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
	my::ItemLocker<my::ThreadPool> &eventManager, nts::ManageIrrlicht &irrlicht
) : APowerUp(entities, eventManager, irrlicht)
{
	_type = "SpeedUp";
}

void is::SpeedUp::action(is::ACharacter *character)
{
	character->setSpeed(character->getSpeed() + 1);
	Debug::debug("Character have now ", character->getSpeed(), " speed");
}
