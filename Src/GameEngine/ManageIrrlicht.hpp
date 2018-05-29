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
# include "IManageIrrlicht.hpp"

namespace nts {

	class ManageIrrlicht : public nts::IManageIrrlicht {
		public:
		ManageIrrlicht(my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> &entities, my::ItemLocker<my::ThreadPool> &eventManager);
		~ManageIrrlicht() override = default;

		void updateView() override;
		void loopDisplay() override;

		bool addEntity(std::shared_ptr<is::IEntity> &, irr::IReferenceCounted &) override;
		irr::IReferenceCounted &getNode(std::shared_ptr<is::IEntity> &) override;

		irr::scene::ISceneManager *getSceneManager() const override;

		protected:

		private:
		void manageEvent();

		my::ItemLocker<my::ThreadPool> &_eventManager;
		my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> &_entities;

		std::unordered_map<std::shared_ptr<is::IEntity>, irr::IReferenceCounted> _listObj;
		std::vector<std::shared_ptr<is::IEntity>> _listPlayer;

		irr::IrrlichtDevice *_device = nullptr;
		irr::video::IVideoDriver *_driver = nullptr;
		irr::scene::ISceneManager *_sceneManager = nullptr;

//		MyEventReceiver _eventReceiver;
	};

}

#endif //TEST_MANAGEIRRLICHT_HPP
