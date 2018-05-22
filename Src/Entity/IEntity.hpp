/*
** EPITECH PROJECT, 2018
** cpp_indie_studio
** File description:
** Created by sahel.lucas-saoudi@epitech.eu,
*/

#ifndef CPP_INDIE_STUDIO_IENTITY_HPP
#define CPP_INDIE_STUDIO_IENTITY_HPP

#include <string>

namespace is {
	class IEntity {
	public:
		IEntity() = default;
		virtual ~IEntity() = 0;

		struct Position {
			float x;
			float y;
			float z;
		};

		virtual is::IEntity::Position &getPosition() = 0;
		virtual float &getX() = 0;
		virtual float &getY() = 0;
		virtual float &getZ() = 0;
		virtual std::string const &getType() const = 0;
		virtual bool isCollidable() const = 0;

		virtual void setX(float &) = 0;
		virtual void setY(float &) = 0;
		virtual void setZ(float &) = 0;

		template<class Callable, class... Arg>
		virtual void addEvent(std::string const &key, Callable func, Arg &...args) {

		};

		virtual void event(std::string const &key) = 0;
		virtual void event(std::string const &key, is::IEntity &caller) = 0;

		virtual void animate() = 0; // TODO when Irlicht is set
	};
}

#endif //CPP_INDIE_STUDIO_IENTITY_HPP
