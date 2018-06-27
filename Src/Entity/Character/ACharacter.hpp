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
			my::ItemLocker<my::ThreadPool> &eventManager, irrl::ManageIrrlicht &irrlicht, std::size_t id = 1);

		// Destructor
		~ACharacter() override;

		// Getter
		bool const &getWallPass() const;
		size_t const &getBomb() const;
		size_t const &getBombMax() const;
		size_t const &getBombLength() const;
		float const &getSpeed() const;


		// Setter
		void setWallPass(bool wallPass);
		void setBomb(size_t bomb);
		void setBombMax(size_t bombMax);
		void setBombLength(size_t length);
		void setSpeed(float speed);
		void setHP(int life);

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
		void save();

		void explode() override;

		bool checkCollision(std::vector<std::shared_ptr<is::IEntity>> &);

		void texture() override;
	protected:
		/*
		 * Internal Config
		 */
		float _speedCoef = 0.15;
		std::size_t _id;

		size_t _pv = 1;

		bool _wallPass = false;
		MoveCharacter _lastMove = MoveCharacter::UNKNOWN;

		size_t _bomb = 1;

		size_t _bombMax = 1;

		size_t _bombLength = 1;
		float _speed = 1;

		bool move(float, float, float);
		int _xmax = -1;
		int _zmax = -1;
	private:
	};
}

#endif //CPP_INDIE_STUDIO_ACHARACTER_HPP
