/*
** EPITECH PROJECT, 2018
** bomberman
** File description:
** Created by martin.januario@epitech.eu,
*/

#include <Entity/Bomb/Bomb.hpp>
#include "Game.hpp"
#include "ArtificialIntelligence.hpp"

irrl::Game::Game(
	my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> &entities,
	my::ItemLocker<my::ThreadPool> &eventManager, irrl::ManageIrrlicht &irrlicht,
	irr::core::vector2di mapSize, bool splitScreen
) : AManageIrrlicht(entities, eventManager, irrlicht)
{
	_splitScreen = splitScreen;
	updateView();
}

irrl::Game::~Game()
{
	_stopThread = true;
	delete _device;
	delete _engine;
	delete _thread;
}

void irrl::Game::updateView()
{
	_endGame = false;
	_draw = false;
	_winPLayer = true;
	irr::core::vector2df tmpDist;
	tmpDist.X = -(_mapSize.first / 2);
	tmpDist.Y = 0;
	_distBetweenPlayer.push_back(tmpDist);
	tmpDist.X = _mapSize.first / 2;
	_distBetweenPlayer.push_back(tmpDist);

	/* Setting Global Camera */
	_camera[GLOBAL] = _sceneManager->addCameraSceneNode(0,
		irr::core::vector3df(getMapSize().X / 2 + 1, static_cast<irr::f32>(getMapSize().X / 1.1), getMapSize().Y / 2),
		irr::core::vector3df(getMapSize().X / 2 + 1, 0, getMapSize().Y / 2 + 1));
	setCameraPos();

	/* Setting Split Screen Camera */
	if (_splitScreen) {
		_camera[PLAYER1] = _sceneManager->addCameraSceneNode(0, irr::core::vector3df(getMapSize().X / 2 + 1, (getMapSize().X / 2), -3), irr::core::vector3df(getMapSize().X / 2 + 1, getMapSize().X / 10, getMapSize().X / 4));
		_camera[PLAYER2] = _sceneManager->addCameraSceneNode(0, irr::core::vector3df(getMapSize().X / 2 + 1, (getMapSize().X / 2), -3), irr::core::vector3df(getMapSize().X / 2 + 1, getMapSize().X / 10, getMapSize().X / 4));
	}
}


void irrl::Game::displayGlobalScene()
{
	if (!_endGame || _draw) {
		_driver->beginScene(true, true, irr::video::SColor(255, 100, 100, 100));
		lock();
		setCameraPos();
		_sceneManager->drawAll();
		unlock();
		_driver->endScene();

	} else if (_endGame && !_draw) {
		endScene();
	}
}


void irrl::Game::endSplitScene()
{
	_currentScene = "winner";
	_driver->setViewPort(irr::core::rect<irr::s32>(0, 0, 1600, 900));
	_driver->beginScene(true,true,irr::video::SColor(255,100,100,100));

	for (const auto &it : _listPlayer) {
		if (it.alive) {
			if (_camera[PLAYER1]->getPosition().Y < _camera[PLAYER1]->getTarget().Y + 2) {
				displayBothPlayers();
			} else {
				if (!getNode(it.entity))
					continue;
				const irr::core::vector3df winnerPos = getNode(it.entity)->getPosition();
				_camera[PLAYER1]->setTarget(winnerPos);
				_camera[PLAYER2]->setTarget(winnerPos);
				const irr::core::vector3df tmp(_camera[PLAYER1]->getPosition().X, static_cast<irr::f32>(_camera[PLAYER1]->getPosition().Y - 0.1), _camera[PLAYER1]->getPosition().Z);
				_camera[PLAYER1]->setPosition(tmp);
				_camera[PLAYER2]->setPosition(tmp);
				displayBothPlayers();
			}
			break;
		}
	}
}

