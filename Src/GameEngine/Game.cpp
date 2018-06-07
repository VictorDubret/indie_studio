/*
** EPITECH PROJECT, 2018
** bomberman
** File description:
** Created by martin.januario@epitech.eu,
*/

#include "Game.hpp"
nts::Game::Game(
	my::ItemLocker<std::vector<std::shared_ptr<is::IEntity>>> &entities,
	my::ItemLocker<my::ThreadPool> &eventManager,
	irr::core::vector2di mapSize, bool splitScreen
) : AManageIrrlicht(entities, eventManager), _mapSize(mapSize), _splitScreen(splitScreen)
{
	irr::core::vector2df tmpDist;
	tmpDist.X = -(_mapSize.X / 2);
	tmpDist.Y = 0;
	_distBetweenPlayer.push_back(tmpDist);
	tmpDist.X = _mapSize.X / 2;
	_distBetweenPlayer.push_back(tmpDist);
	_camera[GLOBAL] = _sceneManager->addCameraSceneNode(0,
		irr::core::vector3df(getMapSize().X / 2 + 1, static_cast<irr::f32>(getMapSize().X / 1.1), getMapSize().Y / 2),
		irr::core::vector3df(getMapSize().X / 2 + 1, 0, getMapSize().Y / 2 + 1));
	/* Split Screen Camera */
	if (_splitScreen) {
		_camera[PLAYER1] = _sceneManager->addCameraSceneNode(0, irr::core::vector3df(getMapSize().X / 2 + 1, (getMapSize().X / 2), -3),	irr::core::vector3df(getMapSize().X / 2 + 1, getMapSize().X / 10, getMapSize().X / 4));
		_camera[PLAYER2] = _sceneManager->addCameraSceneNode(0, irr::core::vector3df(getMapSize().X / 2 + 1, (getMapSize().X / 2), -3),	irr::core::vector3df(getMapSize().X / 2 + 1, getMapSize().X / 10, getMapSize().X / 4));
	}
	//_sceneManager->addCameraSceneNode(0, irr::core::vector3df(0, 15 , -10), irr::core::vector3df(0, 10, 0));
	_engine->play2D("media/AMemoryAway.ogg", true, false, true,
		irrklang::ESM_AUTO_DETECT, true);
	//_sceneManager->addCameraSceneNode(0, irr::core::vector3df(0, (getMapSize().X / 2), -10), irr::core::vector3df(0, getMapSize().X / 10, 0));
	//	_sceneManager->addCameraSceneNode(0, irr::core::vector3df(0, (getMapSize().X / 2), -10), irr::core::vector3df(90, getMapSize().X / 10, 90));
	//_sceneManager->getActiveCamera()->setPosition(irr::core::vector3df(30 , 20, 30));
}

nts::Game::~Game()
{
	_stopThread = true;
	delete _device;
	delete _engine;
	delete _thread;
}

void nts::Game::updateView()
{
}

void nts::Game::displaySplitScreen()
{
	std::cout << "jaffiche la map" << RESET << std::endl;
	_driver->setViewPort(irr::core::rect<irr::s32>(0, 0, 1600, 900));
	//_driver->beginScene(true, true, irr::video::SColor(0, 220, 220, 220));
	_driver->beginScene(true, true, irr::video::SColor(255, 100, 100, 100));
	// test split screen
	_sceneManager->setActiveCamera(_camera[PLAYER1]);
	_driver->setViewPort(irr::core::rect<irr::s32>(0, 0, 1600, 900 / 2));
	_sceneManager->drawAll();

	_sceneManager->setActiveCamera(_camera[PLAYER2]);
	_driver->setViewPort(irr::core::rect<irr::s32>(0, 450, 1600, 900));
	_sceneManager->drawAll();

	int i = 0;
	for (auto &it : _listPlayer) {
		std::cout << "Joueur[" << i << "] X["
			<< getNode(it.entity)->getPosition().X << "] Y["
			<< getNode(it.entity)->getPosition().Y << "] Z["
			<< getNode(it.entity)->getPosition().Z << "]" << RESET
			<< std::endl;

		_camera[i]->setPosition(irr::core::vector3df(
			getNode(it.entity)->getPosition().X,
			(getMapSize().X / 2),
			getNode(it.entity)->getPosition().Z - 5));
		_camera[i]->setTarget(irr::core::vector3df(
			getNode(it.entity)->getPosition().X,
			getMapSize().X / 10,
			getNode(it.entity)->getPosition().Z));
		i++;
	}
}

void nts::Game::manageEventPlayers()
{
	for (auto &it : _listPlayer) {
		bool doSomething = false;
		if (!dynamic_cast<is::AEntity *>(it.entity))
			return;

		for (int i = 0; it.key[i].f != nullptr ; ++i) {
			if (_eventReceiver.IsKeyDown(it.key[i].key)) {
				_eventManager.lock();
				_eventManager->enqueue(it.key[i].f);
				_eventManager.unlock();
				doSomething = true;
			}
		}
		if (_eventReceiver.IsKeyDown(it.doSomething.key)) {
			_eventManager.lock();
			_eventManager->enqueue(it.doSomething.f);
			_eventManager.unlock();
		} else if (!doSomething) {
			_eventManager.lock();
			_eventManager->enqueue(it.nothing.f);
			_eventManager.unlock();
		}
	}
}

