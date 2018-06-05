/*
** EPITECH PROJECT, 2018
** bomberman
** File description:
** Created by martin.januario@epitech.eu,
*/

#ifndef BOMBERMAN_GUI_HPP
# define BOMBERMAN_GUI_HPP

# include "AManageIrrlicht.hpp"

namespace nts {
	class GUI : public virtual AManageIrrlicht {
		public:
		GUI(my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> &entities, my::ItemLocker<my::ThreadPool> &eventManager);

		void manageEventGui();
		void drawGUI();

		protected:
		private:
	};
}

#endif //BOMBERMAN_GUI_HPP
