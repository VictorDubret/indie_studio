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
#include "IEntity.hpp"

namespace is {
	class AEntity : public IEntity {
	public:
		// Constructor
		explicit AEntity(std::vector<IEntity *> &);

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

		// Other
		template<class Callable, class... Arg>
		void addEvent(std::string const &key, Callable &&func, Arg &&...args) {

			_events[key] = [this, &func, args...](is::IEntity *caller) {
				auto f = std::bind(std::forward(func), std::forward(args)..., caller);

				f();
			};
		};

		void event(std::string const &key, is::IEntity *caller = nullptr) override;

		bool ended = false;
		void ia() override;

	protected:
		std::vector<IEntity *> &_entities;
		std::string _type = "AEntity";
		is::IEntity::Position _position;

		std::map<std::string, std::function<void (is::IEntity *)>> _events;
		bool _collidable = false;
		bool _pickable = false;

		bool _wallPassable = false;

		std::mutex _mutex;
		std::thread _thread;
		std::queue<std::pair<std::string, IEntity *>>_poll;

		void execEvent();
	private:
	};
}

#endif //CPP_INDIE_STUDIO_AENTITY_HPP
