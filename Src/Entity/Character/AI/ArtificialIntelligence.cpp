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
	if (stillCrates() == true) {
		//dropBomb();
		updateMap();
		if (inDanger() == true) {
			std::cout << "I'm in ";
			std::cout << getX() << " " << getZ();
			std::cout << " and I don't feel safe." << std::endl;
			dir = lookForAZone(SAFE);
			headTowards(dir);
		} else {
			dir = lookForAZone(POWERUP);
			if (dir != NONE)
				headTowards(dir);
			else {
				std::cout << "I'm in ";
				std::cout << getX() << " " << getZ();
				std::cout << ", I'm safe and there are no powerups I can reach."
					<< std::endl;
			}
		}
		/*} else if (lookForAZone(POWERUP) == NONE) {
			//go for crates
		}*/
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
	_entities.lock();
	for (const auto &it : _entities.get()) {
		if (it->getX() > _width)
			_width = static_cast<size_t>(it->getX());
		if (it->getZ() > _height)
			_height = static_cast<size_t>(it->getZ());
	}
	_entities.unlock();
	_height++;
	_width++;
	#ifdef DEBUG
	std::cout << "map height: " << _height << std::endl;
	std::cout << "map width: " << _width << std::endl;
	#endif
}

void	is::ArtificialIntelligence::setWalls()
{
	_entities.lock();
	for (const auto &it : _entities.get()) {
		if (it->getType() == "UnbreakableWall") {
			_map[(int)it->getX() + (int)it->getZ() * _width].first = WALL;
			_map[(int)it->getX() + (int)it->getZ() * _width].second = it.get();
		}
	}
	_entities.unlock();
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

void 	is::ArtificialIntelligence::setDanger(std::size_t pos, std::size_t range)
{
	int	dir[4] = { -1, 1, static_cast<int>(_width), static_cast<int>(_width * -1)};

	for (std::size_t i = 0 ; i < 4 ; ++i) {
		if (_map[pos + dir[i]].first != WALL && range > 0) {
			if (_map[pos + dir[i]].first != CRATE)
				setDangerRec(pos, range - 1, dir[i]);
			if (_map[pos + dir[i]].first != EXPLOSION)
				_map[pos + dir[i]].first = DANGER;
			_map[pos + dir[i]].second = nullptr;
		}
	}
}

void 	is::ArtificialIntelligence::addDangerZones()
{
	for (std::size_t i = _width + 1 ; i < _width * _height - _width ; ++i) {
		if (_map[i].first == BOMB) {
			setDanger(i, dynamic_cast<is::Bomb *>(_map[i].second)->getLenExplosion());
			std::cout << "bomb range " << dynamic_cast<is::Bomb *>(_map[i].second)->getLenExplosion() << std::endl;
		}
	}
}

int 	is::ArtificialIntelligence::getDist(int pos, std::vector<int> map)
{
	if (map[pos] == -3 || (map[pos] == -2 && _map[(int)getZ() * _width + (int)getX()].first != DANGER && _map[(int)getZ() * _width + (int)getX()].first != BOMB))
		return (100);
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
		static_cast<int>(getZ() + 0.15) * _width + static_cast<int>(getX() + 0.15));

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

void 	is::ArtificialIntelligence::headTowards(is::ArtificialIntelligence::Direction dir)
{
	std::cout << "Z: I'm between " << getZ() << " and " << getZ() + _irrlicht.getNodeSize(_sptr) << std::endl;
	std::cout << "X: I'm between " << getX() << " and " << getX() + _irrlicht.getNodeSize(_sptr) << std::endl;
	if (dir == LEFT || dir == RIGHT) {
		if ((int)(getZ() + 0.15) != (int)(getZ() + _irrlicht.getNodeSize(_sptr) + 0.15)) {
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
		if ((int)(getX() + 0.15) != (int)(getX() + _irrlicht.getNodeSize(_sptr) + 0.15)) {
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
	_entities.lock();
	for (const auto &it : _entities.get()) {
		if (_map[(int)it->getX() + (int)it->getZ() * _width].first != WALL && it->getType() != "Character") {
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
	}
	_entities.unlock();
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
