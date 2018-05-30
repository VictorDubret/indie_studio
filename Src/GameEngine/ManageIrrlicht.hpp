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
		event_t key[6];
	} player_t;

	class ManageIrrlicht : public nts::IManageIrrlicht {
		public:
		ManageIrrlicht(my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> &entities, my::ItemLocker<my::ThreadPool> &eventManager);
		~ManageIrrlicht() override = default;

		void updateView() override;
		void loopDisplay() override;

		bool addEntity(std::shared_ptr<is::IEntity> &, irr::scene::ISceneNode *) override;
		irr::scene::ISceneNode *getNode(std::shared_ptr<is::IEntity> &) override;

		irr::scene::ISceneManager *getSceneManager() const override;
		irr::IrrlichtDevice *getDevice() const override;
		irr::video::IVideoDriver *getDriver() const override;

		protected:

		private:
		void manageEvent();
		void manageEventPlayers();

		my::ItemLocker<my::ThreadPool> &_eventManager;
		my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> &_entities;

		std::unordered_map<std::shared_ptr<is::IEntity>, irr::scene::ISceneNode *> _listObj;
		std::vector<nts::player_t> _listPlayer;

		irr::IrrlichtDevice *_device = nullptr;
		irr::video::IVideoDriver *_driver = nullptr;
		irr::scene::ISceneManager *_sceneManager = nullptr;

		nts::EventManager _eventReceiver;
	};

}

#endif //TEST_MANAGEIRRLICHT_HPP
