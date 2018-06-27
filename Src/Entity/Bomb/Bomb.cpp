//
// EPITECH PROJECT, 2018
// Indie Studio
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
	std::shared_ptr<IEntity> &Player, irrl::ManageIrrlicht &irrlicht,
	size_t time
) : AEntity(entities, eventManager, irrlicht), _player(Player)
{
	_type = "Bomb";
	auto tmp = dynamic_cast<is::ACharacter *>(_player.get());
	if (tmp == nullptr) {
		this->~Bomb();
		return;
	}
	_lenExplosion = tmp->getBombLength();
	_collidable = true;
	_wallPassable = false;
	texture();
	timer(time);
}

is::Bomb::~Bomb()
{
	if (!_locked) {
		_entities.lock();
		_mutex.lock();
	}
	_locked = true;
}

void is::Bomb::setPaused(const bool isPaused)
{
	_isPaused = isPaused;
}

void is::Bomb::explode()
{
	if (_stopTimer) {
		return;
	}
	_stopTimer = true;
	_eventManager.lock();
	_eventManager->enqueue([this]() {
		std::lock_guard<std::recursive_mutex> lock(_mutex);
		_eventManager.lock();
		float x = getX();
		float z = getZ();
		_eventManager->enqueue([this, x, z] {
			_entities.lock();
			doExplosions(x, z);
			_entities.unlock();
		});
		_eventManager.unlock();
		auto tmp = dynamic_cast<is::ACharacter *>(_player.get());
		if (tmp)
			tmp->operator++();
		_eventManager.lock();
		_eventManager->enqueue([this] {
			_entities.lock();
			if (!dynamic_cast<Bomb *>(_spointer.get())) {
				_entities.unlock();
				return;
			}
			_mutex.lock();
			_locked = true;
			this->~Bomb();
		});
		_eventManager.unlock();
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

		bool paused = false;
		bool wait = false;
		bool test = false;
		while (!wait) {
			while (_isPaused) {
				paused = true;
				wait = true;
				test = true;
			}
			if (!_isPaused && !test) {
				if ((t.isOver() || _stopTimer)) {
					wait = true;
					break;
				}
			}
		}
		if (paused) {
			t.startTimer(time);
		}

		while ((!t.isOver() && !_stopTimer));
		if (_stopTimer) {
			return;
		}
		explode();
	});
	_eventManager.unlock();
}

void is::Bomb::texture()
{
	irrl::ManageObject::createAnim(_irrlicht, _spointer, "media/bomb.b3d", 0.75);
	_irrlicht.getNode(_spointer.get())->setPosition(
		irr::core::vector3df(1.1f, -0.5f, 1.1f));
	irrl::ManageObject::setScale(_irrlicht, _spointer,
		irr::core::vector3df(1, 1, 1));
	irrl::ManageObject::setRotation(_irrlicht, _spointer,
		irr::core::vector3df(0, 90, 0));
	irrl::ManageObject::setMaterialLight(_irrlicht, _spointer, false);
	irrl::ManageObject::setTexture(_irrlicht, _spointer, "media/bomb.png");
}

size_t is::Bomb::getLenExplosion() const
{
	return _lenExplosion;
}

void is::Bomb::doExplosions(float x, float z)
{
	std::vector<std::shared_ptr<IEntity>> range;

	check_arround(_lenExplosion, 0, [x](int actualPos) {
		return x - actualPos;
	}, XAXES, x, z);
	check_arround(_lenExplosion, 1, [x](int actualPos) {
		return x + actualPos;
	}, XAXES, x, z);
	check_arround(_lenExplosion, 1, [z](int actualPos) {
		return z - actualPos;
	}, ZAXES, x, z);
	check_arround(_lenExplosion, 1, [z](int actualPos) {
		return z + actualPos;
	}, ZAXES, x, z);
}

bool is::Bomb::check_arround(int lenExplosion, int actualPos,
	std::function<float(int)> f, is::Bomb::Axes_t which_axes, float x_bomb,
	float z_bomb
)
{
	bool stop = false;
	if (actualPos > lenExplosion)
		return false;
	float x = (which_axes != XAXES) ? x_bomb : f(actualPos);
	float z = (which_axes != ZAXES) ? z_bomb : f(actualPos);
	std::vector<std::shared_ptr<IEntity>> tmp = getEntitiesAt(x, z);
	std::for_each(tmp.begin(), tmp.end(),
		[&](std::shared_ptr<IEntity> &it) {
			auto tmp_it = dynamic_cast<AEntity *>(it.get());
			if (!tmp_it || stop)
				return false;
			std::lock_guard<std::recursive_mutex> lock(it->getMutex());

			if (it->getType() == "Wall") {
				it->explode();
				createExplosion(f, which_axes, actualPos,
					x_bomb, z_bomb);
				stop = true;
				return false;
			} else if (it->getType() == "UnbreakableWall") {
				stop = true;
				return false;
			}
			if (it.get() != this)
				it->explode();
			return true;
		});
	if (stop)
		return false;
	createExplosion(f, which_axes, actualPos, x_bomb, z_bomb);
	check_arround(lenExplosion, actualPos + 1, f, which_axes, x_bomb,
		z_bomb);
	return false;
}

void is::Bomb::createExplosion(std::function<float(int)> &f,
	is::Bomb::Axes_t &which_axes, int &actualPos, float x_bomb, float z_bomb
)
{
	float x = (which_axes == XAXES) ? f(actualPos) : x_bomb;
	float z = (which_axes == ZAXES) ? f(actualPos) : z_bomb;
	_eventManager.lock();
	_eventManager->enqueue([this, x, z]() {
		_entities.lock();
		auto explosion = new is::Explosion(_entities, _eventManager,
			_irrlicht);
		explosion->setX(x);
		explosion->setZ(z);
		_entities.unlock();
	});
	_eventManager.unlock();
}

bool is::Bomb::isWalkable(std::shared_ptr<is::IEntity> &entity)
{
	std::vector<std::shared_ptr<IEntity>> tmp_down = getEntitiesAt(
		getX() + 0.15, getZ() + 0.15);
	std::vector<std::shared_ptr<IEntity>> tmp_up = getEntitiesAt(
		getX() - 0.15, getZ() - 0.15);
	for (const auto &it : tmp_down) {
		if (entity.get() == it.get()) {
			for (const auto &tmp_entity : tmp_up) {
				if (tmp_entity.get() == entity.get()) {
					_entities.unlock();
					return true;
				}
			}
		}
	}
	_entities.unlock();
	return false;
}
