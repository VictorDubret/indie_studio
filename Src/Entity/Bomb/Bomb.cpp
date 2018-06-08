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
	if (!_locked) {
		_entities.lock();
		lock();
	}
	_locked = true;
}

void is::Bomb::isPaused(const bool isPaused)
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
		lock();
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
			if (!dynamic_cast<Bomb *>(_sptr.get())) {
				_entities.unlock();
				return;
			}
			this->lock();
			_locked = true;
			this->~Bomb();
		});
		_eventManager.unlock();
		unlock();
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
		std::cout << "I'll explode in " << time << " seconds" << std::endl;
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
	nts::ManageObject::createAnim(_irrlicht, _sptr, "media/bomb.b3d",
		0.75);
	_irrlicht.getNode(_sptr.get())->setPosition(
		irr::core::vector3df(1.1f, 0.1f, 1.1f));
	nts::ManageObject::setScale(_irrlicht, _sptr,
		irr::core::vector3df(1, 1, 1));
	nts::ManageObject::setRotation(_irrlicht, _sptr,
		irr::core::vector3df(0, 90, 0));
	nts::ManageObject::setMaterialLight(_irrlicht, _sptr, false);
	nts::ManageObject::setTexture(_irrlicht, _sptr, "media/bomb.png");
}

size_t is::Bomb::getLenExplosion() const
{
	return _lenExplosion;
}

void is::Bomb::doExplosions(float x, float z)
{
	std::vector<std::shared_ptr<IEntity>> range;

	std::cerr << RED << "-----------------------------------" << RESET
		<< std::endl;
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
	std::vector<std::shared_ptr<IEntity>> tmp = getEntitiesAt(x, 0, z);
	std::for_each(tmp.begin(), tmp.end(),
		[&](std::shared_ptr<IEntity> &it) {
			auto tmp_it = dynamic_cast<AEntity *>(it.get());
			if (!tmp_it || stop)
				return false;
			it->lock();
			if (it->getType() == "Wall") {
				std::cerr << "Wall" << std::endl;

				it->explode();
				createExplosion(f, which_axes, actualPos,
					x_bomb, z_bomb);
				it->unlock();
				stop = true;
				return false;
			} else if (it->getType() == "UnbreakableWall") {
				std::cerr << "UnbreakableWall" << std::endl;
				std::cerr << " X :" << it->getX() << " Z :"
					<< it->getZ() << std::endl;
				std::cerr << YEL << " X_BOMB :" << x_bomb
					<< " Z_BOMB :" << z_bomb << RESET
					<< std::endl;
				it->unlock();
				stop = true;
				return false;
			}
			if (it.get() != this)
				it->explode();
			it->unlock();
			std::cerr << "Normalement c ok" << std::endl;
			return true;
		});
	std::cerr << " X_BOMB :" << x_bomb << " Z_BOMB :" << z_bomb
		<< std::endl;
	std::cerr << "Je suis sortit du foreach !!" << std::endl;
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
	std::cerr << "####### " << x << " " << z << std::endl;

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
	/*unlock();
	std::vector<std::shared_ptr<IEntity>> tmp = getEntitiesAt(getX(),
		getY(), getZ());
	lock();
	for (const auto &it : tmp) {
		std::cout << "type : " << it->getType() << std::endl;
		if (entity.get() == it.get()) {
			return true;
		}
	}
	_entities.unlock();*/
	return true;
}
