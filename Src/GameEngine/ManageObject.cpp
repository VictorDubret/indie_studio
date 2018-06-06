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

bool nts::ManageObject::createAnim(nts::ManageIrrlicht &manage,
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
	auto tmp = manage.getNode(obj);
	if (!tmp)
		return;
	tmp->setMaterialFlag(irr::video::EMF_LIGHTING, opt);
}

void nts::ManageObject::setTexture(nts::ManageIrrlicht &manage, std::shared_ptr<is::IEntity> &obj, const irr::io::path &path)
{
	irr::video::ITexture *texture = manage.getDriver()->getTexture(path);
	auto tmp = manage.getNode(obj);
	if (texture == nullptr || !tmp)
		return;
	tmp->setMaterialTexture(0, texture);
}

void nts::ManageObject::setAnimation(nts::ManageIrrlicht &manage, std::shared_ptr<is::IEntity> &obj, irr::scene::EMD2_ANIMATION_TYPE type)
{
	static_cast<irr::scene::IAnimatedMeshSceneNode *>(manage.getNode(obj))->setMD2Animation(type);
}

void nts::ManageObject::setScale(nts::ManageIrrlicht &manage, std::shared_ptr<is::IEntity> &obj, const irr::core::vector3df &vect)
{
	manage.getNode(obj)->setScale(vect);
}

void nts::ManageObject::setRotation(nts::ManageIrrlicht &manage, std::shared_ptr<is::IEntity> &obj, const irr::core::vector3df &rotation)
{
	manage.getNode(obj)->setRotation(rotation);
}