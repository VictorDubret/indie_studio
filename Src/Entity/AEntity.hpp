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
#include "IEntity.hpp"

namespace is {
	class AEntity : public IEntity {
	public:
		// Constructor
		AEntity(std::vector<IEntity *> &);

		// Destructor
		~AEntity() override = default;

		// Getter
		is::IEntity::Position &getPosition() override;
		float &getX() override;
		float &getY() override;
		float &getZ() override;
		std::string const &getType() const override;

		// Setter
		void setX(float &) override;
		void setY(float &) override;
		void setZ(float &) override;

		// Other
		template<class Callable, class... Arg>
		void addEvent(std::string const &key, Callable &&func, Arg &&...args) {
			auto f = std::bind(std::forward(func), std::forward(args)...);

			_events[key] = [this]() {
				f();
			};
		};

		void event(std::string const &key) override;


	protected:

	private:
		std::vector<IEntity *> &_entities;
		std::string _type = "AEntity";
		is::IEntity::Position _position;
		std::map<std::string, std::function<void (*)>> _events;
	};
}

#endif //CPP_INDIE_STUDIO_AENTITY_HPP
