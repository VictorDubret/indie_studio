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
#include "Debug.hpp"

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
		this->~Bomb();
		return;
	}
	_lenExplosion = tmp->getBombLength();
	_collidable = true;
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
		std::cerr << "Je passe dans la fonction de "
			"destruction d'une bombe ^^" << std::endl;
		std::cout << "I'll explode all blocks around me ! MOUAHAHAH"
			<< std::endl;
		doExplosions();
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
	lock();
	nts::ManageObject::createCube(_irrlicht, _sptr, 0.99);
	nts::ManageObject::setMaterialLight(_irrlicht, _sptr, false);
	nts::ManageObject::setTexture(_irrlicht, _sptr, "media/003shot.jpg");
	unlock();
}

void is::Bomb::doExplosions()
{
	std::vector<std::shared_ptr<IEntity>> range;
	check_arround(_lenExplosion, 0, [this](int actualPos) {
		return getX() - actualPos;
	}, XAXES);
	check_arround(_lenExplosion, 0, [this](int actualPos) {
		return getX() + actualPos;
	}, XAXES);
	check_arround(_lenExplosion, 0, [this](int actualPos) {
		return getZ() - actualPos;
	}, ZAXES);
	check_arround(_lenExplosion, 0, [this](int actualPos) {
		return getZ() + actualPos;
	}, ZAXES);
}

bool is::Bomb::check_arround(int lenExplosion, int actualPos,
	std::function<float(int)> f, is::Bomb::Axes_t which_axes
)
{
	if (actualPos > lenExplosion)
		return false;
	std::vector<std::shared_ptr<IEntity>> tmp =
		(which_axes == XAXES) ? getEntitiesAt(f(actualPos), 0, getZ()) :
			getEntitiesAt(getX(), 0, f(actualPos));
	_entities.lock();
	for (const auto &it : tmp) {
		if (it->getType() == "Wall") {
			_entities.unlock();
			it->explode();
			createExplosion(f, which_axes, actualPos);
			return false;
		} else if (it->getType() == "UnbreakableWall") {
			std::cout << RED << " UNBREAKABLE" << RESET << std::endl;
			_entities.unlock();
			return false;
		}
		it->explode();
	}
	_entities.unlock();
	createExplosion(f, which_axes, actualPos);
	check_arround(lenExplosion, actualPos + 1, f, which_axes);
	return false;
}

void is::Bomb::createExplosion(std::function<float(int)> &f,
	is::Bomb::Axes_t &which_axes, int &actualPos)
{
	auto explosion = new is::Explosion(_entities, _eventManager,
		_irrlicht);
	(which_axes == XAXES) ? explosion->setX(f(actualPos)) :
		explosion->setX(getX());
	explosion->setY(0);
	(which_axes == ZAXES) ? explosion->setZ(f(actualPos)) :
		explosion->setZ(getZ());
}

bool is::Bomb::isWalkable(std::shared_ptr<is::IEntity> &entity) const
{
	std::vector<std::shared_ptr<IEntity>> tmp = getEntitiesAt(getX(), getY(), getZ());
	for (const auto &it : tmp) {
		std::cout << "type : " << it->getType() << std::endl;
		if (entity.get() == it.get()) {
			return true;
		}
	}
	return false;
}
