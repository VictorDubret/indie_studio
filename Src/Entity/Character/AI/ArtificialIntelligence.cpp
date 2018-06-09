//
// EPITECH PROJECT, 2018
// Indie Studio
// File description:
// AI class
//

#include 	<utility>
#include 	<Bomb.hpp>
#include        "ArtificialIntelligence.hpp"

is::ArtificialIntelligence::ArtificialIntelligence(Entity_t &entities, ThreadPool_t &eventManager,
		nts::ManageIrrlicht &irrlicht, std::size_t id) :
		ACharacter(entities, eventManager, irrlicht, id), _entities(entities),
		_height(0), _width(0)
{
	_entities.lock();
	getMapDimensions();
	std::pair<Type, IEntity *>tmp = std::pair<Type, IEntity *> (SAFE, nullptr);
	if (!dynamic_cast<ACharacter *>(_sptr.get()))
		return;
	_position.first = getX();
	_position.second = getZ();
	_goal.first = -1;
	_goal.second = -1;
	_map = std::vector<std::pair<Type, IEntity *>>(_height * _width, tmp);
	updateMap();
	_entities.unlock();
}

void 	is::ArtificialIntelligence::AIsTurn()
{
	Direction	dir;

	if (!dynamic_cast<ACharacter *>(_sptr.get()))
		return;
	_entities.lock();
	updateMap();
	for (int i = 0 ; i < _width * _height ; i++) {
		if (_map[i].first == SAFE)
			std::cout << "  ";
		if (_map[i].first == WALL)
			std::cout << "##";
		if (_map[i].first == CRATE)
			std::cout << "00";
		if (_map[i].first == BOMB)
			std::cout << "BB";
		if (_map[i].first == POWERUP)
			std::cout << "!!";
		if (_map[i].first == EXPLOSION)
			std::cout << "EE";
		if (_map[i].first == DANGER)
			std::cout << "XX";
		if (i % _width == _width - 1)
			std::cout << std::endl;
	}
	_entities.unlock();
	if (inDanger() == true) {
		std::cout << "inDanger() = true" << std::endl;
		dir = lookForAZone(SAFE);
		headTowards(dir);
		_entities.lock();
		if (!dynamic_cast<ACharacter *>(_sptr.get()))
			return;
		_position.first = getX();
		_position.second = getZ();
		_entities.unlock();
		return;
	}
	dir = lookForAZone(POWERUP);
	if (dir != NONE) {
		std::cout << "lookForAZone(POWERUP)" << std::endl;
		headTowards(dir);
		_entities.lock();
		if (!dynamic_cast<ACharacter *>(_sptr.get()))
			return;
		_position.first = getX();
		_position.second = getZ();
		_entities.unlock();
		return;
	}
	if (safeBombDrop() == true) {
		if (onCrate() == false && _bomb == _bombMax) {
			std::cout << "onCrate() = false" << std::endl;
			dropBomb();
		} else {
			std::cout << "onCrate() = true" << std::endl;
			dir = lookForAZone(SAFE);
			headTowards(dir);
		}
	} else {
		std::cout << "else -> crate" << std::endl;
		dir = lookForAZone(CRATE);
		headTowards(dir);
	}
	_entities.lock();
	if (!dynamic_cast<ACharacter *>(_sptr.get()))
		return;
	_position.first = getX();
	_position.second = getZ();
	_entities.unlock();
}

bool is::ArtificialIntelligence::inDanger()
{
	float size = _irrlicht.getNodeSize(_sptr);
	if (_map[(int)_position.second * _width + (int)_position.first].first == BOMB || _map[(int)_position.second * _width + (int)_position.first].first == DANGER)
		return (true);
	if (_map[(int)(_position.second + size) * _width + (int)(_position.first + size)].first == BOMB || _map[(int)(_position.second + size) * _width + (int)(_position.first + size)].first == DANGER)
		return (true);
	return (false);
}

void	is::ArtificialIntelligence::getMapDimensions()
{
	for (const auto &it : _entities.get()) {
		if (it->getX() > _width)
			_width = static_cast<size_t>(it->getX());
		if (it->getZ() > _height)
			_height = static_cast<size_t>(it->getZ());
	}
	_height++;
	_width++;
}

void	is::ArtificialIntelligence::setWalls()
{
	for (const auto &it : _entities.get()) {
		if (it->getType() == "UnbreakableWall") {
			_map[(int)it->getX() + (int)it->getZ() * _width].first = WALL;
			_map[(int)it->getX() + (int)it->getZ() * _width].second = it.get();
		}
	}
}

void 	is::ArtificialIntelligence::setDangerRec(std::size_t pos, std::size_t range, int dir)
{
	if (_map[pos + dir].first != WALL && range > 0) {
		if (_map[pos + dir].first != CRATE)
			setDangerRec(pos + dir, range - 1, dir);
		if (_map[pos + dir].first != EXPLOSION && _map[pos + dir].first != CRATE)
			_map[pos + dir].first = DANGER;
		_map[pos - 1].second = nullptr;
	}
}

