//
// EPITECH PROJECT, 2018
// Indie Studio
// File description:
// MapGenerator class
//

#ifndef		MAP_GENERATOR_HPP_
#define		MAP_GENERATOR_HPP_

#include	<string>
#include	"IndieStudio.hpp"
#include	"ItemLocker.hpp"
#include	"IEntity.hpp"
#include	"ThreadPool.hpp"
#include	"ManageIrrlicht.hpp"

namespace mg {
	class MapGenerator {
	public:
		MapGenerator(is::Entity_t &entities, is::ThreadPool_t &eventManager,
			     nts::ManageIrrlicht &irrlicht,
			     std::pair<std::size_t, std::size_t> &size);
		~MapGenerator();

	private:
		void	setBackground();
		void	addWall(const std::size_t pos);
		void	setWalls();
		void	setUnreachableAreas(std::size_t y, std::size_t x);
		char	definePowerup();
		void	setCrates();
		void	addBorders();
		void	transposeMap(is::Entity_t &entities, is::ThreadPool_t &eventManager,
				     nts::ManageIrrlicht &irrlicht);

		void	displayMap();
		void	createMap(is::Entity_t &entities, is::ThreadPool_t &eventManager,
				  nts::ManageIrrlicht &irrlicht);

		std::size_t	_height;
		std::size_t	_width;
		std::string	_map;
	};
}

#endif		/* !MAP_GENERATOR_HPP_ */