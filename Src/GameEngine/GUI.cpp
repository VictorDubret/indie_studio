/*
** EPITECH PROJECT, 2018
** bomberman
** File description:
** Created by martin.januario@epitech.eu,
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <Entity/Scenery/Wall/Wall.hpp>
#include <Entity/PowerUp/FireUp/FireUp.hpp>
#include <Entity/PowerUp/BombUp/BombUp.hpp>
#include <Entity/PowerUp/WallPass/WallPass.hpp>
#include <Entity/PowerUp/SpeedUp/SpeedUp.hpp>
#include <Entity/Scenery/UnbreakableWall/UnbreakableWall.hpp>
#include <Entity/Bomb/Bomb.hpp>
#include "MapGenerator.hpp"
#include "ArtificialIntelligence.hpp"

irrl::GUI::GUI(my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> &entities, my::ItemLocker<my::ThreadPool> &eventManager, irrl::ManageIrrlicht &irrlicht) : AManageIrrlicht(entities, eventManager, irrlicht)
{
	_gui = getDevice()->getGUIEnvironment();


	_gui->getSkin()->setFont(_font);
	initPause();
	initSettingsScene();
	initBaseScene();
}

void irrl::GUI::drawGUI()
{
	if (_device->run())
		_gui->drawAll();
}

void irrl::GUI::manageEventGui()
{
	for (auto &&it : _hoverManage[_currentScene]) {
		irrl::EventManager::SMouseState tmp = _eventReceiver.GetMouseState();
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

void irrl::GUI::addButton(const wchar_t *text, const irr::core::rect<irr::s32> &textRect, const irr::io::path &trigger)
{
	irr::gui::IGUIFont *font = _gui->getFont("media/fontlucida.png");
	irr::gui::IGUIButton *bouton = _gui->addButton(textRect, 0, -1, text);
	bouton->setOverrideFont(font);

	if (trigger != "")
		bouton->setPressedImage(getDriver()->getTexture(trigger));
}

void irrl::GUI::addButtonImage(const std::string &name, const irr::io::path &scene, const irr::io::path &hover, const irr::io::path &no_hover, const irr::core::rect<irr::s32> &sq, const std::function<void(const struct irrl::hover_s &)> &f)
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

void irrl::GUI::initPause()
{
	_gui->clear();
	_currentScene = "pause";

	_saveName = _gui->addEditBox(L"Save name", irr::core::rect<irr::s32>(500, 360, 800, 460), true, 0, SAVE_NAME);
	_saveName->setOverrideColor(irr::video::SColor(255, 255, 255, 255));

	_gui->addImage(getDriver()->getTexture("media/Bombermon.png"), irr::core::position2d<irr::s32>(450, 110));
	addButtonImage("continue", "pause", "media/continue_hover.png", "media/continue.png", irr::core::rect<irr::s32>(650, 240, 1050, 340), [this](const struct irrl::hover_s &) {_displayGUI = false;_base.endPause();_currentScene = "game";});
	addButtonImage("save", "pause", "media/save_hover.png", "media/save.png", irr::core::rect<irr::s32>(850, 360, 1050, 460), [this](const struct irrl::hover_s &) {
		_entities.lock();

		std::wstring ws(_saveName->getText());
		std::string test(ws.begin(), ws.end());

		if (test.empty()) {
			_entities.unlock();
			return;
		}

		std::ofstream _filestr("save/" + test);

		std::this_thread::sleep_for(std::chrono::seconds(1));
		for (auto &it : _entities.get()) {
			auto isIA = dynamic_cast<is::ArtificialIntelligence *>(it.get());
			if (isIA == nullptr)
				_filestr << it->getType();
			else
				_filestr << "IA";
			_filestr << " " << it->getX() << " " << it->getY() << " " << it->getZ() << " IsPickable " << it->isPickable() << " IsWalkable " << it->isWalkable() << " Iscollidable " <<  it->isCollidable() << " isWallPassable " << it->isWallPassable();
			auto isWall = dynamic_cast<is::Wall *>(it.get());
			if (isWall != nullptr)
				_filestr << " PowerUp " << isWall->getPowerUp();
			auto isCharacter = dynamic_cast<is::ACharacter *>(it.get());
			if (isCharacter != nullptr)
				_filestr << " bombMax " << isCharacter->getBombMax() << " speed " << isCharacter->getSpeed() << " bombLength " << isCharacter->getBombLength() << " wallPass " << isCharacter->getWallPass();
			_filestr << "\n";
		}
		_saveName->setText(L"Save name");
		_entities.unlock();

	});
	addButtonImage("menu", "pause", "media/menu_hover.png", "media/menu.png", irr::core::rect<irr::s32>(740, 490, 940, 590), [this](const struct irrl::hover_s &) {
		initBaseScene();
	});
}

void irrl::GUI::initBaseScene()
{
	{
		std::lock_guard<std::recursive_mutex> lk(_mutex);
		_gui->clear();
		_currentScene = "base";

		_gui->addImage(getDriver()->getTexture("media/Bomberman_.png"), irr::core::position2d<irr::s32>(230, 100));
		_gui->addImage(getDriver()->getTexture("media/Bombermon.png"), irr::core::position2d<irr::s32>(620, 110));

		_saveLoad = _gui->addEditBox(L"Save name", irr::core::rect<irr::s32>(820, 210, 1070, 250));
		_saveLoad->setOverrideColor(irr::video::SColor(255, 255, 255, 255));

		_base.resetListObj();
		addButtonImage("load_game", "base", "media/load_game_hover.png", "media/load_game.png", irr::core::rect<irr::s32>(1100, 210, 1313, 250), [this](const struct irrl::hover_s &) {
			_base.resetListObj();
			_base.lock();

			std::wstring ws(_saveLoad->getText());
			std::string test(ws.begin(), ws.end());

			if (test.empty()) {
				_base.unlock();
				return;
			}

			std::ifstream _filestr("save/" + test);

			if (!_filestr.is_open()) {
				_base.unlock();
				return;
			}

			getSceneManager()->clear();
			updateView();
			if (!loadSave()) {
				_base.resetListObj();
				getSceneManager()->clear();
				updateView();
			}
			_base.unlock();
		});
		addButtonImage("launchSettings", "base", "media/textfx(1).png", "media/textfx.png", irr::core::rect<irr::s32>(1100, 260, 1313, 300), [this](const struct irrl::hover_s &) {
			initSettingsScene();
		});

		addButtonImage("playGame", "base", "media/button_hover.png", "media/button.png", irr::core::rect<irr::s32>(1000, 450, 1300, 750), [this](const struct irrl::hover_s &) {
			_base.resetListObj();
			if (_nb_player != 2)
				_splitScreen = false;
			if (_nb_player + _nb_ia == 1)
				_nb_ia = 1;
			_base.lock();
			getSceneManager()->clear();
			_base.unlock();
			addPlayerAndIA();
			_base.setFloor();
			updateView();
			if (_sound)
				_sound->stop();
			_sound = nullptr;
			_displayGUI = false;
			// setCameraPos();
			_sound = getSoundDevice()->play2D("media/sound/battle.ogg", true, false, true, irrklang::ESM_AUTO_DETECT, true);
			_currentScene = "game";
			_base.checkLastAlive();
		});
	}
}

int irrl::GUI::getValueInput(irr::gui::IGUIEditBox *obj, int min, int max)
{
	if (!obj)
		throw std::exception();

	const wchar_t *txt = obj->getText();
	std::wstring ws(txt);
	std::string str(ws.begin(), ws.end());

	int tmp = 0;
	try {
		 tmp = std::stoi(str.c_str());
	} catch (const std::invalid_argument &e) {
		tmp = 0;
	}

	if (tmp < min)
		tmp = min;
	else if (tmp > max)
		tmp = max;
	return tmp;
}

void irrl::GUI::updateRateSettings()
{
	_mapSize.first = (std::size_t)getValueInput(_mapXEditBox, 11, 20);
	_mapSize.second = (std::size_t)getValueInput(_mapYEditBox, 13, 20);
	_crates = getValueInput(_crateEditBox, 0, 100);
	_drop = getValueInput(_dropEditBox, 0, 100);
	_bombUp = getValueInput(_bombEditBox, 0, 100);
	_fireUp = getValueInput(_fireEditBox, 0, 100);
	_speedUp = getValueInput(_speedEditBox, 0, 100);
	_wallPass = getValueInput(_wallpassEditBox, 0, 100);
}

void irrl::GUI::initSettingsScene()
{
	{
		std::lock_guard<std::recursive_mutex> lk(_mutex);
		_gui->clear();
	}
	_currentScene = "settings";

	_gui->addImage(getDriver()->getTexture("media/Bombermon.png"), irr::core::position2d<irr::s32>(230, 80));
	_gui->addImage(getDriver()->getTexture("media/number_player.png"), irr::core::position2d<irr::s32>(230, 230));
	_gui->addImage(getDriver()->getTexture("media/number_ia.png"), irr::core::position2d<irr::s32>(230, 350));

	auto f = [this](const struct irrl::hover_s &obj) {
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

	addButtonImage("launchHome", "settings", "media/home_button_hover.png", "media/home_button.png", irr::core::rect<irr::s32>(1290, 60, 1390, 160), [this](const struct irrl::hover_s &) {
		updateRateSettings();
		initBaseScene();
	});
	addButtonImage("sounds_mute", "settings", "media/sound_off.png", "media/sound_on.png", irr::core::rect<irr::s32>(1150, 80, 1250, 180), [this](const struct irrl::hover_s &) {
		_soundMute = !_soundMute;
		_engine->setSoundVolume(_soundMute);
		_hoverManage["settings"]["sounds_mute"].base->setImage(_driver->getTexture((_soundMute) ? "media/sound_on.png" : "media/sound_off.png"));
	});
	_hoverManage["settings"]["sounds_mute"].used = false;
	if (!_soundMute)
		_hoverManage["settings"]["sounds_mute"].base->setImage(_driver->getTexture("media/sound_off.png"));

	addButtonImage("split_screen", "settings", "media/split_screen_hover.png", "media/split_screen.png", irr::core::rect<irr::s32>(1000, 65, 1100, 165), [this](const struct irrl::hover_s &) {
		_splitScreen = !_splitScreen;
		_hoverManage["settings"]["split_screen"].base->setImage(_driver->getTexture((_splitScreen) ? "media/split_screen_hover.png" : "media/split_screen.png"));
	});
	_hoverManage["settings"]["split_screen"].used = false;
	if (_splitScreen)
		_hoverManage["settings"]["split_screen"].base->setImage(_driver->getTexture("media/split_screen_hover.png"));


	addButtonImage("number1", "settings", "media/number1_hover.png", "media/number1.png", irr::core::rect<irr::s32>(700, 200, 800, 300), [this](const struct irrl::hover_s &) {if (_nb_player == 1)return; _nb_player = 1;updateRateSettings();initSettingsScene();});
	addButtonImage("number2", "settings", "media/number2_hover.png", "media/number2.png", irr::core::rect<irr::s32>(810, 200, 900, 300), [this](const struct irrl::hover_s &) {if (_nb_player == 2)return;_nb_player = 2;updateRateSettings();initSettingsScene();});

	addButtonImage("number_ia0", "settings", "media/number0_hover.png", "media/number0.png", irr::core::rect<irr::s32>(700, 320, 800, 420), f);
	addButtonImage("number_ia1", "settings", "media/number1_hover.png", "media/number1.png", irr::core::rect<irr::s32>(810, 320, 910, 420), f);
	addButtonImage("number_ia2", "settings", "media/number2_hover.png", "media/number2.png", irr::core::rect<irr::s32>(920, 320, 1020, 420), f);

	_hoverManage["settings"]["number" + std::to_string(_nb_player)].base->setImage(getDriver()->getTexture(_hoverManage["settings"]["number" + std::to_string(_nb_player)].hover));
	_hoverManage["settings"]["number" + std::to_string(_nb_player)].used = false;

	_hoverManage["settings"]["number_ia" + std::to_string(_nb_ia)].base->setImage(getDriver()->getTexture(_hoverManage["settings"]["number_ia" + std::to_string(_nb_ia)].hover));
	_hoverManage["settings"]["number_ia" + std::to_string(_nb_ia)].used = false;

	_gui->addImage(getDriver()->getTexture("media/map_size.png"), irr::core::position2d<irr::s32>(230, 470));
	_mapXEditBox = _gui->addEditBox(irr::core::stringw(std::to_string(_mapSize.first).c_str()).c_str(), irr::core::rect<irr::s32>(410, 470, 470, 500), true, 0, MAPSIZE_X);
	_mapXEditBox->setOverrideColor(irr::video::SColor(255, 255, 255, 255));
	_mapYEditBox = _gui->addEditBox(irr::core::stringw(std::to_string(_mapSize.second).c_str()).c_str(), irr::core::rect<irr::s32>(480, 470, 540, 500), true, 0, MAPSIZE_Y);
	_mapYEditBox->setOverrideColor(irr::video::SColor(255, 255, 255, 255));

	_gui->addImage(getDriver()->getTexture("media/box_rate.png"), irr::core::position2d<irr::s32>(230, 530));
	_crateEditBox = _gui->addEditBox(irr::core::stringw(std::to_string(_crates).c_str()).c_str(), irr::core::rect<irr::s32>(410, 530, 510, 560), true, 0, CRATE_RATE);
	_crateEditBox->setOverrideColor(irr::video::SColor(255, 255, 255, 255));

	_gui->addImage(getDriver()->getTexture("media/drop_rate.png"), irr::core::position2d<irr::s32>(230, 590));
	_dropEditBox = _gui->addEditBox(irr::core::stringw(std::to_string(_drop).c_str()).c_str(), irr::core::rect<irr::s32>(410, 590, 510, 620), true, 0, DROP_RATE);
	_dropEditBox->setOverrideColor(irr::video::SColor(255, 255, 255, 255));

	_gui->addImage(getDriver()->getTexture("media/bombup_rate.png"), irr::core::position2d<irr::s32>(620, 470));
	_bombEditBox = _gui->addEditBox(irr::core::stringw(std::to_string(_bombUp).c_str()).c_str(), irr::core::rect<irr::s32>(890, 470, 990, 500), true, 0, BOMBEUP_RATE);
	_bombEditBox->setOverrideColor(irr::video::SColor(255, 255, 255, 255));

	_gui->addImage(getDriver()->getTexture("media/fireup_rate.png"), irr::core::position2d<irr::s32>(620, 530));
	_fireEditBox = _gui->addEditBox(irr::core::stringw(std::to_string(_fireUp).c_str()).c_str(), irr::core::rect<irr::s32>(890, 530, 990, 560), true, 0, FIREUP_RATE);
	_fireEditBox->setOverrideColor(irr::video::SColor(255, 255, 255, 255));

	_gui->addImage(getDriver()->getTexture("media/speedup_rate.png"), irr::core::position2d<irr::s32>(620, 590));
	_speedEditBox = _gui->addEditBox(irr::core::stringw(std::to_string(_speedUp).c_str()).c_str(), irr::core::rect<irr::s32>(890, 590, 990, 620), true, 0, SPEEDUP_RATE);
	_speedEditBox->setOverrideColor(irr::video::SColor(255, 255, 255, 255));

	_gui->addImage(getDriver()->getTexture("media/wallpass_rate.png"), irr::core::position2d<irr::s32>(620, 650));
	_wallpassEditBox = _gui->addEditBox(irr::core::stringw(std::to_string(_wallPass).c_str()).c_str(), irr::core::rect<irr::s32>(890, 650, 990, 680), true, 0, WALLPASS_RATE);
	_wallpassEditBox->setOverrideColor(irr::video::SColor(255, 255, 255, 255));
}

void irrl::GUI::addPlayer(float x, float z, std::size_t id)
{
	is::ACharacter *player = new is::ACharacter(_entities, _eventManager, _base, id);
	player->setZ(x + !((int)x % 2));
	player->setX(z + !((int)z % 2));
	player->setBombMax(1);
	player->setBomb(1);
}

void irrl::GUI::addIA(float x, float z, std::size_t id)
{
	is::ACharacter *player = new is::ArtificialIntelligence(_entities, _eventManager, _base, id);
	player->setZ(x + !((int)x % 2));
	player->setX(z + !((int)z % 2));
	player->setBombMax(1);
	player->setBomb(1);
}

void irrl::GUI::addPlayerAndIA()
{
	addPlayer(1, 1, 1);
	if (_nb_player == 2)
		addPlayer(_base.getMapSize().X, _base.getMapSize().Y, 2);
	if (_nb_ia) {
		_mapSize.first = 11;
		_mapSize.second = 13;
	}


	mg::MapGenerator tmp(_entities, _eventManager, _base, _mapSize, _crates, _drop, _bombUp, _fireUp, _speedUp, _wallPass);

	if (_nb_ia >= 1)
		addIA(_base.getMapSize().X, 1, 3);
	if (_nb_ia >= 2)
		addIA(1, _base.getMapSize().Y, 4);
	if (_nb_ia >= 3)
		addIA(_base.getMapSize().X, _base.getMapSize().Y, 2);


}

irr::io::path &irrl::GUI::getCurrentScene()
{
	return _currentScene;
}

bool irrl::GUI::isNumber(const std::string& s)
{
	std::string::const_iterator it = s.begin();
	while (it != s.end() && isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}

bool irrl::GUI::setEntity(const std::vector<std::string> &tmpVector, const std::shared_ptr<is::IEntity> &player_tmp2, irrl::ManageIrrlicht &base)
{
	irr::core::vector3df tmpPos(std::stof(tmpVector[1]), std::stof(tmpVector[2]), std::stof(tmpVector[3]));
	_base.getNode(player_tmp2.get())->setPosition(irr::core::vector3df(tmpPos));

	if (tmpVector[4] == "IsPickable" && isNumber(tmpVector[5]))
		player_tmp2.get()->setPickable(static_cast<bool>(stoi(tmpVector[5])));
	else return false;
	if (tmpVector[6] == "IsWalkable" && isNumber(tmpVector[7]))
		player_tmp2.get()->setWalkable(static_cast<bool>(stoi(tmpVector[7])));
	else return false;
	if (tmpVector[8] == "Iscollidable" && isNumber(tmpVector[9]))
		player_tmp2.get()->setCollidable(static_cast<bool>(stoi(tmpVector[9])));
	else return false;
	if (tmpVector[10] == "isWallPassable" && isNumber(tmpVector[11]))
		player_tmp2.get()->setWallPassable(static_cast<bool>(stoi(tmpVector[11])));
	else return false;
	if (tmpVector.size() == 14) {
		auto isWall = dynamic_cast<is::Wall *>(player_tmp2.get());
		if (isWall != nullptr)
			isWall->setPowerUp(tmpVector[13][0]);
		else return false;
	}
	if (tmpVector.size() == 20) {
		auto isCharacter = dynamic_cast<is::ACharacter *>(player_tmp2.get());
		if (isCharacter != nullptr) {
			if (isNumber(tmpVector[13]) && isNumber(tmpVector[17]) && isNumber(tmpVector[19]))
			isCharacter->setBombMax(static_cast<size_t>(stoi(tmpVector[13])));
			isCharacter->setBomb(static_cast<size_t>(stoi(tmpVector[13])));
			isCharacter->setSpeed(stof(tmpVector[15]));
			isCharacter->setBombLength(static_cast<size_t>(stoi(tmpVector[17])));
			isCharacter->setWallPass(static_cast<bool>(stoi(tmpVector[19])));

		} else return false;
	}
	return true;
}

bool irrl::GUI::loadEntity(is::ACharacter *&player2, int &j, std::vector<std::string> &tmpVector)
{
	if (tmpVector[0] == "Character") {
		j++;
		player2 = new is::ACharacter(_entities, _eventManager, _base, (std::size_t)j);
		std::shared_ptr<is::IEntity> player_tmp2 = std::shared_ptr<is::IEntity>(player2, [](is::IEntity *) {});
		if (!setEntity(tmpVector, player_tmp2, _base))
			return false;
	} else if (tmpVector[0] == "IA") {
		j++;
		player2 = new is::ArtificialIntelligence(_entities, _eventManager, _base, (std::size_t)j);
		std::shared_ptr<is::IEntity> player_tmp2 = std::shared_ptr<is::IEntity>(player2, [](is::IEntity *) {});
		if (!setEntity(tmpVector, player_tmp2, _base))
			return false;
	} else if (tmpVector[0] == "Wall") {
		is::IEntity *tmp_data = new is::Wall(_entities, _eventManager, _base);
		std::shared_ptr<is::IEntity> wall = std::shared_ptr<is::IEntity>(tmp_data, [](is::IEntity *) {});
		if (!setEntity(tmpVector, wall, _base))
			return false;
	} else if (tmpVector[0] == "UnbreakableWall") {
		is::IEntity *tmp_data = new is::UnbreakableWall(_entities, _eventManager, _base);
		std::shared_ptr<is::IEntity> wall = std::shared_ptr<is::IEntity>(tmp_data, [](is::IEntity *) {});
		if (!setEntity(tmpVector, wall, _base))
			return false;
	} else if (tmpVector[0] == "SpeedUp") {
		is::IEntity *tmp_data = new is::SpeedUp(_entities, _eventManager, _base);
		std::shared_ptr<is::IEntity> wall = std::shared_ptr<is::IEntity>(tmp_data, [](is::IEntity *) {});
		if (!setEntity(tmpVector, wall, _base))
			return false;
	} else if (tmpVector[0] == "WallPass") {
		is::IEntity *tmp_data = new is::WallPass(_entities, _eventManager, _base);
		std::shared_ptr<is::IEntity> wall = std::shared_ptr<is::IEntity>(tmp_data, [](is::IEntity *) {});
		if (!setEntity(tmpVector, wall, _base))
			return false;
	} else if (tmpVector[0] == "FireUp") {
		is::IEntity *tmp_data = new is::FireUp(_entities, _eventManager, _base);
		std::shared_ptr<is::IEntity> wall = std::shared_ptr<is::IEntity>(tmp_data, [](is::IEntity *) {});
		if (!setEntity(tmpVector, wall, _base))
			return false;
	} /*else if (tmpVector[0] == "Bomb" && player2 != nullptr) {
		std::cerr << "Bomb generated " << std::endl;
		is::IEntity *tmp_data = new is::Bomb(_entities, _eventManager, reinterpret_cast<std::shared_ptr<is::IEntity> &>(player2), _base);
		std::shared_ptr<is::IEntity> wall = std::shared_ptr<is::IEntity>(tmp_data, [](is::IEntity *) {});
		if (!setEntity(tmpVector, wall, _base))
			return false;
	} */else if (tmpVector[0] == "BombUp") {
		is::IEntity *tmp_data = new is::BombUp(_entities, _eventManager, _base);
		std::shared_ptr<is::IEntity> wall = std::shared_ptr<is::IEntity>(tmp_data, [](is::IEntity *) {});
		if (!setEntity(tmpVector, wall, _base))
			return false;
	}
	return true;
}

bool irrl::GUI::loadSave()
{
	std::stringstream streamLine;
	std::string line;
	std::string temp;

	std::wstring ws(_saveLoad->getText());
	std::string test(ws.begin(), ws.end());

	std::ifstream myfile("save/" + test);
	int i = 0;
	int j = 0;
	is::ACharacter *player2 = nullptr;
	if (myfile.is_open()) {
		while (getline(myfile, line)) {
			streamLine << line;
			std::vector<std::string> tmpVector;
			while (streamLine >> temp) {
				tmpVector.push_back(temp);
			}
			streamLine.clear();
			i++;
			if (tmpVector.size() == 12 || tmpVector.size() == 14 || tmpVector.size() == 20) {
				if (!loadEntity(player2, j, tmpVector)) {
					myfile.close();
					return false;
				}
			} else {
				myfile.close();
				return false;
			}
		}
		myfile.close();
		_base.setFloor();
		_displayGUI = false;
		_currentScene = "game";
	}
	return true;
}
