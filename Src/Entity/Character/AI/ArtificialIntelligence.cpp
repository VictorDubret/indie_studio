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

void 	is::ArtificialIntelligence::AIsTurn()
{
	Direction	dir;

	//if (!dynamic_cast<ACharacter *>(_sptr.get()))
		//return;
	if (stillCrates() == true) {
		_entities.lock();
		updateMap();
		_entities.unlock();
		std::cout << "I'm in ";
		std::cout << getX() << " " << getZ();
		if (inDanger() == true) {
			std::cout << " and I don't feel safe." << std::endl;
			dir = lookForAZone(SAFE);
			headTowards(dir);
			return;
		}
		dir = lookForAZone(POWERUP);
		if (dir != NONE) {
			std::cout << " and I head towards a power-up." << std::endl;
			headTowards(dir);
			return;
		}
		if (safeBombDrop() == true) {
			if (onCrate() == false) {
				std::cout << " and I can drop a bomb." << std::endl;
				dropBomb();
			} else {
				std::cout << " and I can't drop a bomb." << std::endl;
				dir = lookForAZone(SAFE);
				headTowards(dir);
			}
		} else {
			std::cout << " and I'm looking for a crate." << std::endl;
			dir = lookForAZone(CRATE);
			headTowards(dir);
		}
	}
}

bool 	is::ArtificialIntelligence::stillCrates()
{
	for (std::size_t i = _width + 1 ; i < _width * _height - _width ; ++i) {
		if (_map[i].first == CRATE) {
			return (true);
		}
	}
	return (false);
}

bool is::ArtificialIntelligence::inDanger()
{
	float size = _irrlicht.getNodeSize(_sptr);
	if (_map[(int)getZ() * _width + (int)getX()].first == BOMB || _map[(int)getZ() * _width + (int)getX()].first == DANGER)
		return (true);
	if (_map[(int)(getZ() + size) * _width + (int)(getX() + size)].first == BOMB || _map[(int)(getZ() + size) * _width + (int)(getX() + size)].first == DANGER)
		return (true);
	return (false);
}

void	is::ArtificialIntelligence::getMapDimensions()
{
	//_entities.lock();
	for (const auto &it : _entities.get()) {
		if (it->getX() > _width)
			_width = static_cast<size_t>(it->getX());
		if (it->getZ() > _height)
			_height = static_cast<size_t>(it->getZ());
	}
	//_entities.unlock();
	_height++;
	_width++;
	#ifdef DEBUG
	std::cout << "map height: " << _height << std::endl;
	std::cout << "map width: " << _width << std::endl;
	#endif
}

void	is::ArtificialIntelligence::setWalls()
{
	//_entities.lock();
	for (const auto &it : _entities.get()) {
		if (it->getType() == "UnbreakableWall") {
			_map[(int)it->getX() + (int)it->getZ() * _width].first = WALL;
			_map[(int)it->getX() + (int)it->getZ() * _width].second = it.get();
		}
	}
	//_entities.unlock();
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

void 	is::ArtificialIntelligence::setDangerRec(std::size_t pos, std::size_t range, int dir)
{
	if (_map[pos + dir].first != WALL && range > 0) {
		if (_map[pos + dir].first != CRATE)
			setDangerRec(pos + dir, range - 1, dir);
		if (_map[pos + dir].first != EXPLOSION)
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
		else
			reachCrate = true;
		if (map[pos + dir].first != EXPLOSION)
			map[pos + dir].first = DANGER;
		map[pos - 1].second = nullptr;
		map[pos - 1].second = nullptr;
	}
	if (reachCrate)
		std::cout << "A crate can be reached." << std::endl;
	else
		std::cout << "No crate can be reached." << std::endl;
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
					setDangerRec(i, tmp_bomb->getLenExplosion(), dir[j]);
				std::cout << "bomb range " << tmp_bomb->getLenExplosion() << std::endl;
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
					setDangerRec(i, tmp_bomb->getLenExplosion(), dir[j], map);
				std::cout << "bomb range " << tmp_bomb->getLenExplosion() << std::endl;
			} else {
				for (std::size_t j = 0; j < 4; ++j)
					reachCrate = setDangerRec(i, getBombLength(), dir[j], map) ? true : reachCrate;
				std::cout << "supposed bomb range " << getBombLength() << std::endl;
			}
		}
	}
	if (reachCrate)
		std::cout << "Glob: A crate can be reached." << std::endl;
	else
		std::cout << "Glob: No crate can be reached." << std::endl;
	return reachCrate;
}

