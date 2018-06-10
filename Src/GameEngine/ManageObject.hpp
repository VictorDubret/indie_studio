/*
** EPITECH PROJECT, 2018
** cpp_indie_studio
** File description:
** Created by martin.januario@epitech.eu,
*/

#ifndef CPP_INDIE_STUDIO_MANAGEOBJECT_HPP
# define CPP_INDIE_STUDIO_MANAGEOBJECT_HPP

# include <irrlicht.h>
# include <memory>
# include "ManageIrrlicht.hpp"
# include "IEntity.hpp"

namespace irrl {

	class ManageObject {
		public:

		// Create functions
		static bool createCube(irrl::ManageIrrlicht &, std::shared_ptr<is::IEntity> &obj, irr::f32 size,
			irr::s32 id=-1,
			const irr::core::vector3df& position = irr::core::vector3df(0,0,0),
			const irr::core::vector3df& rotation = irr::core::vector3df(0,0,0),
			const irr::core::vector3df& scale = irr::core::vector3df(1.0f, 1.0f, 1.0f));
		static bool createAnim(irrl::ManageIrrlicht &, std::shared_ptr<is::IEntity> &obj, const irr::io::path &path, irr::f32 size,
			irr::s32 id=-1,
			const irr::core::vector3df& position = irr::core::vector3df(0,0,0),
			const irr::core::vector3df& rotation = irr::core::vector3df(0,0,0),
			const irr::core::vector3df& scale = irr::core::vector3df(1.0f, 1.0f, 1.0f));

		// Texture functions
		static void setTexture(irrl::ManageIrrlicht &, std::shared_ptr<is::IEntity> &obj, const irr::io::path &path);

		// obj management
		static void setScale(irrl::ManageIrrlicht &manage, std::shared_ptr<is::IEntity> &obj, const irr::core::vector3df &);
		static void setRotation(irrl::ManageIrrlicht &manage, std::shared_ptr<is::IEntity> &obj, const irr::core::vector3df &rotation);

		// Positions functions
		static void setPosition(irrl::ManageIrrlicht &, std::shared_ptr<is::IEntity> &obj, irr::core::vector3df &position);
		static irr::core::vector3df getPosition(irrl::ManageIrrlicht &, std::shared_ptr<is::IEntity> &obj);

		// Set Material functions
		static void setMaterialLight(irrl::ManageIrrlicht &manage, std::shared_ptr<is::IEntity> &obj, bool opt);

		// Manage animation
		static void setAnimation(irrl::ManageIrrlicht &manage, std::shared_ptr<is::IEntity> &obj, irr::scene::EMD2_ANIMATION_TYPE type);
	};

}

#endif //CPP_INDIE_STUDIO_MANAGEOBJECT_HPP
