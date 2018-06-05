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
	enum {
		HOVER_PLAYNOW,
		HOVER_SETTINGS,
		CLICK_SETTINGS,
		CLICK_PLAYNOW
	};

	class GUI : public virtual AManageIrrlicht {
		public:
		GUI(my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> &entities, my::ItemLocker<my::ThreadPool> &eventManager);

		void manageEventGui();
		void drawGUI();
		void setScene(int);

		protected:
		private:
		int _currentScene = 0;

		std::map<int, irr::gui::IGUIEnvironment *> _scene;
	};
}

#endif //BOMBERMAN_GUI_HPP
