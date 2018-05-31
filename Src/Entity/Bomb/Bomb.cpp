//
// Created by victor on 27/05/18.
// File description:
// Bomb.cpp
//

#include <algorithm>
#include <algorithm>
#include "ItemLocker.hpp"
#include "Bomb.hpp"
#include "Timer.hpp"

is::Bomb::Bomb(my::ItemLocker<std::vector<std::shared_ptr<IEntity>>> &entities,
	my::ItemLocker<my::ThreadPool> &eventManager,
	std::shared_ptr<ACharacter> Player, size_t time
) : AEntity(entities, eventManager)
{
	std::cout << "Bomb constructor" << std::endl;
	_type = "Bomb";
	_player = Player;
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

		/*auto test = std::find_if(
			xAxes.begin(), xAxes.end(),
			[this](std::shared_ptr<IEntity> entity) {
				if (entity.get() == this) {
					return entity;
				}
			});*/
		std::vector<std::shared_ptr<IEntity>> yAxes;
		for (int i = (_lenExplosion * -1); i <= _lenExplosion; ++i) {
			std::vector<std::shared_ptr<IEntity>> tmp = this->getEntitiesAt(
				this->getX(), this->getY() - i, this->getZ());
			yAxes.insert(yAxes.end(), tmp.begin(), tmp.end());
		}
		for (const auto &it : xAxes) {
			if (it.get() != this) {
				std::cout << "J'ai recuperer : "
					<< it.get()->getType() << std::endl;
				it.get()->explode();
			}
		}
		for (const auto &it : yAxes) {
			if (it.get() != this) {
				std::cout << "J'ai recuperer : "
					<< it.get()->getType() << std::endl;
				it.get()->explode();
			}
		}
		std::cout << "I'll explode all block around me ! MOUAHAHAH"
			<< std::endl;
		_player.get()->setBomb(1);
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

std::vector<std::shared_ptr<is::IEntity>> is::Bomb::getEntitesInRange(int from,
	int to
)
{
	return std::vector<std::shared_ptr<is::IEntity>>();
}