irr::scene::ISceneNode *nts::Game::getNode(
	is::IEntity *entity
)
{
	return _listObj[entity].obj;
}

float &nts::Game::getNodeSize(
	const std::shared_ptr<is::IEntity> &entity
)
{
	return _listObj[entity.get()].size;
}

bool nts::Game::addEntity(std::shared_ptr<is::IEntity> &entity,
	irr::scene::ISceneNode *obj, float size
)
{
	auto tmp = dynamic_cast<is::ACharacter *>(entity.get());

	if (tmp != nullptr && tmp->getType() == "Character") {
		player_t player;
		if (_listPlayer.empty())
			player = {(tmp),
				{irr::KEY_ESCAPE, [tmp]() {
					tmp->doNothing();
				}}, {irr::KEY_RETURN, [tmp]() {
					tmp->dropBomb();
				}}, {{irr::KEY_LEFT, [tmp]() {
					tmp->moveLeft();
				}}, {irr::KEY_RIGHT, [tmp]() {
					tmp->moveRight();
				}}, {irr::KEY_UP, [tmp]() {
					tmp->moveUp();
				}}, {irr::KEY_DOWN, [tmp]() {
					tmp->moveDown();
				}}, {irr::KEY_ESCAPE, nullptr}}};
		else
			player = {(tmp),
				{irr::KEY_ESCAPE, [tmp]() {
					tmp->doNothing();
				}}, {irr::KEY_SPACE, [tmp]() {
					tmp->dropBomb();
				}}, {{irr::KEY_KEY_Q, [tmp]() {
					tmp->moveLeft();
				}}, {irr::KEY_KEY_D, [tmp]() {
					tmp->moveRight();
				}}, {irr::KEY_KEY_Z, [tmp]() {
					tmp->moveUp();
				}}, {irr::KEY_KEY_S, [tmp]() {
					tmp->moveDown();
				}}, {irr::KEY_ESCAPE, nullptr}}};
		_listPlayer.push_back(player);
	}
	_listObj[entity.get()] = {obj, size};
	return false;
}

bool nts::Game::deleteEntity(std::shared_ptr<is::IEntity> &entity)
{
	lock();
	/*if (entity->getType() == "Character") {
		int idx = 0;
		for (auto &it : _listPlayer) {
			if (it.entity == entity) {
				_listPlayer.erase(_listPlayer.begin() + idx);
				return false;
			}
			idx++;
		}
	}*/
	//if (entity->getType() == "Character") {

	//	unlock();
	//	return false;
	//}
	nts::irrObj_t tmp_obj = _listObj[entity.get()];
	auto tmp_find = _listObj.find(entity.get());
	if (_device && tmp_find != _listObj.end() && tmp_obj.obj) {
		tmp_obj.obj->setVisible(false);
		_listObj.erase(tmp_find);
	}
	unlock();
	return false;
}

void nts::Game::setMapSize(const irr::core::vector2di &mapSize)
{
	_mapSize = mapSize;
}

irr::core::vector2di nts::Game::getMapSize() const
{
	return _mapSize;
}

void nts::Game::setCameraPos()
{
	int i = 0;
	for (auto &it : _listPlayer) {
		if (getNode(it.entity)->getPosition().X <
			_distBetweenPlayer[NEAREST].X)
			_distBetweenPlayer[NEAREST].X = getNode(
				it.entity)->getPosition().X;
		else if (getNode(it.entity)->getPosition().X >
			_distBetweenPlayer[FAREST].X)
			_distBetweenPlayer[FAREST].X = getNode(
				it.entity)->getPosition().X;
		//std::cout << "Map[NEAREST].x [" << _distBetweenPlayer[NEAREST].X << "]" << "Map[FAREST].X [" << _distBetweenPlayer[FAREST].X << "]" << RESET << std::endl;
		//std::cout << "Joueur[" << i << "] X["  << getNode(it.entity)->getPosition().X << "] Y[" << getNode(it.entity)->getPosition().Y << "] Z[" << getNode(it.entity)->getPosition().Z << "]" << RESET << std::endl;
		i++;
	}
	_sceneManager->getActiveCamera()->setPosition(
		irr::core::vector3df(getMapSize().X / 2 + 1,
			static_cast<irr::f32>(
				(_distBetweenPlayer[NEAREST].X * -1 +
					_distBetweenPlayer[FAREST].X) / 2),
			-3));
	_distBetweenPlayer[NEAREST].X = -_mapSize.X;
	_distBetweenPlayer[FAREST].X = _mapSize.X;
}

void nts::Game::displayFPS()
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

void nts::Game::lock()
{
	_mutex.lock();
}

void nts::Game::unlock()
{
	_mutex.unlock();
}

