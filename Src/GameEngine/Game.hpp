/*
** EPITECH PROJECT, 2018
** bomberman
** File description:
** Created by martin.januario@epitech.eu,
*/

#ifndef BOMBERMAN_GAME_HPP
# define BOMBERMAN_GAME_HPP

# include <unordered_map>
# include "AManageIrrlicht.hpp"

namespace irrl {

	typedef struct {
		irr::EKEY_CODE key;
		std::function<void (void)> f;
	} event_t;

	typedef struct {
		is::ACharacter *entity;
		event_t nothing;
		event_t key[6];

		bool alive;
	} player_t;

	typedef struct {
		irr::scene::ISceneNode *obj;
		float size;
	} irrObj_t;

	enum {
		NEAREST = 0,
		FAREST = 1
	};

	enum {
		PLAYER1 = 0,
		PLAYER2 = 1,
		GLOBAL = 3
	};

	enum {
		BOMB = 0,
		FLAME = 1,
		SPEED = 2,
		WALLPASS = 3
	};
	class Game : public virtual AManageIrrlicht {
		public:
		Game(my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> &entities, my::ItemLocker<my::ThreadPool> &eventManager, irrl::ManageIrrlicht &irrlicht,  irr::core::vector2di mapSize, bool splitScreen);
		~Game() override;

		void updateView() override;

		/* Map Size and Camera */
		void setMapSize(const irr::core::vector2di &mapSize) override;
		void setCameraPos() override;
		irr::core::vector2di getMapSize() const override;

		bool addEntity(std::shared_ptr<is::IEntity> &, irr::scene::ISceneNode *, float size = 1.f) override;
		bool deleteEntity(std::shared_ptr<is::IEntity> &) override;
		void resetListObj() override;

		irr::scene::ISceneNode *getNode(is::IEntity *) override;
		float &getNodeSize(const std::shared_ptr<is::IEntity> &) override;

		void lock() override;
		void unlock() override;
		void setPause();
		void setFloor();
		void setBG();
		void endPause();
		void checkLastAlive();

		protected:

		void manageEventPlayers();
		void displayFPS();
		void displaySplitScreen();
		void displayGlobalScene();
		void displaySplitScreenScene();
		void endSplitScene();
		void displayBothPlayers();
		void displayBothPlayersEnd(irr::video::ITexture *);
		void endScene();
		void displayScore();
		void displayBonus(is::ACharacter *hudPos);
		unsigned long long gameLengh() const;


		std::map<is::IEntity *, irrl::irrObj_t> _listObj;
		std::vector<irrl::player_t> _listPlayer;
		std::vector<irr::core::vector2df> _distBetweenPlayer;

		my::Thread *_thread = nullptr;
		bool _stopThread = false;
		irr::scene::ICameraSceneNode *_camera[4]={0,0,0,0};
		bool _endGame = false;
		bool _draw = false;
		bool _winPLayer = true;

		irr::video::ITexture *_winPicture;
		irr::video::ITexture *_drawPicture;
		irr::core::position2d<irr::s32> _winPosPicture;
		irr::core::rect<irr::s32> _winRectangle;
		size_t _bestScore = 0;
		std::chrono::time_point<std::chrono::system_clock> _startTime;
		irr::core::stringc _gameLengh;
		std::map<size_t, irr::core::stringw> _scoreString;
		std::map<size_t, irr::video::SColor> _color;
		std::map<size_t, irr::core::position2d<irr::s32>> _hudPos;
		std::map<char, irr::video::ITexture *> _bonus;
	};

}

#endif //BOMBERMAN_GAME_HPP
