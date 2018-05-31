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
#include "Explosion.hpp"

is::Bomb::Bomb(my::ItemLocker<std::vector<std::shared_ptr<IEntity>>> &entities,
	my::ItemLocker<my::ThreadPool> &eventManager,
	std::shared_ptr<IEntity> &Player, nts::ManageIrrlicht &irrlicht,
	size_t time
) : AEntity(entities, eventManager, irrlicht), _player(Player)
{
	std::cout << "Bomb constructor" << std::endl;
	_type = "Bomb";
	if (dynamic_cast<is::ACharacter *>(_player.get()) == nullptr) {
		delete this;
	}
	timer(time);
}

void is::Bomb::explode()
{
	std::cout << "Adding explode event !" << std::endl;
	_entities.lock();
	_eventManager.lock();

	_eventManager->enqueue([this]() {
		std::cout << "I'll explode all blocks around me ! MOUAHAHAH"
			<< std::endl;
		std::vector<std::shared_ptr<IEntity>> xAxes;
		for (int i = (_lenExplosion * -1); i <= _lenExplosion; ++i) {
			std::vector<std::shared_ptr<IEntity>> tmp = this->getEntitiesAt(
				(int) this->getX() - i, (int) this->getY(), (int) this->getZ());
			xAxes.insert(xAxes.end(), tmp.begin(), tmp.end());
			auto a = new is::Explosion(_entities, _eventManager, _irrlicht);

			a->setX((int) getX() - i);
			a->setY((int) getY());
			a->setZ((int) getZ());
		}
		auto it_tmp = std::unique (xAxes.begin(), xAxes.end());

		xAxes.resize(std::distance(xAxes.begin(), it_tmp));
		for (const auto &it : xAxes) {
			if (it.get() != this) {
				std::cout << "J'ai recuperer sur X : "
					<< it.get()->getType() << std::endl;
				it->explode();
			}
		}
		std::vector<std::shared_ptr<IEntity>> zAxes;
		for (int i = (_lenExplosion * -1); i <= _lenExplosion; ++i) {
			std::vector<std::shared_ptr<IEntity>> tmp = this->getEntitiesAt(
				(int) this->getX(), (int) this->getY(), (int) this->getZ() - i);
			zAxes.insert(zAxes.end(), tmp.begin(), tmp.end());

			auto a = new is::Explosion(_entities, _eventManager, _irrlicht);

			a->setX((int) getX());
			a->setY((int) getY());
			a->setZ((int) getZ() - i);
		}
		auto it_tmp2 = std::unique (zAxes.begin(), zAxes.end());

		zAxes.resize(std::distance(zAxes.begin(), it_tmp2));
		for (const auto &it : zAxes) {
			if (it.get() != this) {
				std::cout << "J'ai recuperer sur Z : "
					<< it.get()->getType() << std::endl;
				it.get()->explode();
			}
		}

		auto tmp = dynamic_cast<is::ACharacter *>(_player.get());
		if (tmp)
			tmp->operator++();
		this->~Bomb();
		_eventManager.unlock();
		_entities.unlock();
		return;
	});
	_eventManager.unlock();
	_entities.unlock();
}

void is::Bomb::timer(size_t time)
{
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
}
