/*
** EPITECH PROJECT, 2018
** cpp_indie_studio
** File description:
** Created by martin.januario@epitech.eu,
*/

#include "AManageIrrlicht.hpp"

nts::AManageIrrlicht::AManageIrrlicht(
	my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> &entities,
	my::ItemLocker<my::ThreadPool> &eventManager, nts::ManageIrrlicht &irrlicht
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
}

void nts::AManageIrrlicht::resetListObj()
{
}

irr::scene::ISceneManager *nts::AManageIrrlicht::getSceneManager() const
{
	return _sceneManager;
}

irr::IrrlichtDevice *nts::AManageIrrlicht::getDevice() const
{
	return _device;
}

irr::video::IVideoDriver *nts::AManageIrrlicht::getDriver() const
{
	return _driver;
}

irrklang::ISoundEngine *nts::AManageIrrlicht::getSoundDevice() const
{
	return _engine;
}

bool nts::AManageIrrlicht::addEntity(std::shared_ptr<is::IEntity> &, irr::scene::ISceneNode *, float)
{
	return true;
}

bool nts::AManageIrrlicht::deleteEntity(std::shared_ptr<is::IEntity> &)
{
	return true;
}

irr::scene::ISceneNode *nts::AManageIrrlicht::getNode(is::IEntity *)
{
	return nullptr;
}

void nts::AManageIrrlicht::updateView()
{
}

void nts::AManageIrrlicht::loopDisplay()
{
}

float &nts::AManageIrrlicht::getNodeSize(const std::shared_ptr<is::IEntity> &)
{
	return _tmp;
}

irr::core::vector2di nts::AManageIrrlicht::getMapSize() const
{
	return irr::core::vector2di(0, 0);
}

void nts::AManageIrrlicht::setMapSize(const irr::core::vector2di &)
{
}

void nts::AManageIrrlicht::setCameraPos()
{
}

void nts::AManageIrrlicht::manageEvent()
{
}