/*
** EPITECH PROJECT, 2018
** cpp_indie_studio
** File description:
** Created by martin.januario@epitech.eu,
*/

#include "EventManager.hpp"

nts::EventManager::EventManager()
{
	for (irr::u32 i = 0; i < irr::KEY_KEY_CODES_COUNT; ++i)
		KeyIsDown[i] = false;
}

std::vector<irr::EKEY_CODE> nts::EventManager::getAllKeyDown()
{
	std::vector<irr::EKEY_CODE> list;

	for (irr::u32 i = 0; i < irr::KEY_KEY_CODES_COUNT; ++i) {
		if (KeyIsDown[i])
			list.push_back(static_cast<irr::EKEY_CODE>(i));
	}
	return list;
}

bool nts::EventManager::OnEvent(const irr::SEvent &event)
{
	if (event.EventType == irr::EET_KEY_INPUT_EVENT)
		KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
	return false;
}

bool nts::EventManager::IsKeyDown(irr::EKEY_CODE keyCode) const
{
	return KeyIsDown[keyCode];
}