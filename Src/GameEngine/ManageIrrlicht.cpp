/*
** EPITECH PROJECT, 2018
** test
** File description:
** Created by martin.januario@epitech.eu,
*/

#include <irrTypes.h>
#include <zconf.h>
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
}

void irrl::ManageIrrlicht::endScene()
{
	for (const auto &it : _listPlayer) {
		if (it.alive) {
			if (_camera[GLOBAL]->getPosition().Y < _camera[GLOBAL]->getTarget().Y + 2) {
				_driver->beginScene(true, true, irr::video::SColor(255, 100, 100, 100));
				lock();

				//initWinner();
				_sceneManager->drawAll();
				//drawGUI();
				unlock();
				_driver->endScene();
			} else {
				if (!getNode(it.entity))
					continue;
				const irr::core::vector3df winnerPos = getNode(it.entity)->getPosition();
				_camera[GLOBAL]->setTarget(winnerPos);
				_driver->beginScene(true, true, irr::video::SColor(255, 100, 100, 100));
				lock();
				_sceneManager->drawAll();
				unlock();
				const irr::core::vector3df tmp(_camera[GLOBAL]->getPosition().X, static_cast<irr::f32>(_camera[GLOBAL]->getPosition().Y - 0.1), _camera[GLOBAL]->getPosition().Z);
				_camera[GLOBAL]->setPosition(tmp);
				_driver->endScene();
			}
		}
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
	sleep(3);
	endPause();
}

void irrl::ManageIrrlicht::manageEvent()
{
	if (_eventReceiver.IsKeyDown(irr::KEY_ESCAPE) && getCurrentScene() != "base" && !_displayGUI) {
		_base.lock();
		_engine->stopAllSounds();
		_sound = getSoundDevice()->play2D("media/sound/opening.ogg", false, false, true, irrklang::ESM_AUTO_DETECT, true);
		if (_currentScene == "winner") {
			_endGame = false;
			_draw = false;
			_alreadyEnd = false;
			endPause();
			initBaseScene();
		} else if (_currentScene != "pause") {
			setPause();
			initPause();
		}
		_displayGUI = true;

		_base.unlock();
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

void irrl::ManageIrrlicht::drawScene()
{
	return;

}
