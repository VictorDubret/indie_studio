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

namespace nts {

	class ManageObject {
		public:

		// Create functions
		static bool createCube(nts::ManageIrrlicht &, std::shared_ptr<is::IEntity> &obj, irr::f32 size=10.0f,
			irr::s32 id=-1,
			const irr::core::vector3df& position = irr::core::vector3df(0,0,0),
			const irr::core::vector3df& rotation = irr::core::vector3df(0,0,0),
			const irr::core::vector3df& scale = irr::core::vector3df(1.0f, 1.0f, 1.0f));

		// Texture functions
		static void setTexture(nts::ManageIrrlicht &, std::shared_ptr<is::IEntity> &obj, const irr::io::path &path);

		// Positions functions
		static void setPosition(nts::ManageIrrlicht &, std::shared_ptr<is::IEntity> &obj, irr::core::vector3df &position);
		static irr::core::vector3df getPosition(nts::ManageIrrlicht &, std::shared_ptr<is::IEntity> &obj);

		// Set Material functions
		static void setMaterialLight(nts::ManageIrrlicht &manage, std::shared_ptr<is::IEntity> &obj, bool opt);
	};

}

#endif //CPP_INDIE_STUDIO_MANAGEOBJECT_HPP
