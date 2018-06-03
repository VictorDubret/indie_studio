//
// EPITECH PROJECT, 2018
// Indie Studio
// File description:
// AI class
//

#include 	<utility>
#include <Entity/Bomb/Bomb.hpp>
#include	"LevelOne.hpp"

is::LevelOne::LevelOne(Entity_t &entities, ThreadPool_t &eventManager,
		nts::ManageIrrlicht &irrlicht) :
		ACharacter(entities, eventManager, irrlicht), _entities(entities),
		_height(0), _width(0)
{
	getMapDimensions();
	std::pair<Type, IEntity *>tmp = std::pair<Type, IEntity *> (SAFE, nullptr);
	_map = std::vector<std::pair<Type, IEntity *>>(_height * _width, tmp);
	setWalls();
	updateMap();
}

void 	is::LevelOne::beAnAI()
{
	while (stillCrates() == true) {
		updateMap();
		if (inDanger() == true) {
			goToSafeSpot();
		}
		//dropBomb(); //test
		// powerup
		// crate
	}
}

bool 	is::LevelOne::stillCrates()
{
	for (std::size_t i = _width + 1 ; i < _width * _height - _width ; ++i) {
		if (_map[i].first == CRATE)
			return (true);
	}
	return (false);
}

bool is::LevelOne::inDanger()
{
	float size = _irrlicht.getNodeSize(_sptr);
	if (_map[getZ() * _width + getX()].first == BOMB || _map[getZ() * _width + getX()].first == DANGER)
		return (true);
	if (_map[(getZ() + size) * _width + (getX() + size)].first == BOMB || _map[(getZ() + size) * _width + (getX() + size)].first == DANGER)
		return (true);
	return (false);
}

void	is::LevelOne::getMapDimensions()
{
	for (const auto &it : _entities.get()) {
		if (it->getX() > _width)
			_width = static_cast<size_t>(it->getX());
		if (it->getZ() > _height)
			_height = static_cast<size_t>(it->getZ());
	}
	_height++;
	_width++;
	#ifdef DEBUG
	std::cout << "map height: " << _height << std::endl;
	std::cout << "map width: " << _width << std::endl;
	#endif
}

void	is::LevelOne::setWalls()
{
	for (const auto &it : _entities.get()) {
		if (it->getType() == "UnbreakableWall") {
			_map[(int)it->getX() + (int)it->getZ() * _width].first = WALL;
			_map[(int)it->getX() + (int)it->getZ() * _width].second = it.get();
		}
	}
	#ifdef DEBUG
	for (std::size_t i = 0 ; i < _width * _height ; ++i) {
		if (_map[i].first == SAFE)
			std::cout << "  ";
		else if (_map[i].first == WALL)
			std::cout << "##";
		if (i % _width == _width - 1)
			std::cout << std::endl;
	}
	#endif
}

void 	is::LevelOne::setDangerRec(std::size_t pos, int range, int dir)
{
	if (_map[pos + dir].first != WALL && range > 0) {
		if (_map[pos + dir].first != CRATE)
			setDangerRec(pos + dir, range - 1, dir);
		_map[pos + dir].first = DANGER;
		_map[pos - 1].second = nullptr;
	}
}

void 	is::LevelOne::setDanger(std::size_t pos, int range)
{
	int	dir[4] = { -1, 1, static_cast<int>(_width), static_cast<int>(_width * -1)};

	for (std::size_t i = 0 ; i < 4 ; ++i) {
		if (_map[pos + dir[i]].first != WALL && range > 0) {
			if (_map[pos + dir[i]].first != CRATE)
				setDangerRec(pos, range - 1, dir[i]);
			_map[pos + dir[i]].first = DANGER;
			_map[pos + dir[i]].second = nullptr;
		}
	}
	/*if (_map[pos + 1].first != WALL && range > 0) {
		if (_map[pos + 1].first != CRATE)
			setDangerRec(pos, range - 1, 1);
		_map[pos + 1].first = DANGER;
		_map[pos + 1].second = nullptr;
	}
	if (_map[pos - _width].first != WALL && range > 0) {
		if (_map[pos - _width].first != CRATE)
			setDangerRec(pos, range - 1, static_cast<int>(_width * -1));
		_map[pos - _width].first = DANGER;
		_map[pos - _width].second = nullptr;
	}
	if (_map[pos + _width].first != WALL && range > 0) {
		if (_map[pos + _width].first != CRATE)
			setDangerRec(pos, range - 1, static_cast<int>(_width));
		_map[pos + _width].first = DANGER;
		_map[pos + _width].second = nullptr;
	}*/
}

void 	is::LevelOne::addDangerZones()
{
	for (std::size_t i = _width + 1 ; i < _width * _height - _width ; ++i) {
		if (_map[i].first == BOMB) {
			setDanger(i, dynamic_cast<is::Bomb *>(_map[i].second)->_lenExplosion);
		}
	}
}

void 	is::LevelOne::goToSafeSpot()
{
}

void 	is::LevelOne::updateMap()
{
	for (const auto &it : _entities.get()) {
		if (_map[(int)it->getX() + (int)it->getZ() * _width].first != WALL && it->getType() != "Character") {
			if (it->getType() == "Wall")
				_map[(int)it->getX() + (int)it->getZ() * _width].first = CRATE;
			if (it->getType() == "Explosion")
				_map[(int)it->getX() + (int)it->getZ() * _width].first = DANGER;
			if (it->getType() == "Bomb")
				_map[(int)it->getX() + (int)it->getZ() * _width].first = BOMB;
			if (it->getType() == "BombUp" || it->getType() == "FireUp" ||
				it->getType() == "SpeedUp" || it->getType() == "WallPass")
				_map[(int)it->getX() + (int)it->getZ() * _width].first = POWERUP;
			_map[(int)it->getX() + (int)it->getZ() * _width].second = it.get();
		}
	}
	addDangerZones();
	#ifdef DEBUG
	for (std::size_t i = 0 ; i < _width * _height ; ++i) {
		if (_map[i].first == SAFE)
			std::cout << "  ";
		else if (_map[i].first == WALL || _map[i].first == BOMB)
			std::cout << "##";
		else if (_map[i].first == CRATE)
			std::cout << "00";
		else if (_map[i].first == DANGER)
			std::cout << "XX";
		else if (_map[i].first == POWERUP)
			std::cout << "!!";
		if (i % _width == _width - 1)
			std::cout << std::endl;
	}
	#endif
}