bool 	is::ArtificialIntelligence::setDangerRec(std::size_t pos, std::size_t range, int dir, std::vector <std::pair<Type, IEntity *>> map)
{
	bool 	reachCrate = false;

	if (map[pos + dir].first != WALL && range > 0) {
		if (map[pos + dir].first != CRATE)
			reachCrate = setDangerRec(pos + dir, range - 1, dir, map);
		else if (range > 1)
			reachCrate = true;
		else
			reachCrate = false;
		if (map[pos + dir].first != EXPLOSION && map[pos + dir].first != CRATE)
			map[pos + dir].first = DANGER;
	}
	return (reachCrate);
}

void 	is::ArtificialIntelligence::addDangerZones()
{
	int	dir[4] = { -1, 1, static_cast<int>(_width), static_cast<int>(_width * -1)};

	for (std::size_t i = _width + 1 ; i < _width * _height - _width ; ++i) {
		if (_map[i].first == BOMB) {
			auto tmp_bomb = dynamic_cast<is::Bomb *>(_map[i].second);
			if (tmp_bomb) {
				for (std::size_t j = 0; j < 4; ++j)
					setDangerRec(i, tmp_bomb->getLenExplosion() + 1, dir[j]);
			}
		}
	}
}

bool 	is::ArtificialIntelligence::addDangerZones(std::vector <std::pair<Type, IEntity *>> map)
{
	int	dir[4] = { -1, 1, static_cast<int>(_width), static_cast<int>(_width * -1)};
	bool 	reachCrate = false;

	for (std::size_t i = _width + 1 ; i < _width * _height - _width ; ++i) {
		if (map[i].first == BOMB) {
			auto tmp_bomb = dynamic_cast<is::Bomb *>(map[i].second);
			if (tmp_bomb) {
				for (std::size_t j = 0; j < 4; ++j)
					setDangerRec(i, tmp_bomb->getLenExplosion() + 1, dir[j], map);
			} else {
				for (std::size_t j = 0; j < 4; ++j)
					reachCrate = setDangerRec(i, getBombLength() + 1, dir[j], map) ? true : reachCrate;
			}
		}
	}
	return reachCrate;
}

int 	is::ArtificialIntelligence::getDist(int pos, std::vector<int> map, int dist)
{
	if (dist <= 0 || map[pos] == -3 || (map[pos] == -2 && _map[(int)(_position.second + 0.15) * _width + (int)(_position.first + 0.15)].first != DANGER &&
		_map[(int)(_position.second + 0.15) * _width + (int)(_position.first + 0.15)].first != BOMB))
		return (100);
	if (map[pos] == -1)
		return (0);

	map[pos] = -3;
	int	left = getDist(pos - 1, map, dist - 1);
	int	right = getDist(pos + 1, map, dist - 1);
	int	up = getDist(static_cast<int>(pos + _width), map, dist - 1);
	int	down = getDist(static_cast<int>(pos - _width), map, dist - 1);
	if (left <= right && left <= up && left <= down)
		return (left + 1);
	if (right <= up && right <= down)
		return (right + 1);
	if (up <= down)
		return (up + 1);
	return (down + 1);
}

is::ArtificialIntelligence::Direction 	is::ArtificialIntelligence::breadthFirst(int pos, std::vector<int> &map)
{
	map[pos] = -3;
	int	left = getDist(pos - 1, map, 5);
	int	right = getDist(pos + 1, map, 5);
	int	up = getDist(static_cast<int>(pos - _width), map, 5);
	int	down = getDist(static_cast<int>(pos + _width), map, 5);
	std::cout << "left: " << left << std::endl;
	std::cout << "right: " << right << std::endl;
	std::cout << "up: " << up << std::endl;
	std::cout << "down: " << down << std::endl;

	if (left >= 100 && right >= 100 && up >= 100 && down >= 100)
		return (NONE);
	if (left <= right && left <= up && left <= down)
		return (LEFT);
	if (right <= up && right <= down)
		return (RIGHT);
	if (up <= down)
		return (DOWN);
	return (UP);
}

is::ArtificialIntelligence::Direction 	is::ArtificialIntelligence::lookForAZone(Type type)
{
	std::vector<int>	map(_width * _height, 0);
	int 			start = static_cast<int>(static_cast<int>(_position.second + 0.2) * _width + static_cast<int>(_position.first + 0.2));

	for (std::size_t i = 0 ; i < _width * _height ; ++i) {
		if (_map[i].first == WALL || _map[i].first == BOMB || _map[i].first == EXPLOSION ||
			(getWallPass() == false && _map[i].first == CRATE))
			map[i] = -3;
		if (_map[i].first == DANGER)
			map[i] = -2;
		if (_map[i].first == type)
			map[i] = -1;
	}
	return (breadthFirst(start, map));
}

