/*
** EPITECH PROJECT, 2018
** cpp_indie_studio
** File description:
** Created by martin.januario@epitech.eu,
*/

#include "ManageObject.hpp"

bool irrl::ManageObject::createCube(irrl::ManageIrrlicht &manage,
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

bool irrl::ManageObject::createAnim(irrl::ManageIrrlicht &manage,
	std::shared_ptr<is::IEntity> &obj, const irr::io::path &path,
	irr::f32 size, irr::s32 id, const irr::core::vector3df &position,
	const irr::core::vector3df &rotation, const irr::core::vector3df &scale)
{
	irr::scene::IAnimatedMesh* mesh = manage.getSceneManager()->getMesh(path);
	if (!mesh)
		throw std::exception();
	irr::scene::IAnimatedMeshSceneNode* node = manage.getSceneManager()->addAnimatedMeshSceneNode(mesh, 0, id, position, rotation, scale);
	if (!node)
		throw std::exception();
	manage.addEntity(obj, node, size);
	return false;
}

void irrl::ManageObject::setPosition(irrl::ManageIrrlicht &manage, std::shared_ptr<is::IEntity> &obj, irr::core::vector3df &position)
{
	manage.getNode(obj.get())->setPosition(position);
}

irr::core::vector3df irrl::ManageObject::getPosition(irrl::ManageIrrlicht &manage, std::shared_ptr<is::IEntity> &obj)
{
	return manage.getNode(obj.get())->getPosition();
}

void irrl::ManageObject::setMaterialLight(irrl::ManageIrrlicht &manage, std::shared_ptr<is::IEntity> &obj, bool opt)
{
	auto tmp = manage.getNode(obj.get());
	if (!tmp)
		return;
	tmp->setMaterialFlag(irr::video::EMF_LIGHTING, opt);
}

void irrl::ManageObject::setTexture(irrl::ManageIrrlicht &manage, std::shared_ptr<is::IEntity> &obj, const irr::io::path &path)
{
	irr::video::ITexture *texture = manage.getDriver()->getTexture(path);
	auto tmp = manage.getNode(obj.get());
	if (texture == nullptr || !tmp)
		return;
	tmp->setMaterialTexture(0, texture);
}

void irrl::ManageObject::setAnimation(irrl::ManageIrrlicht &manage, std::shared_ptr<is::IEntity> &obj, irr::scene::EMD2_ANIMATION_TYPE type)
{
	auto tmp = manage.getNode(obj.get());
	if (!tmp)
		return;
	static_cast<irr::scene::IAnimatedMeshSceneNode *>(tmp)->setMD2Animation(type);
}

void irrl::ManageObject::setScale(irrl::ManageIrrlicht &manage, std::shared_ptr<is::IEntity> &obj, const irr::core::vector3df &vect)
{
	auto tmp = manage.getNode(obj.get());
	if (!tmp)
		return;
	tmp->setScale(vect);
}

void irrl::ManageObject::setRotation(irrl::ManageIrrlicht &manage, std::shared_ptr<is::IEntity> &obj, const irr::core::vector3df &rotation)
{
	auto tmp = manage.getNode(obj.get());
	if (!tmp)
		return;
	tmp->setRotation(rotation);
}