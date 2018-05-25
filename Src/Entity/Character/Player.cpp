/*
** EPITECH PROJECT, 2018
** cpp_indie_studio
** File description:
** Created by sahel.lucas-saoudi@epitech.eu,
*/

#include "Player.hpp"

is::Player::Player(std::vector<std::shared_ptr<IEntity>> &entities, my::ThreadPool &eventManager):
	ACharacter(entities, eventManager)
{
}
