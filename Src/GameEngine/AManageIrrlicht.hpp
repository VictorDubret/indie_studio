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

namespace irrl {

	class ManageIrrlicht;

	class AManageIrrlicht : public IManageIrrlicht {
		public:
		AManageIrrlicht(my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> &entities, my::ItemLocker<my::ThreadPool> &eventManager, irrl::ManageIrrlicht &irrlicht);
		virtual ~AManageIrrlicht() = default;

		irr::scene::ISceneManager *getSceneManager() const override;
		irr::IrrlichtDevice *getDevice() const override;
		irr::video::IVideoDriver *getDriver() const override;
		irrklang::ISoundEngine *getSoundDevice() const override;
		bool getDisplayGUI() const override;

		void updateView() override;
		void loopDisplay() override;

		bool addEntity(std::shared_ptr<is::IEntity> &, irr::scene::ISceneNode *, float) override;
		bool deleteEntity(std::shared_ptr<is::IEntity> &) override;
		void resetListObj() override;

		irr::scene::ISceneNode *getNode(is::IEntity *) override;
		float &getNodeSize(const std::shared_ptr<is::IEntity> &) override;

		irr::core::vector2di getMapSize() const override;
		void setMapSize(const irr::core::vector2di &mapSize) override;
		void setCameraPos() override;

		void manageEvent() override;

		protected:
		irrl::ManageIrrlicht &_base;
		my::ItemLocker<my::ThreadPool> &_eventManager;
		my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> &_entities;

		irr::IrrlichtDevice *_device = nullptr;
		irr::video::IVideoDriver *_driver = nullptr;
		irr::scene::ISceneManager *_sceneManager = nullptr;
		irrklang::ISoundEngine *_engine = nullptr;

		irr::gui::IGUIFont* _font = nullptr;

		std::vector<std::string> _listSave;

		irrl::EventManager _eventReceiver;
		bool _displayGUI = true;
		bool _splitScreen = true;

		irr::io::path _currentScene = "";

		// Game settings
		int _nb_player = 1;
		int _nb_ia = 0;
		std::pair<std::size_t, std::size_t> _mapSize = {11, 13};
		int _crates = 80;
		int _drop = 35;
		int _bombUp = 50;
		int _fireUp = 29;
		int _speedUp = 19;
		int _wallPass = 2;
		std::recursive_mutex _mutex;

		irrklang::ISound *_sound = nullptr;

		private:
		float _tmp = 0;
	};

}

#endif //CPP_INDIE_STUDIO_AMANAGEIRRLICHT_HPP
