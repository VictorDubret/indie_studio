//
// Created by victor on 27/05/18.
// File description:
// Bomb.cpp
//

#include <exception>
#include <algorithm>
#include "ItemLocker.hpp"
#include "Bomb.hpp"
#include "Explosion.hpp"
#include "Timer.hpp"
#include "ManageObject.hpp"

is::Bomb::Bomb(my::ItemLocker<std::vector<std::shared_ptr<IEntity>>> &entities,
	my::ItemLocker<my::ThreadPool> &eventManager,
	std::shared_ptr<IEntity> &Player, nts::ManageIrrlicht &irrlicht,
	size_t time
) : AEntity(entities, eventManager, irrlicht), _player(Player)
{
	std::cout << "Bomb constructor" << std::endl;
	_type = "Bomb";
	auto tmp = dynamic_cast<is::ACharacter *>(_player.get());
	if (tmp == nullptr) {
		delete this;
	}
	_lenExplosion = tmp->getBombLength();
	std::cout << _lenExplosion << std::endl;
	texture();
	timer(time);
}

void is::Bomb::explode()
{
	std::cout << "Adding explode event !" << std::endl;
	if (_stopTimer) {
		return;
	}
	_entities.lock();
	_eventManager.lock();

	_eventManager->enqueue([this]() {
		_stopTimer = true;

		fprintf(stderr, "addr :%p\n", this);
		std::cerr << "Je passe dans la fonction de destruction d'une bombe ^^" << std::endl;


		std::cout << "I'll explode all blocks around me ! MOUAHAHAH"
			<< std::endl;
		std::vector<std::shared_ptr<IEntity>> xAxes;
		for (int i = (_lenExplosion * -1); i <= _lenExplosion; ++i) {
			std::vector<std::shared_ptr<IEntity>> tmp = this->getEntitiesAt(
				(int)this->getX() - i, (int)this->getY(),
				(int)this->getZ());
			xAxes.insert(xAxes.end(), tmp.begin(), tmp.end());
			auto a = new is::Explosion(_entities, _eventManager,
				_irrlicht);

			a->setX((int)getX() - i);
			a->setY((int)getY());
			a->setZ((int)getZ());
		}
		auto it_tmp = std::unique(xAxes.begin(), xAxes.end());

		xAxes.resize(std::distance(xAxes.begin(), it_tmp));
		for (const auto &it : xAxes) {
			if (it.get() != this) {
				std::cout << "J'ai recuperer sur X : "
					<< it.get()->getType() << std::endl;

				if (it->getType() == "Bomb") {
					fprintf(stderr, "je vais explode %p", it.get());
				}

				it->explode();
			}
		}
		std::vector<std::shared_ptr<IEntity>> zAxes;
		for (int i = (_lenExplosion * -1); i <= _lenExplosion; ++i) {
			std::vector<std::shared_ptr<IEntity>> tmp = this->getEntitiesAt(
				(int)this->getX(), (int)this->getY(),
				(int)this->getZ() - i);
			zAxes.insert(zAxes.end(), tmp.begin(), tmp.end());

			auto a = new is::Explosion(_entities, _eventManager,
				_irrlicht);

			a->setX((int)getX());
			a->setY((int)getY());
			a->setZ((int)getZ() - i);
		}
		auto it_tmp2 = std::unique(zAxes.begin(), zAxes.end());

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
		Timer t;

		t.startTimer(time);

		while (!t.isOver() && !_stopTimer);
		std::cout << "I'll explode in " << time << " seconds"
			<< std::endl;
		//Change animation flamish bomb
		//std::cerr << "Bouuum" << std::endl;
		fprintf(stderr, "Bouum de %p\n", this);
		if (_stopTimer) {
			return;
		}
		explode();
	});
	_eventManager.unlock();
}

void is::Bomb::texture()
{
	nts::ManageObject::createCube(_irrlicht, _sptr, 1);
	_irrlicht.getNode(_sptr)->setPosition(irr::core::vector3df(0, 0, 0));
	nts::ManageObject::setMaterialLight(_irrlicht, _sptr, false);
	nts::ManageObject::setTexture(_irrlicht, _sptr, "media/003shot.jpg");
}