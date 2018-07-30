/*
 * jetPackWidget.cpp
 *
 *  Created on: 30 Jul 2018
 *      Author: linbob
 */

#include "jetPackWidget.hpp"

JetPackWidget::JetPackWidget(tgui::Theme::Ptr theme, anax::Entity p, float screenW,
		float screenH) : player(p){
	float w = 102, h = 22;

	outline = sf::RectangleShape(sf::Vector2f(w, h));
	outline.setFillColor(sf::Color::Transparent);
	outline.setPosition((screenW * 0.642), h * 4.8);
	outline.setOutlineColor(sf::Color::Blue);
	outline.setOutlineThickness(2);

	chargeBar = sf::RectangleShape(sf::Vector2f(w, h));
	chargeBar.setFillColor(sf::Color::Green);
	chargeBar.setPosition((screenW * 0.642), h * 4.8);
}

void JetPackWidget::update(sf::Time elapsedTime){
	JetPackComponent& j = player.getComponent<JetPackComponent>();

	chargeBar.setScale(j.getCharge() , 1.0);
}

void JetPackWidget::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	target.draw(chargeBar, states);
	target.draw(outline, states);
}


