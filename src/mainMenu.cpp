/*
 * mainMenu.cpp
 *
 *  Created on: 19 Feb 2016
 *      Author: linbob
 */

#include "mainMenu.hpp"

void MainMenu::init(){
	status = RUNNING;

	createMainControls();
}
void MainMenu::createMainControls(){
	auto windowWidth = tgui::bindWidth(*manager->getGui());
	auto windowHeight = tgui::bindHeight(*manager->getGui());

	auto mainLayout = std::make_shared<tgui::VerticalLayout>();
	mainLayout->setSize(tgui::Layout2d(400,
			manager->getWindow()->getSize().y * 0.4));
	mainLayout->setPosition(windowWidth * 0.35,	windowHeight * 0.3);

	tgui::Button::Ptr button = tgui::Button::create("Start");
	button->setRenderer(manager->getTheme().getRenderer("Button"));

	button->connect("pressed", [this]{
		status = QUIT;
		Screen* s = new GameScreen();
		s->setManager(manager);
		manager->setCurrentScreen(s);
//		manager->getGui()->get("main")->hide();
//		manager->getGui()->get("game")->show();
	});

	// Add the Button to the Box
	mainLayout->add(button);
	mainLayout->addSpace(0.2f);

	button = tgui::Button::create("Editor");
	button->setRenderer(manager->getTheme().getRenderer("Button"));

	button->connect("pressed", [this]{
//		status = QUIT;
//		Screen* s = new EditorScreen();
//		s->setManager(manager);
//		manager->setCurrentScreen(s);
	});
	// Add the Button to the Box
	mainLayout->add(button);
	mainLayout->addSpace(0.2f);

	button = tgui::Button::create("Options");
	button->setRenderer(manager->getTheme().getRenderer("Button"));
	button->connect("pressed", [this]{
//		status = QUIT;
//		Screen* s = new VoronoiScreen();
//		s->setManager(manager);
//		manager->setCurrentScreen(s);
	});
	// Add the Button to the Box
	mainLayout->add(button);
	mainLayout->addSpace(0.2f);

	button = tgui::Button::create("Exit");
	button->setRenderer(manager->getTheme().getRenderer("Button"));
	// Add the Button to the Box
	mainLayout->add(button);
	button->connect("pressed", [this]{
		status = QUIT;
	});
	manager->getGui()->add(mainLayout, "main");
}

int MainMenu::run(){
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

		render();
	}

	return QUIT;
}
void MainMenu::update(sf::Time& delta){
	manager->getGui()->updateTime(delta);
}
void MainMenu::render(){
	manager->getWindow()->clear(sf::Color::Black);

	manager->getGui()->draw();

	// end the current frame
	manager->getWindow()->display();
}
void MainMenu::handleInput(sf::Time& delta){
	sf::Event event;

	while (manager->getWindow()->pollEvent(event)){
		// "close requested" event: we close the window
		switch (event.type){
		case sf::Event::Closed:
			status = QUIT;
			getManager()->setCurrentScreen(nullptr);
			break;
		case sf::Event::Resized:
			resize(event);
			break;
		default:
			manager->getGui()->handleEvent( event );
			break;
		}
	}
}
void MainMenu::resize(sf::Event& e){
	manager->getWindow()->setView(sf::View(sf::FloatRect(0, 0,
			e.size.width, e.size.height)));

	manager->getGui()->setView(manager->getWindow()->getView());
}
void MainMenu::loadResources(){}
