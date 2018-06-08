/*
** EPITECH PROJECT, 2018
** bomberman
** File description:
** Created by martin.januario@epitech.eu,
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <Entity/Scenery/Wall/Wall.hpp>
#include <Entity/PowerUp/FireUp/FireUp.hpp>
#include <Entity/PowerUp/BombUp/BombUp.hpp>
#include <Entity/PowerUp/WallPass/WallPass.hpp>
#include <Entity/PowerUp/SpeedUp/SpeedUp.hpp>
#include <Entity/Scenery/UnbreakableWall/UnbreakableWall.hpp>
#include <Entity/Bomb/Bomb.hpp>
#include "MapGenerator.hpp"
#include "ArtificialIntelligence.hpp"
#include "GUI.hpp"

nts::GUI::GUI(my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> &entities, my::ItemLocker<my::ThreadPool> &eventManager, nts::ManageIrrlicht &irrlicht) : AManageIrrlicht(entities, eventManager, irrlicht)
{
	_gui = getDevice()->getGUIEnvironment();

	initSettingsScene();
	initBaseScene();
}

void nts::GUI::drawGUI()
{
	if (_device->run())
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
	getDriver()->getTexture(no_hover);
	getDriver()->getTexture(hover);
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

void nts::GUI::setEntity(const std::vector<std::string> &tmpVector, const std::shared_ptr<is::IEntity> &player_tmp2, nts::ManageIrrlicht &base)
{
	irr::core::vector3df tmpPos(std::stof(tmpVector[1]), std::stof(tmpVector[2]), std::stof(tmpVector[3]));
	_base.getNode(player_tmp2.get())->setPosition(irr::core::vector3df(tmpPos));
	//	std::cout << "Mon entite est en :" << tmp.getNode(player_tmp2.get())->getPosition().X << "][" << tmp.getNode(player_tmp2.get())->getPosition().Z;


	// IsPickable [0] IsWalkable [0] Iscollidable [1] isWallPassable [0]
	if (tmpVector[4] == "IsPickable")
		player_tmp2.get()->setPickable(static_cast<bool>(stoi(tmpVector[5])));
	if (tmpVector[6] == "IsWalkable")
		player_tmp2.get()->setWalkable(static_cast<bool>(stoi(tmpVector[7])));
	if (tmpVector[8] == "Iscollidable")
		player_tmp2.get()->setCollidable(static_cast<bool>(stoi(tmpVector[9])));
	if (tmpVector[10] == "IsWallPassable")
		player_tmp2.get()->setWallPassable(static_cast<bool>(stoi(tmpVector[11])));
	if (tmpVector.size() == 14) {
		auto isWall = dynamic_cast<is::Wall *>(player_tmp2.get());
		if (isWall != nullptr)
			isWall->setPowerUp(tmpVector[13][0]);
	}
}

void nts::GUI::initBaseScene()
{
	_gui->clear();
	_currentScene = "base";

	_gui->addImage(getDriver()->getTexture("media/Bomberman_.png"), irr::core::position2d<irr::s32>(230, 100));
	_gui->addImage(getDriver()->getTexture("media/Bombermon.png"), irr::core::position2d<irr::s32>(620, 110));

	std::ifstream infile(".save.indie");
	if (infile.good()) {
		addButtonImage("load_game", "base", "media/load_game_hover.png", "media/load_game.png", irr::core::rect<irr::s32>(1100, 210, 1313, 250), [this](const struct nts::hover_s &) {
			/////////////////////////////////////////////////

			is::ACharacter *player2;

			std::stringstream streamLine;
			std::string line;
			std::string temp;
			std::ifstream myfile("save.indie");
			if (myfile.is_open()) {
				while (getline(myfile, line)) {
					std::cout << line << std::endl;

					streamLine << line;
					std::vector<std::string> tmpVector;
					while (streamLine >> temp) {
						tmpVector.push_back(temp);
						std::cout << "J split ma commande: " << temp << std::endl;
					}
					streamLine.clear();
					if (tmpVector.size() == 12 || tmpVector.size() == 14) {
						if (tmpVector[0] == "Character") {
							std::cout << "J'ai trouvé un character" << std::endl;

							player2 = new is::ACharacter(_entities, _eventManager, _base);
							std::shared_ptr<is::IEntity> player_tmp2 = std::shared_ptr<is::IEntity>(player2, [](is::IEntity *) {});

							irr::core::vector3df tmpPos(std::stoi(tmpVector[1]), std::stoi(tmpVector[2]), std::stoi(tmpVector[3]));
							getNode(player_tmp2.get())->setPosition(irr::core::vector3df(tmpPos));
							std::cout << "Mon joueur est en :" << _base.getNode(player_tmp2.get())->getPosition().X << "][" << _base.getNode(player_tmp2.get())->getPosition().Z;



						} else if (tmpVector[0] == "Wall") {
							is::IEntity *tmp_data = new is::Wall(_entities, _eventManager, _base);
							std::shared_ptr<is::IEntity> wall = std::shared_ptr<is::IEntity>(tmp_data, [](is::IEntity *) {});
							setEntity(tmpVector, wall, _base);
						} else if (tmpVector[0] == "UnbreakableWall") {
							is::IEntity *tmp_data = new is::UnbreakableWall(_entities, _eventManager, _base);
							std::shared_ptr<is::IEntity> wall = std::shared_ptr<is::IEntity>(tmp_data, [](is::IEntity *) {});
							setEntity(tmpVector, wall, _base);
						} else if (tmpVector[0] == "SpeedUp") {
							is::IEntity *tmp_data = new is::SpeedUp(_entities, _eventManager, _base);
							std::shared_ptr<is::IEntity> wall = std::shared_ptr<is::IEntity>(tmp_data, [](is::IEntity *) {});
							setEntity(tmpVector, wall, _base);
						} else if (tmpVector[0] == "WallPass") {
							is::IEntity *tmp_data = new is::WallPass(_entities, _eventManager, _base);
							std::shared_ptr<is::IEntity> wall = std::shared_ptr<is::IEntity>(tmp_data, [](is::IEntity *) {});
							setEntity(tmpVector, wall, _base);
						} else if (tmpVector[0] == "BombUp") {
							is::IEntity *tmp_data = new is::BombUp(_entities, _eventManager, _base);
							std::shared_ptr<is::IEntity> wall = std::shared_ptr<is::IEntity>(tmp_data, [](is::IEntity *) {});
							setEntity(tmpVector, wall, _base);
						} else if (tmpVector[0] == "FireUp") {
							is::IEntity *tmp_data = new is::FireUp(_entities, _eventManager, _base);
							std::shared_ptr<is::IEntity> wall = std::shared_ptr<is::IEntity>(tmp_data, [](is::IEntity *) {});
							setEntity(tmpVector, wall, _base);
						} else if (tmpVector[0] == "Bomb") {

							is::IEntity *tmp_data = new is::Bomb(_entities, _eventManager, reinterpret_cast<std::shared_ptr<is::IEntity> &>(player2), _base);
							std::shared_ptr<is::IEntity> wall = std::shared_ptr<is::IEntity>(tmp_data, [](is::IEntity *) {});
							setEntity(tmpVector, wall, _base);

							std::cout << "Ma bombe est en :" << _base.getNode(wall.get())->getPosition().X << "][" << _base.getNode(wall.get())->getPosition().Z;

							//	is::IEntity *tmp_data = new is::Bomb(_entities, _eventManager, player_tmp2, tmp, 5);
							//	std::shared_ptr<is::IEntity> bomb = std::shared_ptr<is::IEntity>(tmp_data, [](is::IEntity *) {});
							//setEntity(tmpVector, bomb, tmp);
						}

					}
				}
				myfile.close();
				_displayGUI = false;
			}


			/////////////////////////////////////////////////
		});
		addButtonImage("launchSettings", "base", "media/textfx(1).png", "media/textfx.png", irr::core::rect<irr::s32>(1100, 260, 1313, 300), [this](const struct nts::hover_s &) {initSettingsScene();});
	} else
		addButtonImage("launchSettings", "base", "media/textfx(1).png", "media/textfx.png", irr::core::rect<irr::s32>(1100, 210, 1313, 276), [this](const struct nts::hover_s &) {initSettingsScene();});



	addButtonImage("playGame", "base", "media/button_hover.png", "media/button.png", irr::core::rect<irr::s32>(1000, 450, 1300, 750), [this](const struct nts::hover_s &) {
		_base.resetListObj();
		getSceneManager()->clear();
		updateView();
		addPlayerAndIA();
		_displayGUI = false;
	});
}

void nts::GUI::initSettingsScene()
{
	_gui->clear();
	_currentScene = "settings";

	_gui->addImage(getDriver()->getTexture("media/Bombermon.png"), irr::core::position2d<irr::s32>(230, 80));
	_gui->addImage(getDriver()->getTexture("media/number_player.png"), irr::core::position2d<irr::s32>(230, 230));
	_gui->addImage(getDriver()->getTexture("media/number_ia.png"), irr::core::position2d<irr::s32>(230, 350));

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

	addButtonImage("launchHome", "settings", "media/home_button_hover.png", "media/home_button.png", irr::core::rect<irr::s32>(1290, 60, 1390, 160), [this](const struct nts::hover_s &) {initBaseScene();});
	addButtonImage("number1", "settings", "media/number1_hover.png", "media/number1.png", irr::core::rect<irr::s32>(700, 200, 800, 300), [this](const struct nts::hover_s &) {_nb_player = 1;initSettingsScene();});
	addButtonImage("number2", "settings", "media/number2_hover.png", "media/number2.png", irr::core::rect<irr::s32>(810, 200, 900, 300), [this](const struct nts::hover_s &) {_nb_player = 2;if (_nb_ia == 3)_nb_ia = 2;_hoverManage["settings"].erase("number_ia3");initSettingsScene();});

	addButtonImage("number_ia0", "settings", "media/number0_hover.png", "media/number0.png", irr::core::rect<irr::s32>(700, 320, 800, 420), f);
	addButtonImage("number_ia1", "settings", "media/number1_hover.png", "media/number1.png", irr::core::rect<irr::s32>(810, 320, 910, 420), f);
	addButtonImage("number_ia2", "settings", "media/number2_hover.png", "media/number2.png", irr::core::rect<irr::s32>(920, 320, 1020, 420), f);
	if (_nb_player == 1)
		addButtonImage("number_ia3", "settings", "media/number3_hover.png", "media/number3.png", irr::core::rect<irr::s32>(1030, 320, 1130, 420), f);

	_hoverManage["settings"]["number" + std::to_string(_nb_player)].base->setImage(getDriver()->getTexture(_hoverManage["settings"]["number" + std::to_string(_nb_player)].hover));
	_hoverManage["settings"]["number" + std::to_string(_nb_player)].used = false;

	_hoverManage["settings"]["number_ia" + std::to_string(_nb_ia)].base->setImage(getDriver()->getTexture(_hoverManage["settings"]["number_ia" + std::to_string(_nb_ia)].hover));
	_hoverManage["settings"]["number_ia" + std::to_string(_nb_ia)].used = false;

}

void nts::GUI::addPlayer(float x, float z)
{
	is::ACharacter *player = new is::ACharacter(_entities, _eventManager, _base);
	player->setZ(x);
	player->setX(z);
	player->setBombMax(5);
	player->setBomb(5);
}

void nts::GUI::addIA(float x, float z)
{
	is::ACharacter *player = new is::ArtificialIntelligence(_entities, _eventManager, _base);
	player->setZ(x);
	player->setX(z);
	player->setBombMax(5);
	player->setBomb(5);
}

void nts::GUI::addPlayerAndIA()
{
	mg::MapGenerator tmp(_entities, _eventManager, _base, _mapSize);

	addPlayer(1, 1);
	if (_nb_player == 2)
		addPlayer(_base.getMapSize().X, _base.getMapSize().Y);
	if (_nb_ia >= 1)
		addIA(_base.getMapSize().X, 1);
	if (_nb_ia >= 2)
		addIA(1, _base.getMapSize().Y);
	if (_nb_ia >= 3)
		addIA(_base.getMapSize().X, _base.getMapSize().Y);


}