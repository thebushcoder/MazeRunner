/*
 * checkpointWidget.cpp
 *
 *  Created on: 1 Jul 2018
 *      Author: linbob
 */

#include "checkpointWidget.hpp"
#include "../../gameScreen.hpp"

CheckpointWidget::CheckpointWidget(tgui::Theme::Ptr theme, GameScreen* s, float screenW, float screenH, int num){
	screen = s;
	float rad = 14;
	for(int i = 0; i < num; ++i){
		sf::CircleShape cp(rad, 3);
		cp.setRotation(180);
		cp.setFillColor(sf::Color::Transparent);
		cp.setPosition((screenW * 0.66) + (rad * 2.8) * i, rad * 2.8);
		cp.setOutlineColor(sf::Color::White);
		cp.setOutlineThickness(1);

		shapes.push_back(cp);
	}
	fill = sf::Color(0, 0, 255);
}

void CheckpointWidget::init(sf::RenderWindow* w){

}

void CheckpointWidget::update(sf::Time elapsedTime){
	int i = 0;

	for(auto& e : screen->getCheckpoints()){
		if(!e.isValid()){
			++i;
		}
	}

	if(i > collected){
		shapes[i - 1].setFillColor(fill);
		shapes[i - 1].setOutlineColor(fill);
		++collected;
	}
}

void CheckpointWidget::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	for(auto& s : shapes){
		target.draw(s, states);
	}
}
