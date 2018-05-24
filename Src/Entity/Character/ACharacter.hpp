/*
** EPITECH PROJECT, 2018
** cpp_indie_studio
** File description:
** Created by sahel.lucas-saoudi@epitech.eu,
*/

#ifndef CPP_INDIE_STUDIO_ACHARACTER_HPP
#define CPP_INDIE_STUDIO_ACHARACTER_HPP

#include "AEntity.hpp"

namespace is {
	class ACharacter: AEntity {
	public:
		// Constructor
		explicit ACharacter(std::vector<is::IEntity *> &entities);

		// Destructor
		~ACharacter() override = default;

		// Getter
		bool const &getWallPass() const;
		size_t const &getBomb() const;
		size_t const &getBombMax() const;
		size_t const &getBombLength() const;
		size_t const &getSpeed() const;


		// Setter
		void setWallPass(bool wallPass);
		void setBomb(size_t bomb);
		void setBombMax(size_t bombMax);
		void setBombLength(size_t length);
		void setSpeed(size_t speed);

		// Other
		is::ACharacter &operator++();

		void moveUp();
		void moveDown();
		void moveLeft();
		void moveRight();

		bool checkCollision();
	protected:
		bool _wallPass = false;

		size_t _bomb = 1;
		size_t _bombMax = 1;

		size_t _bombLength = 1;
		size_t _speed = 1;

		std::string _type = "ACharacter";
	private:
	};
}

#endif //CPP_INDIE_STUDIO_ACHARACTER_HPP