/*
** EPITECH PROJECT, 2018
** bomberman
** File description:
** Created by martin.januario@epitech.eu,
*/

#include <iostream>
#include "MapGenerator.hpp"
#include "GUI.hpp"

nts::GUI::GUI(my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> &entities, my::ItemLocker<my::ThreadPool> &eventManager) : AManageIrrlicht(entities, eventManager)
{
	_gui = getDevice()->getGUIEnvironment();

//	initSettingsScene();
	initBaseScene();
}

void nts::GUI::drawGUI()
{
	_gui->drawAll();
}

void nts::GUI::manageEventGui()
{
	for (auto &&it : _hoverManage[_currentScene]) {
		nts::EventManager::SMouseState tmp = _eventReceiver.GetMouseState();
		if (it.second.sq.UpperLeftCorner.X < tmp.Position.X && it.second.sq.LowerRightCorner.X > tmp.Position.X &&
			it.second.sq.UpperLeftCorner.Y < tmp.Position.Y && it.second.sq.LowerRightCorner.Y > tmp.Position.Y) {
			if (tmp.LeftButtonDown && it.second.status) {
				it.second.click(it.second);
				break;
			} else if (!it.second.status && it.second.used)
				it.second.base->setImage(getDriver()->getTexture(it.second.hover));
			it.second.status = true;
		} else if (it.second.status) {
			if (it.second.used)
				it.second.base->setImage(getDriver()->getTexture(it.second.no_hover));
			it.second.status = false;
		}
	}
}

void nts::GUI::addButton(const wchar_t *text, const irr::core::rect<irr::s32> &textRect, const irr::io::path &trigger)
{
	irr::gui::IGUIFont *font = _gui->getFont("media/fontlucida.png");
	irr::gui::IGUIButton *bouton = _gui->addButton(textRect, 0, -1, text);
	bouton->setOverrideFont(font);

	if (trigger != "")
		bouton->setPressedImage(getDriver()->getTexture(trigger));
}

void nts::GUI::addButtonImage(const std::string &name, const irr::io::path &scene, const irr::io::path &hover, const irr::io::path &no_hover, const irr::core::rect<irr::s32> &sq, const std::function<void(const struct nts::hover_s &)> &f)
{
	irr::gui::IGUIImage *base = _gui->addImage(getDriver()->getTexture(no_hover), irr::core::position2d<irr::s32>(sq.UpperLeftCorner.X, sq.UpperLeftCorner.Y));
	if (!base)
		throw std::exception();
	_hoverManage[scene][name] = hover_t();
	_hoverManage[scene][name].base = base;
	_hoverManage[scene][name].name = name;
	_hoverManage[scene][name].hover = hover;
	_hoverManage[scene][name].no_hover = no_hover;
	_hoverManage[scene][name].click = f;
	_hoverManage[scene][name].sq = sq;
	_hoverManage[scene][name].status = false;
	_hoverManage[scene][name].used = true;
}

void nts::GUI::initBaseScene()
{
	_gui->clear();
	_currentScene = "base";

	_gui->addImage(getDriver()->getTexture("media/Bomberman_.png"), irr::core::position2d<irr::s32>(30, 50));
	_gui->addImage(getDriver()->getTexture("media/Bombermon.png"), irr::core::position2d<irr::s32>(420, 60));

	addButtonImage("playGame", "base", "media/button_hover.png", "media/button.png", irr::core::rect<irr::s32>(800, 400, 1100, 700), [this](const struct nts::hover_s &) {/*_base->resetListObj(); getSceneManager()->clear(); mg::MapGenerator tmp(_entities, _eventManager, _base, _mapSizeSettings);*/_displayGUI = false;});
	addButtonImage("launchSettings", "base", "media/textfx(1).png", "media/textfx.png", irr::core::rect<irr::s32>(900, 160, 1113, 226), [this](const struct nts::hover_s &) {initSettingsScene();});
}

void nts::GUI::initSettingsScene()
{
	_gui->clear();
	_currentScene = "settings";

	_gui->addImage(getDriver()->getTexture("media/Bombermon.png"), irr::core::position2d<irr::s32>(30, 30));
	_gui->addImage(getDriver()->getTexture("media/number_player.png"), irr::core::position2d<irr::s32>(30, 180));
	_gui->addImage(getDriver()->getTexture("media/number_ia.png"), irr::core::position2d<irr::s32>(30, 300));

	addButtonImage("launchHome", "settings", "media/home_button_hover.png", "media/home_button.png", irr::core::rect<irr::s32>(1090, 10, 1190, 110), [this](const struct nts::hover_s &) {initBaseScene();});
	addButtonImage("number1", "settings", "media/number1_hover.png", "media/number1.png", irr::core::rect<irr::s32>(500, 150, 600, 250), [this](const struct nts::hover_s &) {_nb_player = 1;_hoverManage["settings"]["number1"].used = false;_hoverManage["settings"]["number2"].used = true;_hoverManage["settings"]["number2"].base->setImage(getDriver()->getTexture(_hoverManage["settings"]["number2"].no_hover));});
	addButtonImage("number2", "settings", "media/number2_hover.png", "media/number2.png", irr::core::rect<irr::s32>(610, 150, 700, 250), [this](const struct nts::hover_s &) {_nb_player = 2;_hoverManage["settings"]["number2"].used = false;_hoverManage["settings"]["number1"].used = true;_hoverManage["settings"]["number1"].base->setImage(getDriver()->getTexture(_hoverManage["settings"]["number1"].no_hover));});

	auto f = [this](const struct nts::hover_s &obj) {
		for (auto &&it : _hoverManage[_currentScene]) {
			if (it.second.name == obj.name) {
				_nb_ia = std::stoi(it.second.name.substr(9));
				_hoverManage["settings"][it.second.name].base->setImage(getDriver()->getTexture(_hoverManage["settings"][it.second.name].hover));
				_hoverManage["settings"][it.second.name].used = false;
			} else if (it.first.substr(0, 9) == "number_ia") {
				_hoverManage["settings"][it.second.name].base->setImage(getDriver()->getTexture(_hoverManage["settings"][it.second.name].no_hover));
				_hoverManage["settings"][it.second.name].used = true;
			}
		}
	};

	addButtonImage("number_ia0", "settings", "media/number0_hover.png", "media/number0.png", irr::core::rect<irr::s32>(500, 270, 600, 370), f);
	addButtonImage("number_ia1", "settings", "media/number1_hover.png", "media/number1.png", irr::core::rect<irr::s32>(610, 270, 710, 370), f);
	addButtonImage("number_ia2", "settings", "media/number2_hover.png", "media/number2.png", irr::core::rect<irr::s32>(720, 270, 820, 370), f);
	addButtonImage("number_ia3", "settings", "media/number3_hover.png", "media/number3.png", irr::core::rect<irr::s32>(830, 270, 930, 370), f);

	_hoverManage["settings"]["number" + std::to_string(_nb_player)].base->setImage(getDriver()->getTexture(_hoverManage["settings"]["number" + std::to_string(_nb_player)].hover));
	_hoverManage["settings"]["number" + std::to_string(_nb_player)].used = false;

	_hoverManage["settings"]["number_ia" + std::to_string(_nb_ia)].base->setImage(getDriver()->getTexture(_hoverManage["settings"]["number_ia" + std::to_string(_nb_ia)].hover));
	_hoverManage["settings"]["number_ia" + std::to_string(_nb_player)].used = false;

}