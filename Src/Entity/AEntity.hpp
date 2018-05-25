/*
** EPITECH PROJECT, 2018
** cpp_indie_studio
** File description:
** Created by sahel.lucas-saoudi@epitech.eu,
*/

#ifndef CPP_INDIE_STUDIO_AENTITY_HPP
#define CPP_INDIE_STUDIO_AENTITY_HPP

#include <memory>
#include <map>
#include <vector>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include "ThreadPool.hpp"
#include "ItemLocker.hpp"
#include "IEntity.hpp"

namespace is {
	class AEntity : public IEntity {
	public:
		// Constructor
		AEntity(my::ItemLocker<std::vector<std::shared_ptr<IEntity>>> &entities,
			my::ItemLocker<my::ThreadPool> &eventManager);
		// Destructor
		~AEntity() override;

		// Getter
		is::IEntity::Position &getPosition() override;
		float &getX() override;
		float &getY() override;
		float &getZ() override;
		std::string const &getType() const override;
		bool isCollidable() const override;
		bool isPickable() const override;
		bool isWallPassable() const override;

		// Setter
		void setX(float &) override;
		void setY(float &) override;
		void setZ(float &) override;

		void collide(IEntity *collider) override;

	protected:
		my::ItemLocker<std::vector<std::shared_ptr<IEntity>>> &_entities;
		my::ItemLocker<my::ThreadPool> &_eventManager;
		std::string _type = "AEntity";
		is::IEntity::Position _position;

		bool _collidable = false;
		bool _pickable = false;

		bool _wallPassable = false;
	private:
	};
}

#endif //CPP_INDIE_STUDIO_AENTITY_HPP
