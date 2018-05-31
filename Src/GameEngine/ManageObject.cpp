/*
** EPITECH PROJECT, 2018
** cpp_indie_studio
** File description:
** Created by martin.januario@epitech.eu,
*/

#include "ManageObject.hpp"

bool nts::ManageObject::createCube(nts::ManageIrrlicht &manage,
	std::shared_ptr<is::IEntity> &obj, irr::f32 size, irr::s32 id,
	const irr::core::vector3df &position,
	const irr::core::vector3df &rotation, const irr::core::vector3df &scale)
{
	irr::scene::ISceneNode *cube = manage.getSceneManager()->addCubeSceneNode(size, 0, id, position, rotation, scale);
	if (cube == nullptr)
		throw std::exception();
	manage.addEntity(obj, cube, size);
	return false;
}

void nts::ManageObject::setPosition(nts::ManageIrrlicht &manage, std::shared_ptr<is::IEntity> &obj, irr::core::vector3df &position)
{
	manage.getNode(obj)->setPosition(position);
}

irr::core::vector3df nts::ManageObject::getPosition(nts::ManageIrrlicht &manage, std::shared_ptr<is::IEntity> &obj)
{
	return manage.getNode(obj)->getPosition();
}

void nts::ManageObject::setMaterialLight(nts::ManageIrrlicht &manage, std::shared_ptr<is::IEntity> &obj, bool opt)
{
	manage.getNode(obj)->setMaterialFlag(irr::video::EMF_LIGHTING, opt);
}

void nts::ManageObject::setTexture(nts::ManageIrrlicht &manage, std::shared_ptr<is::IEntity> &obj, const irr::io::path &path)
{
	irr::video::ITexture *texture = manage.getDriver()->getTexture(path);
	if (texture == nullptr)
		throw std::exception();
	manage.getNode(obj)->setMaterialTexture(0, texture);
}