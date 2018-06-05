/*
** EPITECH PROJECT, 2018
** test
** File description:
** Created by martin.januario@epitech.eu,
*/

#include "ManageIrrlicht.hpp"

nts::ManageIrrlicht::ManageIrrlicht(my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> &entities, my::ItemLocker<my::ThreadPool> &eventManager, irr::core::vector2di mapSize, bool splitScreen) : AManageIrrlicht(entities, eventManager), Game(entities, eventManager, mapSize, splitScreen), GUI(entities, eventManager)
{
}

void nts::ManageIrrlicht::loopDisplay()
{
	while (_device && _device->run()) {
		if (_displayGUI) {
			_driver->beginScene(true, true, irr::video::SColor(255, 115, 214, 210));
			drawGUI();
		} else if (_splitScreen) {
			displaySplitScreen();
		} else {
			_driver->beginScene(true,true,irr::video::SColor(255,100,100,100));
			_sceneManager->drawAll();
			//setCameraPos();
		}
		//fin test
		displayFPS();

		_driver->endScene();
		manageEvent();
	}
}

void nts::ManageIrrlicht::manageEvent()
{
	if (_eventReceiver.IsKeyDown(irr::KEY_ESCAPE)) {
		_device->closeDevice();
		delete _device;
		_device = nullptr;
	} else if (_displayGUI)
		manageEventGui();
	 else
		manageEventPlayers();
}

void nts::ManageIrrlicht::displayGui(bool display)
{
	_displayGUI = display;
}