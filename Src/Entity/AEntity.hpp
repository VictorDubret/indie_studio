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
			ThreadPool_t &eventManager,
			irrl::ManageIrrlicht &irrlicht);
		// Destructor
		~AEntity() override;

		// Getter
		irr::core::vector3df const getPosition() const override;
		float getX() const override;
		float getY() const override;
		float getZ() const override;
		std::string const &getType() const override;
		bool isCollidable() const override;
		bool isPickable() const override;
		bool isWallPassable() const override;
		bool isWalkable(std::shared_ptr<is::IEntity> &) override;
		bool isWalkable() const override;

		// Setter
		void setX(float) override;
		void setY(float) override;
		void setZ(float) override;
		void setPosition(irr::core::vector3df) override;

		void setCollidable(bool state) override;
		void setPickable(bool state) override;
		void setWallPassable(bool state) override;
		void setWalkable(bool state) override;

		void collide(is::IEntity *collider) override;
		std::vector<std::shared_ptr<IEntity>> getEntitiesAt(float x, float z) const;
		void explode() override;

		virtual void texture() = 0;

		std::recursive_mutex &getMutex() override;
	protected:
		my::ItemLocker<std::vector<std::shared_ptr<IEntity>>> &_entities;
		my::ItemLocker<my::ThreadPool> &_eventManager;
		std::string _type = "Entity";

		bool _collidable = true;
		bool _pickable = false;
		bool _walkable = true;
		bool _wallPassable = true;

		std::recursive_mutex _mutex;
		bool _locked = true;
		bool isInCollisionWith(std::shared_ptr<IEntity> &entity);

		std::shared_ptr<IEntity> _spointer;
		irrl::ManageIrrlicht &_irrlicht;
	private:
	};
}

#endif //CPP_INDIE_STUDIO_AENTITY_HPP
