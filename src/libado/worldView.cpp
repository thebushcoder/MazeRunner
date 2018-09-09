/*
 * WorldView.cpp
 *
 *  Created on: 11 Feb 2016
 *      Author: linbob
 */

#include "worldView.hpp"

WorldView::WorldView(int w, int h, anax::Entity p) : player(p), hasPlayer(true){
	view = std::unique_ptr<sf::View>(new sf::View(sf::FloatRect(0.f, 0.f, w, h)));
}

//for editor screen
WorldView::WorldView(int w, int h, ThorInput* input){
	view = std::unique_ptr<sf::View>(new sf::View(sf::FloatRect(0.f, 0.f, w, h)));

	input->getMap()["moveCam_Left"] = thor::Action(sf::Keyboard::Key::A, thor::Action::Hold) ||
			thor::Action(sf::Keyboard::Key::A, thor::Action::PressOnce);

	input->getActionSys().connect("moveCam_Left", std::bind([this, input](){
		if(input->isGuiFocused()) return false;
		view->move(-CAMERAVELOCITY * input->getDelta().asSeconds(), 0);
		return true;
	}));

	input->getMap()["moveCam_Right"] = thor::Action(sf::Keyboard::Key::D, thor::Action::Hold) ||
			thor::Action(sf::Keyboard::Key::D, thor::Action::PressOnce);

	input->getActionSys().connect("moveCam_Right", std::bind([this, input](){
		if(input->isGuiFocused()) return false;
		view->move(CAMERAVELOCITY * input->getDelta().asSeconds(), 0);
		return true;
	}));

	input->getMap()["moveCam_Up"] = thor::Action(sf::Keyboard::Key::W, thor::Action::Hold) ||
			thor::Action(sf::Keyboard::Key::W, thor::Action::PressOnce);

	input->getActionSys().connect("moveCam_Up", std::bind([this, input](){
		if(input->isGuiFocused()) return false;
		view->move(0, -CAMERAVELOCITY * input->getDelta().asSeconds());
		return true;
	}));

	input->getMap()["moveCam_Down"] = thor::Action(sf::Keyboard::Key::S, thor::Action::Hold) ||
			thor::Action(sf::Keyboard::Key::S, thor::Action::PressOnce);

	input->getActionSys().connect("moveCam_Down", std::bind([this, input](){
		if(input->isGuiFocused()) return false;
		view->move(0, CAMERAVELOCITY * input->getDelta().asSeconds());
		return true;
	}));
}
void WorldView::update(){
	if(hasPlayer){
		PositionComponent& p = player.getComponent<PositionComponent>();
		view->setCenter(p.screenPosition.x, p.screenPosition.y);
	}
}
WorldView& WorldView::operator = (WorldView& other){
	view = std::move(other.view);

	return *this;
}
void WorldView::resize(int w, int h){
	view->setSize(w, h);
}
sf::View* WorldView::getView(){
	return view.get();
}
