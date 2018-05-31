/*
** EPITECH PROJECT, 2018
** test
** File description:
** Created by martin.januario@epitech.eu,
*/

#include "ManageIrrlicht.hpp"

nts::ManageIrrlicht::ManageIrrlicht(my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> &entities, my::ItemLocker<my::ThreadPool> &eventManager, irr::core::vector2di mapSize) : _eventManager(eventManager) ,_entities(entities), _eventReceiver(), _mapSize(mapSize)
{
	_device = irr::createDevice(irr::video::EDT_OPENGL, irr::core::dimension2d<irr::u32>(1200, 800), 32, false, false, false, &_eventReceiver);
	if (!_device)
		throw std::exception();

	_driver = _device->getVideoDriver();
	_sceneManager = _device->getSceneManager();
	if (!_driver || !_sceneManager)
		throw std::exception();

	_sceneManager->addCameraSceneNode(0, irr::core::vector3df(0, 15 , -10), irr::core::vector3df(0, 10, 0));
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
		bool doSomething = false;
		for (int i = 0; it.key[i].f != nullptr ; ++i) {
			if (_eventReceiver.IsKeyDown(it.key[i].key)) {
				_eventManager->enqueue(it.key[i].f);
				doSomething = true;
				break;
			}
		}
		if (!doSomething)
			_eventManager->enqueue(it.nothing.f);
	}

}

irr::scene::ISceneNode *nts::ManageIrrlicht::getNode(const std::shared_ptr<is::IEntity> &entity)
{
	return _listObj[entity].obj;
}

float &nts::ManageIrrlicht::getNodeSize(const std::shared_ptr<is::IEntity> &entity)
{
	return _listObj[entity].size;
}

bool nts::ManageIrrlicht::addEntity(std::shared_ptr<is::IEntity> &entity, irr::scene::ISceneNode *obj, float size)
{
	auto tmp = dynamic_cast<is::ACharacter *>(entity.get());

	if (tmp != nullptr && tmp->getType() == "Character") {
		player_t player;
		if (_listPlayer.empty())
			player = {std::shared_ptr<is::ACharacter>(tmp), {irr::KEY_ESCAPE, [tmp](){tmp->doNothing();}},
			        {{irr::KEY_LEFT, [tmp](){tmp->moveLeft();}},
				{irr::KEY_RIGHT, [tmp](){tmp->moveRight();}},
				{irr::KEY_UP, [tmp](){tmp->moveUp();}},
				{irr::KEY_DOWN, [tmp](){tmp->moveDown();}},
				{irr::KEY_RETURN, [tmp](){tmp->dropBomb();}},
				{irr::KEY_ESCAPE, nullptr}}};
		else
			player = {std::shared_ptr<is::ACharacter>(tmp), {irr::KEY_ESCAPE, [tmp](){tmp->doNothing();}},
			        {{irr::KEY_KEY_Q, [tmp](){tmp->moveLeft();}},
				{irr::KEY_KEY_D, [tmp](){tmp->moveRight();}},
				{irr::KEY_KEY_Z, [tmp](){tmp->moveUp();}},
				{irr::KEY_KEY_S, [tmp](){tmp->moveDown();}},
				{irr::KEY_SPACE, [tmp](){tmp->dropBomb();}},
				{irr::KEY_ESCAPE, nullptr}}};
		_listPlayer.push_back(player);
	}
	_listObj[entity] = {obj, size};
	return false;
}

bool nts::ManageIrrlicht::deleteEntity(std::shared_ptr<is::IEntity> &entity)
{
	auto tmp = dynamic_cast<is::ACharacter *>(entity.get());

	if (tmp && tmp->getType() == "Character") {
		int idx = 0;
		for (auto &it : _listPlayer) {
			if (it.entity == entity) {
				_listPlayer.erase(_listPlayer.begin() + idx);
				break;
			}
			idx++;
		}
	}
	_listObj[entity].obj->setVisible(false);
	_listObj.erase(_listObj.find(entity));
	return false;
}

irr::scene::ISceneManager *nts::ManageIrrlicht::getSceneManager() const
{
	return _sceneManager;
}

irr::IrrlichtDevice *nts::ManageIrrlicht::getDevice() const
{
	return _device;
}

irr::video::IVideoDriver *nts::ManageIrrlicht::getDriver() const
{
	return _driver;
}

void nts::ManageIrrlicht::setMapSize(const irr::core::vector2di &mapSize)
{
	_mapSize = mapSize;
}

irr::core::vector2di nts::ManageIrrlicht::getMapSize() const
{
	return _mapSize;
}