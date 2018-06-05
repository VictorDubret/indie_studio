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
# include <mutex>
#include "Debug.hpp"

# include "ACharacter.hpp"
# include "EventManager.hpp"
# include "IManageIrrlicht.hpp"

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

	class ManageIrrlicht : public nts::IManageIrrlicht {
		public:
		ManageIrrlicht(my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> &entities, my::ItemLocker<my::ThreadPool> &eventManager, irr::core::vector2di mapSize, bool splitScreen);
		~ManageIrrlicht() override = default;

		void updateView() override;
		void loopDisplay() override;

		/* Map Size and Camera */
		void setMapSize(const irr::core::vector2di &mapSize) override;
		void setCameraPos() override;
		irr::core::vector2di getMapSize() const override;

		bool addEntity(std::shared_ptr<is::IEntity> &, irr::scene::ISceneNode *, float size = 1.f) override;
		bool deleteEntity(std::shared_ptr<is::IEntity> &) override;

		irr::scene::ISceneNode *getNode(const std::shared_ptr<is::IEntity> &) override;
		float &getNodeSize(const std::shared_ptr<is::IEntity> &) override;

		irr::scene::ISceneManager *getSceneManager() const override;
		irr::IrrlichtDevice *getDevice() const override;
		irr::video::IVideoDriver *getDriver() const override;
		irrklang::ISoundEngine *getSoundDevice() const override;

		void lock() override;
		void unlock() override;

		protected:

		private:
		void manageEvent();
		void manageEventPlayers();
		void displayFPS();
		void displaySplitScreen();

		my::ItemLocker<my::ThreadPool> &_eventManager;
		my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> &_entities;

		std::unordered_map<std::shared_ptr<is::IEntity>, nts::irrObj_t> _listObj;
		std::vector<nts::player_t> _listPlayer;
		std::vector<irr::core::vector2df> _distBetweenPlayer;

		irr::IrrlichtDevice *_device = nullptr;
		irr::video::IVideoDriver *_driver = nullptr;
		irr::scene::ISceneManager *_sceneManager = nullptr;
		irrklang::ISoundEngine *_engine = nullptr;

		irr::core::vector2di _mapSize;

		std::mutex _mutex;

		nts::EventManager _eventReceiver;
		irr::scene::ICameraSceneNode *_camera[4]={0,0,0,0};
		bool _splitScreen = false;
	};

}

#endif //TEST_MANAGEIRRLICHT_HPP
