/*
** EPITECH PROJECT, 2018
** cpp_indie_studio
** File description:
** Created by sahel.lucas-saoudi@epitech.eu,
*/

#include "AAI.hpp"

is::AAI::AAI(
	my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> &entities,
	my::ItemLocker<my::ThreadPool> &eventManager, irrl::ManageIrrlicht &irrlicht
) : ACharacter(entities, eventManager, irrlicht)
{
}

void is::AAI::ia()
{

}