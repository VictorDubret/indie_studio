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

void is::BombUp::action(is::ACharacter *character)
{
	character->setBombMax(character->getBombMax() + 1);
	character->operator++();
	Debug::debug("Character have now ", character->getBombMax(), " bombMax");
}

