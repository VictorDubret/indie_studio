/*
** EPITECH PROJECT, 2018
** cpp_indie_studio
** File description:
** Created by martin.januario@epitech.eu,
*/

#ifndef CPP_INDIE_STUDIO_AMANAGEIRRLICHT_HPP
# define CPP_INDIE_STUDIO_AMANAGEIRRLICHT_HPP

# include <irrKlang.h>
# include "IManageIrrlicht.hpp"
# include "ACharacter.hpp"
# include "EventManager.hpp"

namespace nts {

	class AManageIrrlicht : public IManageIrrlicht {
		public:
		AManageIrrlicht(my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> &entities, my::ItemLocker<my::ThreadPool> &eventManager);
		virtual ~AManageIrrlicht() = default;

		irr::scene::ISceneManager *getSceneManager() const override;
		irr::IrrlichtDevice *getDevice() const override;
		irr::video::IVideoDriver *getDriver() const override;
		irrklang::ISoundEngine *getSoundDevice() const override;

		void updateView() override;
		void loopDisplay() override;

		bool addEntity(std::shared_ptr<is::IEntity> &, irr::scene::ISceneNode *, float) override;
		bool deleteEntity(std::shared_ptr<is::IEntity> &) override;

		irr::scene::ISceneNode *getNode(const std::shared_ptr<is::IEntity> &) override;
		float &getNodeSize(const std::shared_ptr<is::IEntity> &) override;

		irr::core::vector2di getMapSize() const override;
		void setMapSize(const irr::core::vector2di &mapSize) override;
		void setCameraPos() override;


		protected:
		my::ItemLocker<my::ThreadPool> &_eventManager;
		my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> &_entities;

		irr::IrrlichtDevice *_device = nullptr;
		irr::video::IVideoDriver *_driver = nullptr;
		irr::scene::ISceneManager *_sceneManager = nullptr;
		irrklang::ISoundEngine *_engine = nullptr;
		nts::EventManager _eventReceiver;

		private:
		float _tmp = 0;
	};

}

#endif //CPP_INDIE_STUDIO_AMANAGEIRRLICHT_HPP
