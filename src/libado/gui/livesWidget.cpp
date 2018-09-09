/*
 * livesWidget.cpp
 *
 *  Created on: 14 Jul 2018
 *      Author: linbob
 */

#include "livesWidget.hpp"

LivesWidget::LivesWidget(anax::Entity p, float screenW,	float screenH) : player(p){
	LivesComponent& l = player.getComponent<LivesComponent>();
	currentCount = l.getLivesCounter();

	float w = 18, h = 24;
	for(int i = 0; i < l.getLivesCounter(); ++i){
		sf::RectangleShape rect(sf::Vector2f(w, h));
		rect.setFillColor(sf::Color::Red);
		rect.setPosition((screenW * 0.642) + (w * 2.2) * i, h * 2.8);
		rect.setOutlineColor(sf::Color::White);
		rect.setOutlineThickness(1);

		shapes.push_back(rect);
	}
}

void LivesWidget::init(sf::RenderWindow* w){}

void LivesWidget::update(sf::Time elapsedTime){
	LivesComponent& l = player.getComponent<LivesComponent>();

	if(l.getLivesCounter() < currentCount){
		shapes[currentCount - 1].setFillColor(sf::Color::Transparent);
		--currentCount;
	}
}

void LivesWidget::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	for(auto& s : shapes){
		target.draw(s, states);
	}
}