void irrl::Game::endScene()
{
	_currentScene = "winner";
	for (const auto &it : _listPlayer) {
		if (it.alive) {
			if (_camera[GLOBAL]->getPosition().Y < _camera[GLOBAL]->getTarget().Y + 2) {
				_driver->beginScene(true, true, irr::video::SColor(255, 100, 100, 100));
				lock();

				//initWinner();
				_sceneManager->drawAll();
				//drawGUI();
				unlock();
				_driver->endScene();
			} else {
				if (!getNode(it.entity))
					continue;
				const irr::core::vector3df winnerPos = getNode(it.entity)->getPosition();

				if (_winPLayer) {
					_camera[GLOBAL]->setPosition(irr::core::vector3df(winnerPos.X, _camera[GLOBAL]->getPosition().Y, winnerPos.Z - 1));
					_camera[GLOBAL]->setTarget(winnerPos);
					_winPLayer = false;
				} else {
					const irr::core::vector3df tmp(_camera[GLOBAL]->getPosition().X, static_cast<irr::f32>(_camera[GLOBAL]->getPosition().Y - 0.1), _camera[GLOBAL]->getPosition().Z);
					_camera[GLOBAL]->setPosition(tmp);
				}
				_driver->beginScene(true, true, irr::video::SColor(255, 100, 100, 100));
				lock();
				_sceneManager->drawAll();
				unlock();
				//std::cout << "Ma camera est en " << _camera[GLOBAL]->getPosition().Y << " et mon target est en " << _camera[GLOBAL]->getTarget().Y << std::endl;
				_driver->endScene();
			}
		}
	}
}


void irrl::Game::displaySplitScreenScene()
{
	checkLastAlive();
	if (!_endGame || _draw) {
		displaySplitScreen();
	}
	else
		endSplitScene();
}

void irrl::Game::displayBothPlayers()
{
	/* Display of Player 1 */
	_sceneManager->setActiveCamera(_camera[PLAYER1]);
	_driver->setViewPort(irr::core::rect<irr::s32>(0, 0, 1600, 900 / 2));
	lock();
	_sceneManager->drawAll();
	unlock();
	/* Display of Player 2 */
	_sceneManager->setActiveCamera(_camera[PLAYER2]);
	_driver->setViewPort(irr::core::rect<irr::s32>(0, 450, 1600, 900));
	lock();
	_sceneManager->drawAll();
	unlock();
	_driver->endScene();
}


void irrl::Game::displaySplitScreen()
{
	_driver->setViewPort(irr::core::rect<irr::s32>(0, 0, 1600, 900));
	_driver->beginScene(true,true,irr::video::SColor(255,100,100,100));
	int i = 0;
	/* Setting Camera pos to player's position */
	for (auto &it : _listPlayer) {
		if (it.alive) {
			if (getNode(it.entity) == nullptr)
				continue;
			_camera[i]->setPosition(irr::core::vector3df(getNode(it.entity)->getPosition().X, static_cast<irr::f32>(getMapSize().X / 1.4), getNode(it.entity)->getPosition().Z));
			_camera[i]->setTarget(irr::core::vector3df(getNode(it.entity)->getPosition().X, 0, getNode(it.entity)->getPosition().Z + 3));
			i++;
		}
		if (i == 2)
			break;
	}
	displayBothPlayers();
}

void irrl::Game::manageEventPlayers()
{
	for (auto &it : _listPlayer) {
		bool doSomething = false;
		if (!dynamic_cast<is::AEntity *>(it.entity))
			continue;
		auto tmp = dynamic_cast<is::ArtificialIntelligence *>(it.entity);
		if (tmp) {
			_eventManager.lock();
			_eventManager->enqueue([tmp]{tmp->AIsTurn();});
			_eventManager.unlock();
		}
		else {
			for (int i = 0; it.key[i].f != nullptr ; ++i) {
				if (_eventReceiver.IsKeyDown(it.key[i].key)) {
					_eventManager.lock();
					_eventManager->enqueue(it.key[i].f);
					_eventManager.unlock();
					doSomething = true;
				}
			}
			if (!doSomething) {
				_eventManager.lock();
				if (it.nothing.f) {
					_eventManager->enqueue(it.nothing.f);
				}
				_eventManager.unlock();
			}
		}
	}
}

irr::scene::ISceneNode *irrl::Game::getNode(
	is::IEntity *entity
)
{
	if (_listObj.find(entity) != _listObj.end())
		return _listObj[entity].obj;
	return nullptr;
}

float &irrl::Game::getNodeSize(
	const std::shared_ptr<is::IEntity> &entity
)
{
	return _listObj[entity.get()].size;
}

