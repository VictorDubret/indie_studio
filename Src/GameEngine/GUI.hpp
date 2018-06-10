/*
** EPITECH PROJECT, 2018
** bomberman
** File description:
** Created by martin.januario@epitech.eu,
*/

#ifndef BOMBERMAN_GUI_HPP
# define BOMBERMAN_GUI_HPP

# include "AManageIrrlicht.hpp"

namespace irrl {
	enum {
		MAPSIZE_X,
		MAPSIZE_Y,
		CRATE_RATE,
		DROP_RATE,
		BOMBEUP_RATE,
		FIREUP_RATE,
		SPEEDUP_RATE,
		WALLPASS_RATE
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

		std::function<void(const struct irrl::hover_s &)> click;
		bool status;
		bool used = true;
	} hover_t;

	class GUI : public virtual AManageIrrlicht {
		public:
		GUI(my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> &entities, my::ItemLocker<my::ThreadPool> &eventManager, irrl::ManageIrrlicht &irrlicht);

		void manageEventGui();
		void drawGUI();
		void initWinner();
		void initDraw();
		void addButton(const wchar_t *text, const irr::core::rect<irr::s32> &textRect, const irr::io::path &trigger = "");
		void addButtonImage(const std::string &name, const irr::io::path &scene, const irr::io::path &hover, const irr::io::path &no_hover, const irr::core::rect<irr::s32> &sq, const std::function<void(const struct irrl::hover_s &)> &f);
		void setEntity(const std::vector<std::string> &tmpVector, const std::shared_ptr<is::IEntity> &player_tmp2, irrl::ManageIrrlicht &tmp);
		void initBaseScene();
		void initPause();
		irr::io::path &getCurrentScene();

		protected:
		void addPlayerAndIA();
		void addPlayer(float x, float z, std::size_t id);
		void addIA(float x, float z, std::size_t id);

		irr::gui::IGUIEnvironment *_gui = nullptr;

		private:
		void initSettingsScene();
		void updateRateSettings();
		int getValueInput(irr::gui::IGUIEditBox *obj, int min, int max);
		void loadSave();
		void loadEntity(is::ACharacter *player2, int &j, std::vector<std::string> &tmpVector);

		std::map<irr::io::path, std::map<std::string, hover_t>> _hoverManage;

		// rate et map size settings
		irr::gui::IGUIEditBox *_mapXEditBox = nullptr;
		irr::gui::IGUIEditBox *_mapYEditBox = nullptr;
		irr::gui::IGUIEditBox *_crateEditBox = nullptr;
		irr::gui::IGUIEditBox *_dropEditBox = nullptr;

		irr::gui::IGUIEditBox *_bombEditBox = nullptr;
		irr::gui::IGUIEditBox *_fireEditBox = nullptr;
		irr::gui::IGUIEditBox *_speedEditBox = nullptr;
		irr::gui::IGUIEditBox *_wallpassEditBox = nullptr;

		bool _soundMute = true;
	};
}

#endif //BOMBERMAN_GUI_HPP
