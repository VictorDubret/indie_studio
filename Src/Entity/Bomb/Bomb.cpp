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
		std::cerr << BLU << "je suis pas détruit" << RESET <<std::endl;
		_entities.lock();
		lock();
	}
	std::cerr << BLU << "je suis bloqué dans la déstruction" << RESET <<std::endl;
	_locked = true;
}

void is::Bomb::explode()
{
	if (_stopTimer) {
		return;
	}
	_stopTimer = true;
	_eventManager.lock();
	_eventManager->enqueue([this]() {
		std::cout << std::this_thread::get_id() << YEL << " " << this
			<< " : " << __PRETTY_FUNCTION__ << " LOCK" << RESET
			<< std::endl;
		lock();
		std::cout << std::this_thread::get_id() << YEL << " " << this
			<< " : " << __PRETTY_FUNCTION__ << " AFTER LOCK"
			<< RESET << std::endl;
		float x = getX();
		float z = getZ();
		_eventManager.lock();
		_eventManager->enqueue([this, x, z] {
			doExplosions(x, z);
		});
		_eventManager.unlock();
		auto tmp = dynamic_cast<is::ACharacter *>(_player.get());
		if (tmp)
			tmp->operator++();
		_eventManager.lock();
		_eventManager->enqueue([this] {
			std::cerr << BLU << "mmmmmmmmmmmmmmmmmmmmmmmh" << RESET << std::endl;
			this->~Bomb();
		});
		_eventManager.unlock();
		std::cout << std::this_thread::get_id() << YEL << " " << this
			<< " : " << __PRETTY_FUNCTION__ << " UNLOCK" << RESET
			<< std::endl;
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
	std::cout << std::this_thread::get_id() << YEL << " " << this << " : "
		<< __PRETTY_FUNCTION__ << " LOCK" << RESET << std::endl;
	lock();
	std::cout << std::this_thread::get_id() << YEL << " " << this << " : "
		<< __PRETTY_FUNCTION__ << " AFTER LOCK" << RESET << std::endl;
	nts::ManageObject::createCube(_irrlicht, _sptr, 0.9999);
	nts::ManageObject::setMaterialLight(_irrlicht, _sptr, false);
	nts::ManageObject::setTexture(_irrlicht, _sptr, "media/003shot.jpg");
	unlock();
	std::cout << std::this_thread::get_id() << YEL << " " << this << " : "
		<< __PRETTY_FUNCTION__ << " UNLOCK" << RESET << std::endl;
}

void is::Bomb::doExplosions(float x, float z)
{
	std::vector<std::shared_ptr<IEntity>> range;

	/*auto tmp = dynamic_cast<AEntity *>(_sptr.get());
	if (!tmp) {
		return;
	}*/
	_eventManager.lock();
	std::cerr << RED << "-----------------------------------" << RESET << std::endl;
	_eventManager->enqueue([&] {
		std::cout << std::this_thread::get_id() << " : "
			<< __PRETTY_FUNCTION__ << " LOCK" << std::endl;
		_entities.lock();
		std::cout << std::this_thread::get_id() << " : "
			<< __PRETTY_FUNCTION__ << " AFTER LOCK" << std::endl;
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
		std::cout << std::this_thread::get_id() << " : "
			<< __PRETTY_FUNCTION__ << " UNLOCK" << std::endl;
		_entities.unlock();
	});
	_eventManager.unlock();
	std::cout << std::this_thread::get_id() << YEL << " " << this << " : "
		<< __PRETTY_FUNCTION__ << " UNLOCK" << RESET << std::endl;
}

bool is::Bomb::check_arround(int lenExplosion, int actualPos,
	std::function<float(int)> f, is::Bomb::Axes_t which_axes, float x_bomb, float z_bomb
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
				createExplosion(f, which_axes, actualPos, x_bomb, z_bomb);
				it->unlock();
				stop = true;
				return false;
			} else if (it->getType() == "UnbreakableWall") {
				std::cerr << "UnbreakableWall" << std::endl;
				std::cerr << " X :" << it->getX() << " Z :" << it->getZ() << std::endl;
				std::cerr << YEL <<" X_BOMB :" << x_bomb << " Z_BOMB :" << z_bomb << RESET << std::endl;
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
	std::cerr << " X_BOMB :" << x_bomb << " Z_BOMB :" << z_bomb << std::endl;
	std::cerr << "Je suis sortit du foreach !!" << std::endl;
	if (stop)
		return false;
	createExplosion(f, which_axes, actualPos, x_bomb, z_bomb);
	check_arround(lenExplosion, actualPos + 1, f, which_axes, x_bomb, z_bomb);
	return false;
}

void is::Bomb::createExplosion(std::function<float(int)> &f,
	is::Bomb::Axes_t &which_axes, int &actualPos, float x_bomb, float z_bomb
)
{
	//if (dynamic_cast<AEntity *>(_sptr.get()) == nullptr)
	//	return;
	float x = (which_axes == XAXES) ? f(actualPos) : x_bomb;
	float z = (which_axes == ZAXES) ? f(actualPos) : z_bomb;
	std::cerr << "####### " << x << " " << z << std::endl;

	//_eventManager.lock();
	//_eventManager->enqueue([&]{
	std::cerr << "lol mdr" << std::endl;
	_eventManager.lock();
	_eventManager->enqueue([this, x, z]() {
		auto explosion = new is::Explosion(_entities, _eventManager,
			_irrlicht);
		std::cerr << "Je suis en cour de crea de l'explo " << std::endl;
		explosion->setX(x);
		explosion->setZ(z);
	});
	_eventManager.unlock();
	std::cerr << "J'ai fini de creer l'explo" << std::endl;
	//});
	//_eventManager.unlock();
}

bool is::Bomb::isWalkable(std::shared_ptr<is::IEntity> &entity) const
{
	/*_entities.lock();
	std::vector<std::shared_ptr<IEntity>> tmp = getEntitiesAt(getX(), getY(), getZ());
	for (const auto &it : tmp) {
		std::cout << "type : " << it->getType() << std::endl;
		if (entity.get() == it.get()) {
			return true;
		}
	}
	_entities.unlock();
	return false;*/
	return false;
}
