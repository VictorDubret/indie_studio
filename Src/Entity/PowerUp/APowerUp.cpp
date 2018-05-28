/*
** EPITECH PROJECT, 2018
** cpp_indie_studio
** File description:
** Created by sahel.lucas-saoudi@epitech.eu,
*/

#include "APowerUp.hpp"
#include "ACharacter.hpp"
#include "Debug.hpp"

is::APowerUp::APowerUp(
	my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> &entities,
	my::ItemLocker<my::ThreadPool> &eventManager
):
	AEntity(entities, eventManager)
{
	_pickable = true;
	_type = "PowerUp";
}



void is::APowerUp::collide(is::IEntity *entity)
{
	auto character = dynamic_cast<ACharacter *>(entity);

	if (character) {
		Debug::debug("Character take ", _type, " in ", _position.x , ", ", _position.y, ", ", _position.z);
		action(character);
		this->~APowerUp();
	}
}

// Do nothing
void is::APowerUp::action(is::ACharacter *)
{
}
