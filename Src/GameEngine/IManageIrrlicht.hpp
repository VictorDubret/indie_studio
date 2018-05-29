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
# include "ThreadPool.hpp"
# include "ItemLocker.hpp"
# include "IEntity.hpp"

namespace nts {

	class IManageIrrlicht {
		public:
		// Constructor & detructor
		IManageIrrlicht() = default;
		virtual ~IManageIrrlicht() = default;

		// Update the view of the map, take the 2 farest player
		virtual void updateView() = 0;
		// call each function needed on the loop
		virtual void loopDisplay() = 0;

		// add ISceneNode into a map of IEntity
		virtual bool addEntity(std::shared_ptr<is::IEntity> &, irr::IReferenceCounted *) = 0;
		// get the object
		virtual irr::IReferenceCounted *getNode(std::shared_ptr<is::IEntity> &) = 0;

		// Get the scene manager
		virtual irr::scene::ISceneManager *getSceneManager() const = 0;

		protected:
		private:
	};

}

#endif //TEST_IMANAGEIRRLICHT_HPP
