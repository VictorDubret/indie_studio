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
	class ACharacter: public AEntity {
	public:

		enum MoveCharacter {
			NOTHING,
			LEFT,
			RIGHT,
			UP,
			DOWN,
			UNKNOWN
		};

		// Constructor
		ACharacter(my::ItemLocker<std::vector<std::shared_ptr<IEntity>>> &entities,
			my::ItemLocker<my::ThreadPool> &eventManager, nts::ManageIrrlicht &irrlicht);

		// Destructor
		~ACharacter() override;

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

		/*
		 * Event
		 */
		void moveUp();
		void moveDown();
		void moveLeft();
		void moveRight();
		void dropBomb();
		void doNothing();

		void explode() override;

		bool checkCollision();
	protected:
		/*
		 * Internal Config
		 */
		float _speedCoef = 0.1;

		uint _pv = 1;

		bool _wallPass = false;
		MoveCharacter _lastMove = MoveCharacter::UNKNOWN;

		size_t _bomb = 1;
		size_t _bombMax = 1;

		size_t _bombLength = 2;
		size_t _speed = 1;

		void move(float, float, float);
	private:
	};
}

#endif //CPP_INDIE_STUDIO_ACHARACTER_HPP
