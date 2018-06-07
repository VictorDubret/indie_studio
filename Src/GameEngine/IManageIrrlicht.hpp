/*
** EPITECH PROJECT, 2018
** test
** File description:
** Created by martin.januario@epitech.eu,
*/

#ifndef TEST_IMANAGEIRRLICHT_HPP
# define TEST_IMANAGEIRRLICHT_HPP

# include <memory>
# include <irrlicht.h>
# include <irrKlang.h>
# include "ThreadPool.hpp"
# include "ItemLocker.hpp"
# include "IEntity.hpp"

namespace nts {

	class IManageIrrlicht {
		public:
		// Update the view of the map, take the 2 farest player
		virtual void updateView() = 0;
		// call each function needed on the loop
		virtual void loopDisplay() = 0;

		// add/remove ISceneNode into a map of IEntity
		virtual bool addEntity(std::shared_ptr<is::IEntity> &, irr::scene::ISceneNode *, float) = 0;
		virtual bool deleteEntity(std::shared_ptr<is::IEntity> &) = 0;

		// get the object
		virtual irr::scene::ISceneNode *getNode(const std::shared_ptr<is::IEntity> &) = 0;
		virtual float &getNodeSize(const std::shared_ptr<is::IEntity> &) = 0;
		virtual void resetListObj() = 0;

		// Getter
		virtual irr::scene::ISceneManager *getSceneManager() const = 0;
		virtual irr::IrrlichtDevice *getDevice() const = 0;
		virtual irr::video::IVideoDriver *getDriver() const = 0;
		virtual irrklang::ISoundEngine *getSoundDevice() const = 0;
		virtual irr::core::vector2di getMapSize() const = 0;

		// Setter
		virtual void setMapSize(const irr::core::vector2di &mapSize) = 0;
		virtual void setCameraPos() = 0;

		protected:
		private:
	};

}

#endif //TEST_IMANAGEIRRLICHT_HPP
