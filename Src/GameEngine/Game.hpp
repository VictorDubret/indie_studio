/*
** EPITECH PROJECT, 2018
** bomberman
** File description:
** Created by martin.januario@epitech.eu,
*/

#ifndef BOMBERMAN_GAME_HPP
# define BOMBERMAN_GAME_HPP

#include <unordered_map>
# include "AManageIrrlicht.hpp"

namespace nts {

	typedef struct {
		irr::EKEY_CODE key;
		std::function<void (void)> f;
	} event_t;

	typedef struct {
		std::shared_ptr<is::ACharacter> entity;
		event_t nothing;
		event_t doSomething;
		event_t key[5];
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

	class Game : public virtual AManageIrrlicht {
		public:
		Game(my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> &entities, my::ItemLocker<my::ThreadPool> &eventManager, irr::core::vector2di mapSize, bool splitScreen);

		void updateView() override;

		/* Map Size and Camera */
		void setMapSize(const irr::core::vector2di &mapSize) override;
		void setCameraPos() override;
		irr::core::vector2di getMapSize() const override;

		bool addEntity(std::shared_ptr<is::IEntity> &, irr::scene::ISceneNode *, float size = 1.f) override;
		bool deleteEntity(std::shared_ptr<is::IEntity> &) override;

		irr::scene::ISceneNode *getNode(const std::shared_ptr<is::IEntity> &) override;
		float &getNodeSize(const std::shared_ptr<is::IEntity> &) override;

		void resetListObj() override;

		protected:
		std::unordered_map<std::shared_ptr<is::IEntity>, nts::irrObj_t> _listObj;
		std::vector<nts::player_t> _listPlayer;

		std::vector<irr::core::vector2df> _distBetweenPlayer;


		irr::core::vector2di _mapSize;

		irr::scene::ICameraSceneNode *_camera[4]={0,0,0,0};
		bool _splitScreen = false;

		void manageEventPlayers();
		void displayFPS();
		void displaySplitScreen();
		private:
	};
}

#endif //BOMBERMAN_GAME_HPP
