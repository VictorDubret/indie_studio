/*
** EPITECH PROJECT, 2018
** cpp_indie_studio
** File description:
** Created by sahel.lucas-saoudi@epitech.eu,
*/

#ifndef CPP_INDIE_STUDIO_IENTITY_HPP
#define CPP_INDIE_STUDIO_IENTITY_HPP

#include <memory>
#include <string>
#include <irrlicht.h>

namespace irrl {
	class ManageIrrlicht;
};

namespace is {
	class IEntity {
	public:
		IEntity() = default;
		virtual ~IEntity() = default;

		virtual irr::core::vector3df const getPosition() const = 0;
		virtual float getX() const = 0;
		virtual float getY() const = 0;
		virtual float getZ() const = 0;
		virtual std::string const &getType() const = 0;
		virtual bool isCollidable() const = 0;
		virtual bool isPickable() const = 0;
		virtual bool isWallPassable() const = 0;
		virtual bool isWalkable(std::shared_ptr<is::IEntity> &) = 0;
		virtual bool isWalkable() const = 0;

		virtual void setX(float) = 0;
		virtual void setY(float) = 0;
		virtual void setZ(float) = 0;
		virtual void setPosition(irr::core::vector3df) = 0;
		virtual void setCollidable(bool state) = 0;
		virtual void setPickable(bool state) = 0;
		virtual void setWallPassable(bool state) = 0;
		virtual void setWalkable(bool state) = 0;

		virtual void collide(IEntity * collider) = 0;
		virtual void explode() = 0;

		virtual void lock() = 0;
		virtual void unlock() = 0;

		//virtual void animate() = 0; // TODO when Irlicht is set
	};
}

#endif //CPP_INDIE_STUDIO_IENTITY_HPP
