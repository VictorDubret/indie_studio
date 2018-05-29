/*
** EPITECH PROJECT, 2018
** cpp_indie_studio
** File description:
** Created by martin.januario@epitech.eu,
*/

#ifndef CPP_INDIE_STUDIO_EVENTMANAGER_HPP
# define CPP_INDIE_STUDIO_EVENTMANAGER_HPP

#include <irrlicht.h>
#include <vector>

namespace nts {

	class EventManager : public irr::IEventReceiver {
		public:
		EventManager();

		bool OnEvent(const irr::SEvent& event);
		bool IsKeyDown(irr::EKEY_CODE keyCode) const;
		std::vector<irr::EKEY_CODE> getAllKeyDown();

		private:
		bool KeyIsDown[irr::KEY_KEY_CODES_COUNT];
	};
}

#endif //CPP_INDIE_STUDIO_EVENTMANAGER_HPP