int 	is::ArtificialIntelligence::getDist(int pos, std::vector<int> map)
{
	if (map[pos] == -3 || (map[pos] == -2 && _map[(int)(getZ() + 0.15) * _width + (int)(getX() + 0.15)].first != DANGER && _map[(int)(getZ() + 0.15) * _width + (int)(getX() + 0.15)].first != BOMB))
		return (100); //collision
	if (map[pos] == -1)
		return (0);

	map[pos] = -3;
	int	left = getDist(pos - 1, map);
	int	right = getDist(pos + 1, map);
	int	up = getDist(static_cast<int>(pos + _width), map);
	int	down = getDist(static_cast<int>(pos - _width), map);
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
	int	left = getDist(pos - 1, map);
	int	right = getDist(pos + 1, map);
	int	up = getDist(static_cast<int>(pos - _width), map);
	int	down = getDist(static_cast<int>(pos + _width), map);

	std::cout << "left distance = " << left << std::endl;
	std::cout << "right distance = " << right << std::endl;
	std::cout << "up distance = " << up << std::endl;
	std::cout << "down distance = " << down << std::endl;
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
	int 			start = static_cast<int>(
		//static_cast<int>(getZ()) * _width + static_cast<int>(getX()));
		static_cast<int>(getZ() + 0.15) * _width + static_cast<int>(getX() + 0.15)); //collision

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
	int 			start = static_cast<int>(
		//static_cast<int>(getZ()) * _width + static_cast<int>(getX()));
		static_cast<int>(getZ() + 0.15) * _width + static_cast<int>(getX() + 0.15)); //collision

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

	map[(int)getX() + (int)getZ() * _width].first = BOMB;
	map[(int)getX() + (int)getZ() * _width].second = nullptr;
	if (addDangerZones(map) == false) {
		std::cout << "No use in dropping a bomb." << std::endl;
		return (false);
	}
	if (lookForAZone(SAFE, map) == NONE) {
		std::cout << "Too dangerous to drop a bomb." << std::endl;
		return (false);
	}
	return (true);
}

bool 	is::ArtificialIntelligence::onCrate()
{
	if (_map[(int)getX() + (int)getZ() * _width].first == BOMB ||
		_map[(int)getX() + (int)getZ() * _width].first == CRATE)
		return (true);
	return (false);
}

void 	is::ArtificialIntelligence::headTowards(is::ArtificialIntelligence::Direction dir)
{
	std::cout << "Z: I'm between " << getZ() << " and " << getZ() + _irrlicht.getNodeSize(_sptr) << std::endl;
	std::cout << "X: I'm between " << getX() << " and " << getX() + _irrlicht.getNodeSize(_sptr) << std::endl;
	if (dir == LEFT || dir == RIGHT) {
		//if ((int)(getZ()) != (int)(getZ() + _irrlicht.getNodeSize(_sptr))) {
		if ((int)(getZ() + 0.15) != (int)(getZ() + _irrlicht.getNodeSize(_sptr) + 0.15)) { //collision
			std::cout << "I'm going down" << std::endl;
			moveDown();
		} else if (dir == LEFT) {
			std::cout << "I'm going left" << std::endl;
			moveLeft();
		} else {
			std::cout << "I'm going right" << std::endl;
			moveRight();
		}
	} else {
		//if ((int)(getX()) != (int)(getX() + _irrlicht.getNodeSize(_sptr))) {
		if ((int)(getX() + 0.15) != (int)(getX() + _irrlicht.getNodeSize(_sptr) + 0.15)) { //collision
			std::cout << "I'm going left" << std::endl;
			moveLeft();
		} else if (dir == DOWN) {
			std::cout << "I'm going down" << std::endl;
			moveDown();
		} else {
			std::cout << "I'm going up" << std::endl;
			moveUp();
		}
	}
}

void 	is::ArtificialIntelligence::updateMap()
{
	for (std::size_t i = _width + 1 ; i < (_height - 1) * _width ; ++i) {
		if (_map[i].first != WALL) {
			_map[i].first = SAFE;
			_map[i].second = nullptr;
		}
	}
	//_entities.lock();
	for (const auto &it : _entities.get()) {
		if (!dynamic_cast<AEntity *>(it.get()))
			continue;
		//it->lock();
		if ((int)it->getX() + (int)it->getZ() * _width < _height * _width && _map[(int)it->getX() + (int)it->getZ() * _width].first != WALL && it->getType() != "Character") {
			if (it->getType() == "Wall")
				_map[(int)it->getX() + (int)it->getZ() * _width].first = CRATE;
			if (it->getType() == "Explosion")
				_map[(int)it->getX() + (int)it->getZ() * _width].first = EXPLOSION;
			if (it->getType() == "Bomb")
				_map[(int)it->getX() + (int)it->getZ() * _width].first = BOMB;
			if (it->getType() == "BombUp" || it->getType() == "FireUp" ||
				it->getType() == "SpeedUp" || it->getType() == "WallPass")
				_map[(int)it->getX() + (int)it->getZ() * _width].first = POWERUP;
			_map[(int)it->getX() + (int)it->getZ() * _width].second = it.get();
		}
		//it->unlock();
	}
	//_entities.unlock();
	addDangerZones();
	#ifdef DEBUG
	for (std::size_t i = 0 ; i < _width * _height ; ++i) {
		if (_map[i].first == SAFE)
			std::cout << "  ";
		else if (_map[i].first == WALL)
			std::cout << "##";
		else if (_map[i].first == CRATE)
			std::cout << "00";
		else if (_map[i].first == BOMB)
			std::cout << "BB";
		else if (_map[i].first == EXPLOSION)
			std::cout << "EE";
		else if (_map[i].first == DANGER)
			std::cout << "XX";
		else if (_map[i].first == POWERUP)
			std::cout << "!!";
		if (i % _width == _width - 1)
			std::cout << std::endl;
	}
	#endif
}
