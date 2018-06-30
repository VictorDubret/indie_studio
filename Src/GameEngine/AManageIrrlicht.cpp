/*
** EPITECH PROJECT, 2018
** cpp_indie_studio
** File description:
** Created by martin.januario@epitech.eu,
*/

#include "AManageIrrlicht.hpp"

irrl::AManageIrrlicht::AManageIrrlicht(
	my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> &entities,
	my::ItemLocker<my::ThreadPool> &eventManager, irrl::ManageIrrlicht &irrlicht
) : _base(irrlicht), _eventManager(eventManager), _entities(entities), _eventReceiver()
{
	_device = irr::createDevice(irr::video::EDT_OPENGL,
		irr::core::dimension2d<irr::u32>(1600, 900), 32, false, false,
		false, &_eventReceiver);
	_engine = irrklang::createIrrKlangDevice();
	if (!_device || !_engine)
		throw std::exception();
	_driver = _device->getVideoDriver();
	_sceneManager = _device->getSceneManager();
	if (!_driver || !_sceneManager)
		throw std::exception();

	_driver->getTexture(irr::io::path("media/WallPass.png"));
	_driver->getTexture(irr::io::path("media/Bomb2.png"));
	_driver->getTexture(irr::io::path("media/Speed.png"));
	_driver->getTexture(irr::io::path("media/Flame.png"));

	_driver->getTexture(irr::io::path("media/fire.jpg"));
	_driver->getTexture(irr::io::path("media/winner.png"));
	_driver->getTexture(irr::io::path("media/1_character.png"));
	_driver->getTexture(irr::io::path("media/2_character.png"));
	_driver->getTexture(irr::io::path("media/3_character.png"));
	_driver->getTexture(irr::io::path("media/4_character.png"));
	_driver->getTexture(irr::io::path("media/bomb.png"));
	_driver->getTexture(irr::io::path("media/map_ob000_item_AL.png"));
	_driver->getTexture(irr::io::path("media/caisse.png"));
	_driver->getTexture(irr::io::path("media/wall.png"));
	_driver->getTexture(irr::io::path("media/floor.png"));
	_driver->getTexture(irr::io::path("media/bg2.png"));
}

void irrl::AManageIrrlicht::resetListObj()
{
}

irr::scene::ISceneManager *irrl::AManageIrrlicht::getSceneManager() const
{
	return _sceneManager;
}

irr::IrrlichtDevice *irrl::AManageIrrlicht::getDevice() const
{
	return _device;
}

irr::video::IVideoDriver *irrl::AManageIrrlicht::getDriver() const
{
	return _driver;
}

irrklang::ISoundEngine *irrl::AManageIrrlicht::getSoundDevice() const
{
	return _engine;
}

bool irrl::AManageIrrlicht::addEntity(std::shared_ptr<is::IEntity> &, irr::scene::ISceneNode *, float)
{
	return true;
}

bool irrl::AManageIrrlicht::deleteEntity(std::shared_ptr<is::IEntity> &)
{
	return true;
}

irr::scene::ISceneNode *irrl::AManageIrrlicht::getNode(is::IEntity *)
{
	return nullptr;
}

void irrl::AManageIrrlicht::updateView()
{
}

void irrl::AManageIrrlicht::loopDisplay()
{
}

float &irrl::AManageIrrlicht::getNodeSize(const std::shared_ptr<is::IEntity> &)
{
	return _tmp;
}

irr::core::vector2di irrl::AManageIrrlicht::getMapSize() const
{
	return irr::core::vector2di(0, 0);
}

void irrl::AManageIrrlicht::setMapSize(const irr::core::vector2di &)
{
}

void irrl::AManageIrrlicht::setCameraPos()
{
}

void irrl::AManageIrrlicht::manageEvent()
{
}

bool irrl::AManageIrrlicht::getDisplayGUI() const
{
	return _displayGUI;
}