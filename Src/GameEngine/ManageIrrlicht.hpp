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
		event_t key[6];
	} player_t;

	typedef struct {
		irr::scene::ISceneNode *obj;
		float size;
	} irrObj_t;

	class ManageIrrlicht : public nts::IManageIrrlicht {
		public:
		ManageIrrlicht(my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> &entities, my::ItemLocker<my::ThreadPool> &eventManager, irr::core::vector2di mapSize);
		~ManageIrrlicht() override = default;

		void updateView() override;
		void loopDisplay(const std::shared_ptr<is::IEntity> &entity) override;

		void setMapSize(const irr::core::vector2di &mapSize) override;
		irr::core::vector2di getMapSize() const override;



		bool addEntity(std::shared_ptr<is::IEntity> &, irr::scene::ISceneNode *, float size = 1.f) override;
		irr::scene::ISceneNode *getNode(const std::shared_ptr<is::IEntity> &) override;
		float &getNodeSize(const std::shared_ptr<is::IEntity> &) override;

		irr::scene::ISceneManager *getSceneManager() const override;
		irr::IrrlichtDevice *getDevice() const override;
		irr::video::IVideoDriver *getDriver() const override;

		protected:

		private:
		void manageEvent();
		void manageEventPlayers();

		my::ItemLocker<my::ThreadPool> &_eventManager;
		my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> &_entities;

		std::unordered_map<std::shared_ptr<is::IEntity>, nts::irrObj_t> _listObj;
		std::vector<nts::player_t> _listPlayer;

		irr::IrrlichtDevice *_device = nullptr;
		irr::video::IVideoDriver *_driver = nullptr;
		irr::scene::ISceneManager *_sceneManager = nullptr;

		irr::core::vector2di _mapSize;

		nts::EventManager _eventReceiver;
	};

}

#endif //TEST_MANAGEIRRLICHT_HPP
