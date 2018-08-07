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

	chargeOutline = sf::RectangleShape(sf::Vector2f(w, h));
	chargeOutline.setFillColor(sf::Color::Transparent);
	chargeOutline.setPosition((screenW * 0.642), h * 4.8);
	chargeOutline.setOutlineColor(sf::Color::Blue);
	chargeOutline.setOutlineThickness(2);

	chargeBar = sf::RectangleShape(sf::Vector2f(w, h));
	chargeBar.setFillColor(sf::Color::Green);
	chargeBar.setPosition((screenW * 0.642), h * 4.8);

	JetPackComponent& jet = p.getComponent<JetPackComponent>();
	currentCount = jet.getTurboCharges();

	w = 18, h = 24;
	for(int i = 0; i < jet.getMaxTurboCharges(); ++i){
		sf::RectangleShape rect(sf::Vector2f(w, h));
		if(i <= jet.getTurboCharges() - 1){
			rect.setFillColor(sf::Color::White);
		}else{
			rect.setFillColor(sf::Color::Transparent);
		}
		rect.setPosition((screenW * 0.642) + (w * 2.2) * i, h * 5.8);
		rect.setOutlineColor(sf::Color::White);
		rect.setOutlineThickness(1);

		turboWidgets.push_back(rect);
	}
}

void JetPackWidget::update(sf::Time elapsedTime){
	JetPackComponent& j = player.getComponent<JetPackComponent>();

	chargeBar.setScale(j.getCharge() , 1.0);

	if(j.getTurboCharges() < currentCount){
		turboWidgets[currentCount - 1].setFillColor(sf::Color::Transparent);
		--currentCount;
	}
}

void JetPackWidget::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	target.draw(chargeBar, states);
	target.draw(chargeOutline, states);

	for(auto& w : turboWidgets){
		target.draw(w, states);
	}
}


