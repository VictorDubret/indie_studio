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
#include "IndieStudio.hpp"
#include "ThreadPool.hpp"
#include "ItemLocker.hpp"
#include "IEntity.hpp"


namespace is {
	class AEntity : public IEntity {
	public:
		// Constructor
		AEntity(Entity_t &entities,
			ThreadPool_t &eventManager);
		// Destructor
		~AEntity() override;

		// Getter
		is::IEntity::Position &getPosition() override;
		double &getX() override;
		double &getY() override;
		double &getZ() override;
		std::string const &getType() const override;
		bool isCollidable() const override;
		bool isPickable() const override;
		bool isWallPassable() const override;
		bool isWalkable() const override;

		// Setter
		void setX(double &) override;
		void setY(double &) override;
		void setZ(double &) override;
		void setPosition(Position &) override;

		void collide(is::IEntity *collider) override;
		std::vector<std::shared_ptr<IEntity>> getEntitiesAt(int x, int y, int z);
		void explode() override;

	protected:
		my::ItemLocker<std::vector<std::shared_ptr<IEntity>>> &_entities;
		my::ItemLocker<my::ThreadPool> &_eventManager;
		std::string _type = "Entity";
		is::IEntity::Position _position;

		bool _collidable = true;
		bool _pickable = false;
		bool _walkable = true;
		bool _wallPassable = true;

		bool isInCollisionWith(std::shared_ptr<IEntity> &entity);
	private:
	};
}

#endif //CPP_INDIE_STUDIO_AENTITY_HPP
