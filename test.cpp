#include "ACharacter.hpp"
#include <iostream>
#include "SpeedUp.hpp"
#include "WallPass.hpp"
#include "BombUp.hpp"
#include "FireUp.hpp"
#include "ThreadPool.hpp"
#include "ItemLocker.hpp"

int main(int ac, char **av)
{
	(void) ac;
	(void) av;
	my::ThreadPool eventManager(100);
	std::vector<std::shared_ptr<is::IEntity>> entities;
	my::ItemLocker<my::ThreadPool> eventManagerLocker(eventManager);
	my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> entitiesLocker(entities);

	{
		is::ACharacter character(entitiesLocker, eventManagerLocker);
		is::ACharacter character1(entitiesLocker, eventManagerLocker);

		std::cout << "Entities size (Before collision): " << entitiesLocker->size() << std::endl;
		character.checkCollision();
		character.dropBomb();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		std::cout << "Entities size (After collision and event): " << entitiesLocker->size() << std::endl;
	}

	std::cout << "Entities size at the end: " << entities.size() << std::endl;
}