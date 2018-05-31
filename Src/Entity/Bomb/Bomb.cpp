//
// Created by victor on 27/05/18.
// File description:
// Bomb.cpp
//

#include <exception>
#include <algorithm>
#include "ItemLocker.hpp"
#include "Bomb.hpp"
#include "Timer.hpp"

is::Bomb::Bomb(my::ItemLocker<std::vector<std::shared_ptr<IEntity>>> &entities,
	my::ItemLocker<my::ThreadPool> &eventManager,
	std::shared_ptr<IEntity> &Player, nts::ManageIrrlicht &irrlicht,
	size_t time
) : AEntity(entities, eventManager, irrlicht), _player(Player)
{
	std::cout << "Bomb constructor" << std::endl;
	_type = "Bomb";
	if (dynamic_cast<is::ACharacter *>(_player.get()) == nullptr) {
		throw std::exception();
	}
	timer(time);
}

void is::Bomb::explode()
{
	std::cout << "Adding explode event !" << std::endl;
	_entities.lock();
	_eventManager.lock();

	_eventManager->enqueue([this]() {
		std::vector<std::shared_ptr<IEntity>> xAxes;
		for (int i = (_lenExplosion * -1); i <= _lenExplosion; ++i) {
			std::vector<std::shared_ptr<IEntity>> tmp = this->getEntitiesAt(
				this->getX() - i, this->getY(), this->getZ());
			xAxes.insert(xAxes.end(), tmp.begin(), tmp.end());
		}
		std::vector<std::shared_ptr<IEntity>> zAxes;
		for (int i = (_lenExplosion * -1); i <= _lenExplosion; ++i) {
			std::vector<std::shared_ptr<IEntity>> tmp = this->getEntitiesAt(
				this->getX(), this->getY(), this->getZ() - i);
			zAxes.insert(zAxes.end(), tmp.begin(), tmp.end());
		}
		for (const auto &it : xAxes) {
			if (it.get() != this) {
				std::cout << "J'ai recuperer sur X : "
					<< it.get()->getType() << std::endl;
				it.get()->explode();
			}
		}
		std::cout << "I'll explode all blocks around me ! MOUAHAHAH"
			<< std::endl;
		dynamic_cast<is::ACharacter *>(_player.get())->setBomb(1);
		for (const auto &it : zAxes) {
			if (it.get() != this) {
				std::cout << "J'ai recuperer sur Z : "
					<< it.get()->getType() << std::endl;
				it.get()->explode();
			}
		}
	});
	_eventManager.unlock();
	_entities.unlock();
}

void is::Bomb::timer(size_t time)
{
	_entities.lock();
	_eventManager.lock();
	_eventManager->enqueue([this, time]() {
		Timer timer;
		std::cout << "I'll explode in " << time << " seconds"
			<< std::endl;
		timer.startTimer(time);
		//Change animation flamish bomb
		while (!timer.isOver()) {
		}
		std::cout << "Bouuum" << std::endl;
		explode();
	});
	_eventManager.unlock();
	_entities.unlock();
}
