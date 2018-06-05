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
	setScene(0);

	_scene[_currentScene]->addImage(getDriver()->getTexture("media/Bombermon.png"), irr::core::position2d<irr::s32>(0, 0));

//	loadImage("title", 0, "media/Bombermon.png", irr::core::position2d<irr::s32>(0, 0), irr::core::dimension2d<irr::u32>(711, 101), -1);


	irr::gui::IGUIFont *font = _scene[_currentScene]->getFont("media/fontlucida.png");
	irr::gui::IGUIButton *bouton = _scene[_currentScene]->addButton(irr::core::rect<irr::s32>(100,80,200,120), 0, -1, L"NICK TAMERE");

	bouton->setOverrideFont(font);                  // changement de police

	bouton->setPressedImage(getDriver()->getTexture("media/fire.jpg"));

	irr::gui::IGUIEditBox *editBox = _scene[_currentScene]->addEditBox(L"Texte de depart", irr::core::rect<irr::s32>(20,20,320,70));
}

void nts::GUI::setScene(int scene)
{
	if (_scene.find(scene) == _scene.end()) {
		_scene[scene] = getSceneManager()->getGUIEnvironment();
		if (!_scene[scene])
			throw std::exception();
	}
	_currentScene = scene;
}

void nts::GUI::drawGUI()
{
	_scene[_currentScene]->drawAll();
}

void nts::GUI::manageEventGui()
{
}