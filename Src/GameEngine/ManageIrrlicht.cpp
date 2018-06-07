/*
** EPITECH PROJECT, 2018
** test
** File description:
** Created by martin.januario@epitech.eu,
*/

#include <irrTypes.h>
#include "ManageIrrlicht.hpp"

nts::ManageIrrlicht::ManageIrrlicht(
	my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> &entities,
	my::ItemLocker<my::ThreadPool> &eventManager,
	irr::core::vector2di mapSize, bool splitScreen
) : AManageIrrlicht(entities, eventManager), Game(entities, eventManager, mapSize, splitScreen), GUI(entities, eventManager)
{
	_base = this;

	_thread = new my::Thread([&]() {
		while (!_stopThread && _device) {
			manageEvent();
			std::this_thread::sleep_for(
				std::chrono::milliseconds(50));
		}
	});
}

void nts::ManageIrrlicht::loopDisplay()
{
	while (_device && _device->run()) {
		lock();
		if (_displayGUI) {
			_driver->beginScene(true, true, irr::video::SColor(255, 115, 214, 210));
			drawGUI();
		} else if (_splitScreen) {
			displaySplitScreen();
		} else {
			_driver->beginScene(true, true, irr::video::SColor(255, 100, 100, 100));
			_sceneManager->drawAll();
			setCameraPos();
		}
		unlock();
		//fin test
		displayFPS();

		_driver->endScene();
		std::this_thread::yield();
	}
}

void nts::ManageIrrlicht::manageEvent()
{
	if (_eventReceiver.IsKeyDown(irr::KEY_ESCAPE)) {
		_displayGUI = true;
	} else if (_displayGUI) {
		manageEventGui();
	} else
		manageEventPlayers();
}

void nts::ManageIrrlicht::lock()
{
	_mutex.lock();
}

void nts::ManageIrrlicht::unlock()
{
	_mutex.unlock();
}

void nts::ManageIrrlicht::displayGui(bool display)
{
	_displayGUI = display;
}