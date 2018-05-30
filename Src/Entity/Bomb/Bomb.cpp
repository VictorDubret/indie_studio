//
// Created by victor on 27/05/18.
// File description:
// Bomb.cpp
//

#include <algorithm>
#include "ItemLocker.hpp"
#include "Bomb.hpp"
#include "Timer.hpp"

is::Bomb::Bomb(my::ItemLocker<std::vector<std::shared_ptr<IEntity>>> &entities,
	my::ItemLocker<my::ThreadPool> &eventManager, nts::ManageIrrlicht &irrlicht
) : AEntity(entities, eventManager, irrlicht)
{
	std::cout << "Bomb constructor" << std::endl;
	_type = "Bomb";
	timer(2);
}

void is::Bomb::explode()
{
	std::cout << "Adding explode event !" << std::endl;
	_entities.lock();
	_eventManager.lock();

	_eventManager->enqueue([this] () {
		std::cout << "I'll explode all block around me ! MOUAHAHAH" << std::endl;
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
		std::cout << "I'll explode in " << time << " seconds" << std::endl;
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

