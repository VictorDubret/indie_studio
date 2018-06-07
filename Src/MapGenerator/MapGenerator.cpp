//
// EPITECH PROJECT, 2018
// Indie Studio
// File description:
// MapGenerator class
//

#include	<iostream>
#include	<ctime>
#include	<vector>
#include	"Debug.hpp"
#include	"Wall.hpp"
#include	"UnbreakableWall.hpp"
#include	"MapGenerator.hpp"

mg::MapGenerator::MapGenerator(is::Entity_t &entities, is::ThreadPool_t &eventManager,
			       nts::ManageIrrlicht &irrlicht,
			       std::pair<std::size_t, std::size_t> &size) :
	_height(size.first), _width(size.second), _dropRate(65), _bombUpRate(50),
	_fireUpRate(29), _speedUpRate(19), _wallPassRate(2)
{
	if (_height % 2 == 0) {
		size.first++;
		_height++;
	}
	if (_width % 2 == 0) {
		size.second++;
		_width++;
	}
	_map = std::string(_height * _width, '.');
	createMap(entities, eventManager, irrlicht);
}

mg::MapGenerator::MapGenerator(is::Entity_t &entities, is::ThreadPool_t &eventManager,
	nts::ManageIrrlicht &irrlicht,
	std::pair<std::size_t, std::size_t> &size, int dropRate, int bombUpRate,
	int fireUpRate, int speedUpRate, int wallPassRate) :
	_height(size.first), _width(size.second), _dropRate(100 - dropRate), _bombUpRate(bombUpRate),
	_fireUpRate(fireUpRate), _speedUpRate(speedUpRate), _wallPassRate(wallPassRate)
{
	if (_height % 2 == 0) {
		size.first++;
		_height++;
	}
	if (_width % 2 == 0) {
		size.second++;
		_width++;
	}
	_map = std::string(_height * _width, '.');
	createMap(entities, eventManager, irrlicht);
}

mg::MapGenerator::~MapGenerator()
{
}

void		mg::MapGenerator::setBackground()
{
	for (std::size_t i = 0 ; i < _height ; ++i) {
		for (std::size_t j = 0 ; j < _width ; ++j) {
			if (i % 2 == 1 && j % 2 == 1)
				_map[i * _width + j] = '#';
		}
	}
}

void		mg::MapGenerator::addWall(const std::size_t pos)
{
	std::size_t	posNb = -1;

	for (std::size_t i = 1 ; i < (_height + 1) / 2 ; ++i) {
		for (std::size_t j = 1 ; j < (_width + 1) / 2 ; ++j) {
			if (posNb == pos && j > 0 && (i % 2 == 0 || j % 2 == 0)) {
				_map[i * _width + j] = '#';
				_map[(_height - i - 1) * _width + j] = '#';
				_map[i * _width + (_width - j - 1)] = '#';
				_map[(_height - i - 1) * _width + (_width - j - 1)] = '#';
				return;
			}
			if (j < (_width - 1) / 2 &&
			    ((i % 2 == 1 && j % 2 == 1) ||
			     (i % 2 == 0 && j % 2 == 0)))
				posNb++;
		}
	}
}

void		mg::MapGenerator::setWalls()
{
	std::size_t	quarterSize = ((_height - 1) / 2) * ((_width - 1) / 2);

	for (std::size_t i = 0 ; i < quarterSize / 10 ; ++i)
		addWall(std::rand() % (quarterSize / 2));
}

void		mg::MapGenerator::setUnreachableAreas(std::size_t y, std::size_t x)
{
	_map[y * _width + x] = ' ';
	if (y > 0 && _map[(y - 1) * _width + x] == '.')
		setUnreachableAreas(y - 1, x);
	if (y < _height - 1 && _map[(y + 1) * _width + x] == '.')
		setUnreachableAreas(y + 1, x);
	if (x > 0 && _map[y * _width + (x - 1)] == '.')
		setUnreachableAreas(y, x - 1);
	if (x < _width - 1 && _map[y * _width + (x + 1)] == '.')
		setUnreachableAreas(y, x + 1);
}

char		mg::MapGenerator::definePowerup()
{
	int	type = rand();

	if (rand() % 100 < _dropRate)
		return '0';
	if (type % 100 < _bombUpRate)
		return 'b';
	if (type % 100 < _bombUpRate + _fireUpRate)
		return 'f';
	if (type % 100 < _bombUpRate + _fireUpRate + _speedUpRate)
		return 's';
	return 'w';
}

void		mg::MapGenerator::setCrates()
{
	for (std::size_t i = 0 ; i < _height ; ++i) {
		for (std::size_t j = 0 ; j < _width ; ++j) {
			if (std::rand() % 5  != 0 && _map[i * _width + j] == ' ' &&
			    ((i > 2 && i + 3 < _height) || (j > 2 && j + 3 < _width)))
				_map[i * _width + j] = definePowerup();
		}
	}
}

void		mg::MapGenerator::addBorders()
{
	_map.insert(_map.end(), _width + 1, '#');
	for (std::size_t i = _height * _width ; i > 0 ; --i) {
		if (i % _width == 0)
			_map.insert(i, "##");
	}
	_map.insert(_map.begin(), _width + 3, '#');
}

void		mg::MapGenerator::transposeMap(is::Entity_t &entities,
					       is::ThreadPool_t &eventManager,
					       nts::ManageIrrlicht &irrlicht)
{
	for (std::size_t i = 0 ; i < _map.size() ; ++i) {
		if (_map[i] == '#' || _map[i] == '.') {
			entities.lock();
			auto wall = new is::UnbreakableWall(entities, eventManager, irrlicht);
			wall->setZ(i / (_width + 2));
			wall->setX(i % (_width + 2));
			entities.unlock();
		} else if (_map[i] != ' ') {
			entities.lock();
			auto crate = new is::Wall(entities, eventManager, irrlicht);
			crate->setPowerUp(_map[i]);
			crate->setZ(i / (_width + 2));
			crate->setX(i % (_width + 2));
			entities.unlock();
		}
	}
}

void		mg::MapGenerator::displayMap()
{
	for (std::size_t i = 0 ; i < _height + 2 ; ++i) {
		for (std::size_t j = 0 ; j < _width + 2 ; ++j) {
			std::cout << _map[i * (_width + 2) + j];
			std::cout << _map[i * (_width + 2) + j];
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	for (std::size_t i = 0 ; i < _height + 2 ; ++i) {
		for (std::size_t j = 0 ; j < _width + 2 ; ++j) {
			if (_map[i * (_width + 2) + j] == '#' || _map[i * (_width + 2) + j] == '.') {
				std::cout << "##";
			} else {
				std::cout << "  ";
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void		mg::MapGenerator::createMap(is::Entity_t &entities, is::ThreadPool_t &eventManager,
					    nts::ManageIrrlicht &irrlicht)
{
	std::srand(std::time(nullptr));
	setBackground();
	setWalls();
	setUnreachableAreas(0, 0);
	setCrates();
	addBorders();
	#ifdef DEBUG
	displayMap();
	#endif
	transposeMap(entities, eventManager, irrlicht);
}
