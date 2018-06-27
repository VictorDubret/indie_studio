/*
** EPITECH PROJECT, 2018
** test
** File description:
** Created by martin.januario@epitech.eu,
*/

#include <chrono>
#include <irrTypes.h>
#include "ManageIrrlicht.hpp"

irrl::ManageIrrlicht::ManageIrrlicht(
	my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> &entities,
	my::ItemLocker<my::ThreadPool> &eventManager,
	irr::core::vector2di mapSize, bool splitScreen
) : AManageIrrlicht(entities, eventManager, *this), Game(entities, eventManager, *this, mapSize, splitScreen), GUI(entities, eventManager, *this)
{
	_thread = new my::Thread([&]() {
		while (!_stopThread && _device) {
			manageEvent();
			std::this_thread::sleep_for(
				std::chrono::milliseconds(50));
		}
	});
	_sound = getSoundDevice()->play2D("media/sound/opening.ogg", false, false, true, irrklang::ESM_AUTO_DETECT, true);
	wchar_t tmp[1024];
	swprintf(tmp, 1024, L"Bomberman (%ls)",
		_driver->getName());
	if (_device && _device->run()) {
		_device->setWindowCaption(tmp);
	}
}

void irrl::ManageIrrlicht::loopDisplay()
{
	while (_device && _device->run()) {
		if (_displayGUI) {
			if (_currentScene != "pause")
				endPause();
			lock();
			_driver->setViewPort(
				irr::core::rect<irr::s32>(0, 0, 1600, 900));
			_driver->beginScene(true, true,
				irr::video::SColor(255, 115, 214, 210));
			drawGUI();
			_driver->endScene();
			unlock();
		} else if (!_splitScreen) {
			displayGlobalScene();
		} else {
			displaySplitScreenScene();
		}
		displayFPS();
		std::this_thread::yield();
	}
	std::this_thread::sleep_for(std::chrono::seconds(3));
	endPause();
	std::this_thread::sleep_for(std::chrono::seconds(3));
}

void irrl::ManageIrrlicht::manageEvent()
{
	if (_eventReceiver.IsKeyDown(irr::KEY_ESCAPE) && getCurrentScene() != "base" && !_displayGUI) {
		lock();

		if (_sound)
			_sound->stop();
		_sound = getSoundDevice()->play2D("media/sound/opening.ogg", false, false, true, irrklang::ESM_AUTO_DETECT, true);
		if (_currentScene == "winner") {
			_endGame = false;
			_draw = false;
			_winPLayer = true;
			endPause();
			unlock();
			initBaseScene();
			lock();
		} else if (_currentScene != "pause") {
			setPause();
			initPause();
		}
		_displayGUI = true;

		unlock();
	} else if (_displayGUI) {
		manageEventGui();
	} else if (!_endGame)
		manageEventPlayers();
}

void irrl::ManageIrrlicht::lock()
{
	_mutex.lock();
}

void irrl::ManageIrrlicht::unlock()
{
	_mutex.unlock();
}

void irrl::ManageIrrlicht::displayGui(bool display)
{
	_displayGUI = display;
}