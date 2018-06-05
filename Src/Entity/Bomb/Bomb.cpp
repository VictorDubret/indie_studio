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

is::Bomb::~Bomb()
{
	if (!_locked)
		_entities.lock();
	_locked = true;
}

void is::Bomb::explode()
{
	std::cout << "Adding explode event !" << std::endl;
	if (_stopTimer) {
		return;
	}
	_stopTimer = true;
	std::cout << RED << __PRETTY_FUNCTION__ << " LOCK" << RESET << std::endl;
	_eventManager.lock();
	_eventManager->enqueue([this]() {

		fprintf(stderr, "addr :%p\n", this);
		std::cerr << "Je passe dans la fonction de "
			"destruction d'une bombe ^^" << std::endl;
		std::cout << "I'll explode all blocks around me ! MOUAHAHAH"
			<< std::endl;
		doExplosions();
		auto tmp = dynamic_cast<is::ACharacter *>(_player.get());
		if (tmp)
			tmp->operator++();
		auto tmp_this = dynamic_cast<Bomb *>(this);
		if (!tmp_this)
			return;
		this->~Bomb();
		std::cout << GRN << __PRETTY_FUNCTION__ << " UNLOCK" << RESET << std::endl;
		return;
	});
	_eventManager.unlock();
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
	nts::ManageObject::createCube(_irrlicht, _sptr, 0.9999);
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
	bool stop = false;

	if (actualPos > lenExplosion)
		return false;
	std::vector<std::shared_ptr<IEntity>> tmp =
		(which_axes == XAXES) ? getEntitiesAt(f(actualPos), 0, getZ()) :
			getEntitiesAt(getX(), 0, f(actualPos));
	std::cout << RED << __PRETTY_FUNCTION__ << " LOCK" << RESET << std::endl;
	_entities.lock();
	std::for_each(tmp.begin(), tmp.end(), [&](std::shared_ptr<IEntity> &it) {
		auto tmp_it = dynamic_cast<AEntity *>(it.get());
		if (!tmp_it || stop)
			return false;
		if (it->getType() == "Wall") {
			_entities.unlock();std::cout << GRN << __PRETTY_FUNCTION__ << " UNLOCK" << RESET << std::endl;
			if (dynamic_cast<is::AEntity *>(it.get()) == nullptr) {
				return false;
			}
			it->lock();
			if (dynamic_cast<is::AEntity *>(it.get()) == nullptr) {
				return false;
			}
			it->explode();
			createExplosion(f, which_axes, actualPos);
			stop = true;
			return false;
		} else if (it->getType() == "UnbreakableWall") {
			std::cout << RED << " UNBREAKABLE" << RESET << std::endl;
			_entities.unlock();std::cout << GRN << __PRETTY_FUNCTION__ << " UNLOCK" << RESET << std::endl;
			stop = true;
			return false;
		}
		_entities.unlock(); std::cout << GRN << __PRETTY_FUNCTION__ << " UNLOCK" << RESET << std::endl;
//		it->lock();
		if (dynamic_cast<is::AEntity *>(it.get()) == nullptr) {
			return false;
		}
		it->explode();
//		it->unlock();
		std::cout << RED << __PRETTY_FUNCTION__ << " LOCK" << RESET << std::endl;
		_entities.lock();
		return true;
	});
	if (stop)
		return false;
	_entities.unlock(); std::cout << GRN << __PRETTY_FUNCTION__ << " UNLOCK" << RESET << std::endl;
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
