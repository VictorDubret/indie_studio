/*
** EPITECH PROJECT, 2018
** cpp_indie_studio
** File description:
** Created by sahel.lucas-saoudi@epitech.eu,
*/

#include "FireUp.hpp"
#include "Debug.hpp"

is::FireUp::FireUp(
	my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> &entities,
	my::ItemLocker<my::ThreadPool> &eventManager
) : APowerUp(entities, eventManager)
{
	_type = "FireUp";
}

void is::FireUp::action(is::ACharacter *character)
{
	character->setBombLength(character->getBombLength() + 1);
	Debug::debug("Character have now ", character->getBombLength(), " range");
}