bool irrl::Game::addEntity(std::shared_ptr<is::IEntity> &entity,
	irr::scene::ISceneNode *obj, float size
)
{
	auto tmp = dynamic_cast<is::ACharacter *>(entity.get());

	if (tmp != nullptr && tmp->getType() == "Character") {
		player_t player;
		if (_listPlayer.empty())
			player = {(tmp),
				{irr::KEY_ESCAPE, [tmp]() {tmp->doNothing();}},
				{{irr::KEY_LEFT, [tmp]() {tmp->moveLeft();}},
				{irr::KEY_RIGHT, [tmp]() {tmp->moveRight();}},
				{irr::KEY_UP, [tmp]() {tmp->moveUp();}},
				{irr::KEY_DOWN, [tmp]() {tmp->moveDown();}},
				{irr::KEY_RETURN, [tmp]() {tmp->dropBomb();}},
				{irr::KEY_ESCAPE, nullptr}}};
		else
			player = {(tmp),
				{irr::KEY_ESCAPE, [tmp]() {tmp->doNothing();}},
				{{irr::KEY_KEY_Q, [tmp]() {tmp->moveLeft();}},
				{irr::KEY_KEY_D, [tmp]() {tmp->moveRight();}},
				{irr::KEY_KEY_Z, [tmp]() {tmp->moveUp();}},
				{irr::KEY_KEY_S, [tmp]() {tmp->moveDown();}},
				{irr::KEY_SPACE, [tmp]() {tmp->dropBomb();}},
				{irr::KEY_ESCAPE, nullptr}}};
		player.alive = true;
		_listPlayer.push_back(player);
	}
	_listObj[entity.get()] = {obj, size};
	return false;
}

bool irrl::Game::deleteEntity(std::shared_ptr<is::IEntity> &entity)
{
	std::cout << "je dlete mon entité" << std::endl;
	int i = 0;
	if (entity->getType() == "Character") {
		for (auto &it : _listPlayer) {
			if (it.entity == entity.get()) {
				std::cout << "je met alive a false" << std::endl;
				it.alive = false;
			}
			if (it.alive)
				i++;
		}
		std::cout << "l'entité que je delete est un joueur" << std::endl;
		if (i == 0) {
			_endGame = false;
			_draw = true;
		}
	}
	lock();
	irrl::irrObj_t tmp_obj = _listObj[entity.get()];
	auto tmp_find = _listObj.find(entity.get());
	if (_device && tmp_find != _listObj.end() && tmp_obj.obj) {
		tmp_obj.obj->setVisible(false);
		_listObj.erase(tmp_find);
	}
	checkLastAlive();
	unlock();
	return false;
}

void irrl::Game::setMapSize(const irr::core::vector2di &mapSize)
{
	_mapSize.first = (std::size_t)mapSize.X;
	_mapSize.second = (std::size_t)mapSize.Y;
}

irr::core::vector2di irrl::Game::getMapSize() const
{
	return irr::core::vector2di(irr::s32(_mapSize.first), irr::s32(_mapSize.second));
}

void irrl::Game::setCameraPos()
{
	if (_listPlayer.empty() || _endGame || _draw)
		return;
	int i = 0;
	irr::f32 tmpX = 0;
	irr::f32 tmpY = 0;
	for (auto &it : _listPlayer) {
		if (!it.alive || !getNode(it.entity))
			continue;
		if (!it.alive || !getNode(it.entity))
			continue;
		tmpX = getNode(it.entity)->getPosition().X;
		tmpY = getNode(it.entity)->getPosition().Z;
		if (i == 0) {
			_distBetweenPlayer[NEAREST].X = tmpX;
			_distBetweenPlayer[FAREST].X = tmpX;

			_distBetweenPlayer[NEAREST].Y = tmpY;
			_distBetweenPlayer[FAREST].Y = tmpY;
		} else {
			if (tmpX < _distBetweenPlayer[NEAREST].X)
				_distBetweenPlayer[NEAREST].X = tmpX;
			if (tmpX > _distBetweenPlayer[FAREST].X)
				_distBetweenPlayer[FAREST].X = tmpX;

			if (tmpY < _distBetweenPlayer[NEAREST].Y)
				_distBetweenPlayer[NEAREST].Y = tmpY;
			if (tmpY > _distBetweenPlayer[FAREST].Y)
				_distBetweenPlayer[FAREST].Y = tmpY;
		}
		i++;
	}
	_distBetweenPlayer[FAREST].Y += 2;

	_camera[GLOBAL]->setPosition(irr::core::vector3df(((_distBetweenPlayer[NEAREST].X  + _distBetweenPlayer[FAREST].X) / 2), _camera[GLOBAL]->getPosition().Y, ((_distBetweenPlayer[NEAREST].Y  + _distBetweenPlayer[FAREST].Y) / 2) - 2));
	_camera[GLOBAL]->setTarget(irr::core::vector3df(((_distBetweenPlayer[NEAREST].X + _distBetweenPlayer[FAREST].X) / 2), 0, ((_distBetweenPlayer[NEAREST].Y  + _distBetweenPlayer[FAREST].Y) / 2) - 1));

	//std::cout << "Total PLayer "<< totalPLayer << " alive player " << alivePLayer << std::endl;
}

