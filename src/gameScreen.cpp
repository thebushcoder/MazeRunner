/*
 * gameScreen.cpp
 *
 *  Created on: 12 Mar 2018
 *      Author: linbob
 */

#include "main.hpp"
#include "gameScreen.hpp"

void GameScreen::init(){
	loadResources();
	Screen::init();

	//BUMP / TEST!

	status = RUNNING;
	entityWorld = std::make_shared<anax::World>();
	renderSys = std::make_shared<RenderingSystem>(manager->getWindow());
	entityWorld->addSystem(*renderSys);
	entFactory = std::make_shared<EntityFactory>("bin/data/entityData.json", entityWorld.get());

	tileMap = std::make_shared<TileMap>(entFactory.get(), "bin/data/tileData.json", 162, 72);

	physicsSys = std::make_shared<PhysicsSystem>(tileMap.get());
	entityWorld->addSystem(*physicsSys);
	animSys = std::make_shared<AnimationSystem>();
	entityWorld->addSystem(*animSys);
	particleSys = std::make_shared<ParticleSystem>(entFactory.get());
	entityWorld->addSystem(*particleSys);
	collisionSys = std::make_shared<CollisionSystem>(tileMap.get(), this);
	entityWorld->addSystem(*collisionSys);
	invulnerabilitySys = std::make_shared<InvulnerabilitySystem>();
	entityWorld->addSystem(*invulnerabilitySys);

	/////////////////////////////////////////////////////////////

	MazeGen::getInstance()->generateMaze(tileMap.get());

	anax::Entity e = entFactory->createEntity("player");
	float pX = TILESIZE * 1.5, pY = TILESIZE * 2;
	int tileX = std::floor(pX / TILESIZE), tileY = std::floor(pY / TILESIZE);

	e.getComponent<PositionComponent>().setPosition(pX, pY);
	tileMap->getEntityLayer().setEntity(tileX, tileY, e.getId().index);

	mapView = std::unique_ptr<WorldView>(new WorldView(manager->getWindow()->getSize().x,
				manager->getWindow()->getSize().y, entFactory->getPlayer()));

	manager->getWindow()->setView(*mapView->getView());

	controller = std::make_shared<PlayerController>(entFactory.get(),
			manager->getWindow(), mapView->getView());
	entityWorld->addSystem(*controller);

	jetpackSys = std::make_shared<JetPackSystem>(particleSys.get(), manager->getWindow(), mapView->getView());
	entityWorld->addSystem(*jetpackSys);

	int numCheckpoints = 3;
	checkpoints = CheckpointUtil::createCheckPoints(numCheckpoints, tileMap.get(), entFactory.get());

	EnemyUtil::createEnemies(6, tileMap.get(), entFactory.get(), checkpoints);
	/////////////////////////////////////////////////////////////

	//GUI STUFF
	auto windowWidth = tgui::bindWidth(*manager->getGui());
	auto windowHeight = tgui::bindHeight(*manager->getGui());

	auto minimap = std::make_shared<MiniMap>(this);
	minimap->init(manager->getWindow());
	manager->getGui()->add(minimap, "minimap");

	auto cpWidget = std::make_shared<CheckpointWidget>(this,
			manager->getWindow()->getSize().x, manager->getWindow()->getSize().y,
			numCheckpoints);
	manager->getGui()->add(cpWidget, "cpWidget");

	auto livesWidget = std::make_shared<LivesWidget>(
			entFactory->getPlayer(), manager->getWindow()->getSize().x,
			manager->getWindow()->getSize().y);
	manager->getGui()->add(livesWidget, "livesWidget");

	auto jetPackWidget = std::make_shared<JetPackWidget>(
			entFactory->getPlayer(), manager->getWindow()->getSize().x,
			manager->getWindow()->getSize().y);
	manager->getGui()->add(jetPackWidget, "jetPackWidget");

	/////////////////////////////////////////////////////////////

	input->getMap()["physics_debug"] = thor::Action(sf::Keyboard::Key::F5,
			thor::Action::PressOnce);

	input->getActionSys().connect("physics_debug", std::bind([this](){
		if(input->isGuiFocused()) return false;

		return true;
	}));

	input->getMap()["particle_debug"] = thor::Action(sf::Keyboard::Key::F4,
			thor::Action::PressOnce);

	input->getActionSys().connect("particle_debug", std::bind([this](){
		if(input->isGuiFocused()) return false;

		PositionComponent& p = entFactory->getPlayer().getComponent<PositionComponent>();

		particleSys->createExplosion(12, p.screenPosition);

		return true;
	}));
}
int GameScreen::run(){
	if(status == UNINIT){
		Screen::run();
		init();
	}else{
		status = RUNNING;
	}

	sf::Clock deltaClock;
	int frameCount = 0;
	// run the program as long as the window is open
	while(status == RUNNING){
		sf::Time deltaTime = deltaClock.restart();

		handleInput(deltaTime);

//		printf("===========|Frame : %d|==========\n\n", frameCount++);

		update(deltaTime);

		render(deltaTime);

//		printf("\n======================\n");
	}

	return QUIT;
}

void GameScreen::load(std::string path){

}
void GameScreen::save(std::string path){

}

void GameScreen::update(sf::Time& delta){
	entityWorld->refresh();

	controller->update(delta);
	physicsSys->update(delta);
	collisionSys->update(delta);
	jetpackSys->update(delta);
	invulnerabilitySys->update(delta);
	particleSys->update(delta);
	animSys->update(delta);

	mapView->update();

	manager->getGui()->updateTime(delta);
}
void GameScreen::render(sf::Time& delta){
	manager->getWindow()->clear(sf::Color(71,71,71));

	manager->getWindow()->setView(*mapView->getView());
	tileMap->render(manager->getWindow());
	renderSys->render(delta);

	manager->getGui()->draw();

	// end the current frame
	manager->getWindow()->display();
}
void GameScreen::handleInput(sf::Time& delta){
	sf::Event event;

	input->setDelta(delta);
	input->getMap().clearEvents();

	while (manager->getWindow()->pollEvent(event)){
		// "close requested" event: we close the window
		switch (event.type){
		case sf::Event::Closed:
			status = QUIT;
			getManager()->setCurrentScreen(nullptr);
			break;
		case sf::Event::Resized:
			resize(event);
			manager->resize(event);
			break;
		default:
			// if the gui doesnt handle the even pass it on to the ThorInput sys
			if(!manager->getGui()->handleEvent( event )){
				input->getMap().pushEvent(event);
			}

			break;
		}
	}
	input->getMap().invokeCallbacks(input->getActionSys(), manager->getWindow());
}

void GameScreen::resize(sf::Event& e){
	manager->getWindow()->setView(sf::View(sf::FloatRect(0, 0,
			e.size.width, e.size.height)));

	manager->getGui()->setView(manager->getWindow()->getView());
}

void GameScreen::loadResources(){
//	atlas = std::make_shared<TextureAtlas>("bin/imgs/", "atlas", 3);

	lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::string, sol::lib::table);

	tgui::Container::Ptr w = getManager()->getGui()->getContainer();
//	lua["gui_root"] = w.get();
//	lua["gameScreen"] = this;
//	lua["print"] = Logging::printToConsole;

	WidgetLuaBind::initBindings(lua);

	lua.script_file("/home/linbob/linbobGame/script.lua");
}
