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

	_scene[MENU]->addImage(getDriver()->getTexture("media/Bombermon.png"), irr::core::position2d<irr::s32>(0, 0));

//	loadImage("title", 0, "media/Bombermon.png", irr::core::position2d<irr::s32>(0, 0), irr::core::dimension2d<irr::u32>(711, 101), -1);

	addButton(MENU, L"Bonjour", irr::core::rect<irr::s32>(0, 100, 100, 200));
	addButton(MENU, L"Aurevoir", irr::core::rect<irr::s32>(0, 200, 100, 300));
	irr::gui::IGUIEditBox *editBox = _scene[MENU]->addEditBox(L"Texte de depart", irr::core::rect<irr::s32>(20,20,320,70));
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

void nts::GUI::addButton(int scene, wchar_t *text, const irr::core::rect<irr::s32> &textRect)
{
	irr::gui::IGUIFont *font = _scene[MENU]->getFont("media/fontlucida.png");
	irr::gui::IGUIButton *bouton = _scene[scene]->addButton(textRect, 0, -1, text);
	bouton->setOverrideFont(font);

	bouton->setPressedImage(getDriver()->getTexture("media/fire.jpg"));
}