void irrl::Game::resetListObj()
{
	_entities->clear();
	_listPlayer.clear();
	_listObj.clear();
}

void irrl::Game::displayFPS()
{
	int lastFPS = -1;
	wchar_t tmp[1024];
	int fps = _driver->getFPS();

	if (lastFPS != fps) {
		swprintf(tmp, 1024, L"GameFreak (%ls)(fps:%d)",
			_driver->getName(), fps);
		_device->setWindowCaption(tmp);
		lastFPS = fps;
	}
}

void irrl::Game::lock()
{
	_mutex.lock();
}

void irrl::Game::unlock()
{
	_mutex.unlock();
}

void irrl::Game::setPause()
{
	_entities.lock();
	std::cout << "START PAUSE " << std::endl;
	for (auto &it : _listObj) {
		auto tmp = dynamic_cast<is::AEntity *>(it.first);
		if (tmp && it.first->getType() == "Bomb") {
			auto tmp = static_cast<is::Bomb *>(it.first);
			tmp->setPaused(true);
		}
	}
	_entities.unlock();
}

void irrl::Game::endPause()
{
	_entities.lock();
	for (auto &it : _listObj) {
		auto tmp = dynamic_cast<is::AEntity *>(it.first);
		if (tmp && it.first->getType() == "Bomb") {
			auto tmp = static_cast<is::Bomb *>(it.first);
			tmp->setPaused(false);
			it.first->unlock();
		}
	}
	_entities.unlock();
}

void irrl::Game::checkLastAlive()
{
	int totalPLayer = 0;
	int alivePLayer = 0;
	for (auto &it : _listPlayer) {
		totalPLayer++;
		if (!it.alive || !getNode(it.entity))
			continue;
		alivePLayer++;
	}
	if ((alivePLayer == 1 || totalPLayer == 1) && !_endGame) {
		_endGame = true;
		for (auto &it : _listPlayer) {
			if (it.alive)
				it.entity->setHP(10);
		}
	}
}

void irrl::Game::setFloor()
{
	/* Création floor */

	std::cout << getSceneManager()->getRegisteredSceneNodeFactoryCount() << std::endl;
	irr::core::dimension2d<irr::f32> tileSize(1.0, 1.0); // taille dun bloc
	irr::core::dimension2d<irr::u32> tileCount(1, 1); // taille de la map

	auto material = new irr::video::SMaterial();
	material->MaterialType = irr::video::E_MATERIAL_TYPE::EMT_SOLID;
	material->Wireframe = false;
	material->Lighting = false;
	irr::core::dimension2d<irr::f32> textureRepeatCount(1.0, 1.0);
	irr::scene::IMesh *cube = getSceneManager()->getGeometryCreator()->createPlaneMesh(tileSize, tileCount, material, textureRepeatCount);

	cube->setMaterialFlag(irr::video::EMF_WIREFRAME, false);
	irr::video::ITexture *texture = getDriver()->getTexture(irr::io::path("media/floor.png"));

	unsigned int i = 1;
	unsigned int j = 1;

	while (j < getMapSize().Y + 1) {
		std::cout << "i " << i << " mapsize.x " << getMapSize().X << " j " << j << " mapsize.y " << getMapSize().Y << std::endl;
		irr::scene::IMeshSceneNode *cubeNode = getSceneManager()->addMeshSceneNode(cube);
		cubeNode->setMaterialTexture(0, texture);
		cubeNode->setPosition(irr::core::vector3df(j, -0.5f, i));
		//cubeNode->setScale(irr::core::vector3df(1, 1, 1));
		cubeNode->setMaterialFlag(irr::video::EMF_WIREFRAME, false);
		cubeNode->setMaterialFlag(irr::video::EMF_LIGHTING, false);
		i++;
		if (i > getMapSize().X && j != getMapSize().Y + 1) {
			i = 1;
			j++;
		}
	}
}
