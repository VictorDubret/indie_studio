/*
** EPITECH PROJECT, 2018
** test
** File description:
** Created by martin.januario@epitech.eu,
*/

#include "ManageIrrlicht.hpp"

nts::ManageIrrlicht::ManageIrrlicht(my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> &entities, my::ItemLocker<my::ThreadPool> &eventManager) : _eventManager(eventManager) ,_entities(entities), _eventReceiver()
{
	_device = irr::createDevice(irr::video::EDT_OPENGL, irr::core::dimension2d<irr::u32>(1200, 800), 32, false, false, false, &_eventReceiver);
	_driver = _device->getVideoDriver();
	_sceneManager = _device->getSceneManager();

	_sceneManager->addCameraSceneNode(0, irr::core::vector3df(0,30,-40), irr::core::vector3df(0,5,0));
}

void nts::ManageIrrlicht::updateView()
{
}

void nts::ManageIrrlicht::loopDisplay()
{
	while (_device->run()) {
		_driver->beginScene(true, true, irr::video::SColor(0, 220, 220, 220));
		_sceneManager->drawAll();
		_driver->endScene();
		manageEvent();
	}
}

void nts::ManageIrrlicht::manageEvent()
{
	if (_eventReceiver.IsKeyDown(irr::KEY_ESCAPE))
		_device->closeDevice();
	else
		manageEventPlayers();
}

void nts::ManageIrrlicht::manageEventPlayers()
{
	for (auto &it : _listPlayer) {
		for (int i = 0; it.key[i].f != nullptr ; ++i) {
			if (_eventReceiver.IsKeyDown(it.key[i].key)) {
				_eventManager->enqueue(it.key[i].f);
			}
		}
	}

}

irr::IReferenceCounted *nts::ManageIrrlicht::getNode(std::shared_ptr<is::IEntity> &entity)
{
	return _listObj[entity];
}

bool nts::ManageIrrlicht::addEntity(std::shared_ptr<is::IEntity> &entity, irr::IReferenceCounted *obj)
{
	auto tmp = dynamic_cast<is::ACharacter *>(entity.get());

	if (tmp != nullptr && tmp->getType() == "Character") {
		player_t player;
		if (_listPlayer.empty())
			player = {std::shared_ptr<is::ACharacter>(tmp),
			        {{irr::KEY_LEFT, [tmp](){tmp->moveLeft();}},
				{irr::KEY_RIGHT, [tmp](){tmp->moveRight();}},
				{irr::KEY_UP, [tmp](){tmp->moveUp();}},
				{irr::KEY_DOWN, [tmp](){tmp->moveDown();}},
				{irr::KEY_RETURN, [tmp](){tmp->dropBomb();}},
				{irr::KEY_ESCAPE, nullptr}}};
		else
			player = {std::shared_ptr<is::ACharacter>(tmp),
			        {{irr::KEY_KEY_Q, [tmp](){tmp->moveLeft();}},
				{irr::KEY_KEY_D, [tmp](){tmp->moveRight();}},
				{irr::KEY_KEY_Z, [tmp](){tmp->moveUp();}},
				{irr::KEY_KEY_S, [tmp](){tmp->moveDown();}},
				{irr::KEY_SPACE, [tmp](){tmp->dropBomb();}},
				{irr::KEY_ESCAPE, nullptr}}};
		_listPlayer.push_back(player);
	}
	_listObj[entity] = obj;
	return false;
}

irr::scene::ISceneManager* nts::ManageIrrlicht::getSceneManager() const
{
	return _sceneManager;
}