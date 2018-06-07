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

	enum {
		MENU = 0,
		SETTINGS = 1
	};

	typedef struct hover_s {
		irr::gui::IGUIImage *base;
		std::string name;

		irr::io::path no_hover;
		irr::io::path hover;
		irr::core::rect<irr::s32> sq;

		std::function<void(const struct nts::hover_s &)> click;
		bool status;
		bool used = true;
	} hover_t;

	class GUI : public virtual AManageIrrlicht {
		public:
		GUI(my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> &entities, my::ItemLocker<my::ThreadPool> &eventManager);

		void manageEventGui();
		void drawGUI();
		void addButton(const wchar_t *text, const irr::core::rect<irr::s32> &textRect, const irr::io::path &trigger = "");
		void addButtonImage(const std::string &name, const irr::io::path &scene, const irr::io::path &hover, const irr::io::path &no_hover, const irr::core::rect<irr::s32> &sq, const std::function<void(const struct nts::hover_s &)> &f);

		protected:

		private:
		void initBaseScene();
		void initSettingsScene();

		irr::io::path _currentScene = "";

		irr::gui::IGUIEnvironment *_gui = nullptr;
		std::map<irr::io::path, std::map<std::string, hover_t>> _hoverManage;

	};
}

#endif //BOMBERMAN_GUI_HPP