is::ArtificialIntelligence::Direction 	is::ArtificialIntelligence::lookForAZone(Type type, std::vector<std::pair<Type, IEntity *>>map)
{
	std::vector<int>	minMap(_width * _height, 0);
	// + 0.15
	int 			start = static_cast<int>(static_cast<int>(_position.second + 0.2) * _width + static_cast<int>(_position.first + 0.2));

	for (std::size_t i = 0 ; i < _width * _height ; ++i) {
		if (map[i].first == WALL || map[i].first == BOMB || map[i].first == EXPLOSION ||
			(getWallPass() == false && map[i].first == CRATE))
			minMap[i] = -3;
		if (map[i].first == DANGER)
			minMap[i] = -2;
		if (map[i].first == type)
			minMap[i] = -1;
	}
	return (breadthFirst(start, minMap));
}

bool	is::ArtificialIntelligence::safeBombDrop()
{
	std::vector<std::pair<Type, IEntity *>> map = _map;

	map[(int)(_position.first + _irrlicht.getNodeSize(_sptr) / 2) + (int)(_position.second + _irrlicht.getNodeSize(_sptr) / 2) * _width].first = BOMB;
	map[(int)(_position.first + _irrlicht.getNodeSize(_sptr) / 2) + (int)(_position.second + _irrlicht.getNodeSize(_sptr) / 2) * _width].second = nullptr;
	if (addDangerZones(map) == false) {
		return (false);
	}
	if (lookForAZone(SAFE, map) == NONE) {
		return (false);
	}
	return (true);
}

bool 	is::ArtificialIntelligence::onCrate()
{
	if (_map[(int)(_position.first + 0.15) + (int)(_position.second + 0.15) * _width].first == BOMB ||
		_map[(int)(_position.first + 0.15) + (int)(_position.second + 0.15) * _width].first == CRATE)
		return (true);
	return (false);
}

void 	is::ArtificialIntelligence::move(is::ArtificialIntelligence::Direction dir)
{
	if (dir == LEFT || dir == RIGHT) {
		if ((int)(_position.second + 0.15) != (int)(_position.second + _irrlicht.getNodeSize(_sptr) + 0.20)) {
			std::cout << "recalibrage vers le bas" << std::endl;
			_goal.second = (int)(_position.second + 1.15);
			moveDown();
		} else if (dir == LEFT) {
			_goal.first = (int)(_position.first - 0.85);
			moveLeft();
		} else {
			_goal.first = (int)(_position.first + 1.15);
			moveRight();
		}
	} else {
		if ((int)(_position.first + 0.15) != (int)(_position.first + _irrlicht.getNodeSize(_sptr) + 0.20)) {
			std::cout << "recalibrage vers la gauche" << std::endl;
			_goal.first = (int)(_position.first - 0.85);
			moveLeft();
		} else if (dir == DOWN) {
			_goal.second = (int)(_position.second + 1.15);;
			moveDown();
		} else {
			_goal.second = (int)(_position.second - 0.85);;
			moveUp();
		}
	}
}

void 	is::ArtificialIntelligence::headTowards(is::ArtificialIntelligence::Direction dir)
{
	std::cout << "X: " << _position.first << " Z: " << _position.second << std::endl;
	if (dir == LEFT || dir == RIGHT) {
		if (dir == LEFT) {
			_goal.first = (int)(_position.first - 0.85);
			move(LEFT);
		} else {
			_goal.first = (int)(_position.first + 1.15);
			move(RIGHT);
		}
	} else {
		if (dir == DOWN) {
			_goal.second = (int)(_position.second + 1.15);;
			move(DOWN);
		} else {
			_goal.second = (int)(_position.second - 0.85);;
			move(UP);
		}
	}
}

void 	is::ArtificialIntelligence::updateMap()
{
	for (std::size_t i = _width + 1 ; i < (_height - 1) * _width ; ++i) {
		_map[i].first = SAFE;
		_map[i].second = nullptr;
	}
	setWalls();
	for (const auto &it : _entities.get()) {
		if (!dynamic_cast<AEntity *>(it.get()))
			continue;
		if ((int)it->getX() + (int)it->getZ() * _width < _height * _width && _map[(int)it->getX() + (int)it->getZ() * _width].first != WALL && it->getType() != "Character") {
			if (it->getType() == "Wall")
				_map[(int)it->getX() + (int)it->getZ() * _width].first = CRATE;
			if (it->getType() == "Explosion")
				_map[(int)it->getX() + (int)it->getZ() * _width].first = WALL;
			if (it->getType() == "Bomb")
				_map[(int)it->getX() + (int)it->getZ() * _width].first = BOMB;
			if (it->getType() == "BombUp" || it->getType() == "FireUp" ||
				it->getType() == "SpeedUp" || it->getType() == "WallPass")
				_map[(int)it->getX() + (int)it->getZ() * _width].first = POWERUP;
			_map[(int)it->getX() + (int)it->getZ() * _width].second = it.get();
		}
	}
	addDangerZones();
}
