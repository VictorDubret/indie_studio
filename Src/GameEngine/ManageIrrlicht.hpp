/*
** EPITECH PROJECT, 2018
** test
** File description:
** Created by martin.januario@epitech.eu,
*/

#ifndef TEST_MANAGEIRRLICHT_HPP
# define TEST_MANAGEIRRLICHT_HPP

# include <unordered_map>
# include <vector>
# include "Game.hpp"
# include "GUI.hpp"

namespace nts {

	class ManageIrrlicht : public Game, public GUI {
		public:
		ManageIrrlicht(my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> &entities, my::ItemLocker<my::ThreadPool> &eventManager, irr::core::vector2di mapSize, bool splitScreen);

		void loopDisplay() override;
		void displayGui(bool display);

		protected:

		private:
		void manageEvent();

		bool _displayGUI = true;
		};

}

#endif //TEST_MANAGEIRRLICHT_HPP
