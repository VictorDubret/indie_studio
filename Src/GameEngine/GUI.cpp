/*
** EPITECH PROJECT, 2018
** bomberman
** File description:
** Created by martin.januario@epitech.eu,
*/

#include <iostream>
#include "GUI.hpp"

nts::GUI::GUI(my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> &entities, my::ItemLocker<my::ThreadPool> &eventManager) : AManageIrrlicht(entities, eventManager)
{
	std::cout << "Hey ! I'm in the GUI section" << std::endl;
}

void nts::GUI::drawGUI()
{
}

void nts::GUI::manageEventGui()
{
}