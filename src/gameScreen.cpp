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

	status = RUNNING;
	entityWorld = std::make_shared<anax::World>();
	renderSys = std::make_shared<RenderingSystem>(manager->getWindow());
	entityWorld->addSystem(*renderSys);
	entFactory = std::make_shared<EntityFactory>("bin/data/entityData.json", entityWorld.get());

	tileMap = std::make_shared<TileMap>("bin/data/tileData.json", 162, 72);

	physicsSys = std::make_shared<PhysicsSystem>(tileMap.get());
	entityWorld->addSystem(*physicsSys);
	ropeSys = std::make_shared<NinjaRopeSystem>(tileMap.get());
	entityWorld->addSystem(*ropeSys);
	animSys = std::make_shared<AnimationSystem>();
	entityWorld->addSystem(*animSys);
	particleSys = std::make_shared<ParticleSystem>(entFactory.get());
	entityWorld->addSystem(*particleSys);

//	mapView = std::unique_ptr<WorldView>(new WorldView(manager->getWindow()->getSize().x,
//			manager->getWindow()->getSize().y, input.get()));

	/////////////////////////////////////////////////////////////

	MazeGen::getInstance()->generateMaze(tileMap.get());

	anax::Entity e = entFactory->createEntity("player");
	e.getComponent<PositionComponent>().setPosition(TILESIZE * 1.5, TILESIZE * 2);

	mapView = std::unique_ptr<WorldView>(new WorldView(manager->getWindow()->getSize().x,
				manager->getWindow()->getSize().y, entFactory->getPlayer()));

	manager->getWindow()->setView(*mapView->getView());

	controller = std::make_shared<PlayerController>(entFactory.get(),
			manager->getWindow(), mapView->getView());
	entityWorld->addSystem(*controller);

	checkpoints = CheckpointUtil::createCheckPoints(3, tileMap.get(), entFactory.get());

	/////////////////////////////////////////////////////////////

	//GUI STUFF
	auto windowWidth = tgui::bindWidth(*manager->getGui());
	auto windowHeight = tgui::bindHeight(*manager->getGui());

	auto minimap = std::make_shared<MiniMap>(manager->getTheme(), this);
	minimap->init(manager->getWindow());
	manager->getGui()->add(minimap, "minimap");

	/////////////////////////////////////////////////////////////

	input->getMap()["physics_debug"] = thor::Action(sf::Keyboard::Key::F5,
			thor::Action::PressOnce);

	input->getActionSys().connect("physics_debug", std::bind([this](){
		if(input->isGuiFocused()) return false;

		ropeSys->isDrawingDebug = !ropeSys->isDrawingDebug;

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
	// run the program as long as the window is open
	while(status == RUNNING){
		sf::Time deltaTime = deltaClock.restart();

		handleInput(deltaTime);

		update(deltaTime);

		render(deltaTime);
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
	ropeSys->update(delta);
	physicsSys->update(delta);
	particleSys->update(delta);
	animSys->update(delta);

	mapView->update();

	manager->getGui()->updateTime(delta);
}
void GameScreen::render(sf::Time& delta){
	manager->getWindow()->clear(sf::Color::Black);

	manager->getWindow()->setView(*mapView->getView());
	tileMap->render(manager->getWindow());
	renderSys->render(delta);

	ropeSys->drawDebug(manager->getWindow());

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
