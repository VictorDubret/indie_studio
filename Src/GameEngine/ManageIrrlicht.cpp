/*
** EPITECH PROJECT, 2018
** test
** File description:
** Created by martin.januario@epitech.eu,
*/

#include "ManageIrrlicht.hpp"

nts::ManageIrrlicht::ManageIrrlicht(my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> &entities, my::ItemLocker<my::ThreadPool> &eventManager) : _eventManager(eventManager) ,_entities(entities)
{
	_device = irr::createDevice(irr::video::EDT_OPENGL, irr::core::dimension2d<irr::u32>(1200,800), 32, false, false, false/*, &receiver*/);
	_driver = _device->getVideoDriver();
	_sceneManager = _device->getSceneManager();
}

void nts::ManageIrrlicht::updateView()
{
}

void nts::ManageIrrlicht::loopDisplay()
{
}

void nts::ManageIrrlicht::manageEvent()
{
}

irr::IReferenceCounted &nts::ManageIrrlicht::getNode(std::shared_ptr<is::IEntity> &entity)
{
	return _listObj[entity];
}

bool nts::ManageIrrlicht::addEntity(std::shared_ptr<is::IEntity> &entity, irr::IReferenceCounted &obj)
{
	if (dynamic_cast<is::ACharacter *>(entity.get()))
		_listPlayer.push_back(entity);
	_listObj[entity] = obj;
	return false;
}

irr::scene::ISceneManager* nts::ManageIrrlicht::getSceneManager() const
{
	return _sceneManager;
}