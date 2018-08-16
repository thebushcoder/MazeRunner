/*
 * worldView.hpp
 *
 *  Created on: 11 Feb 2016
 *      Author: linbob
 */

#ifndef WORLDVIEW_HPP_
#define WORLDVIEW_HPP_

#define CAMERAVELOCITY 1500

#include <memory>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <anax/Entity.hpp>

#include "input.hpp"
#include "entity/components/positionComponent.hpp"
//#include "map/zone.hpp"

class Zone;

class WorldView{
public:
	std::unique_ptr<sf::View> view;

	WorldView(){}
	WorldView(int w, int h, anax::Entity p);
	WorldView(int w, int h, ThorInput* input);
	void update();
	void resize(int w, int h);
	sf::View* getView();

	WorldView& operator = (WorldView& other);

private:
	anax::Entity player;
	bool hasPlayer = false;
};

#endif /* WORLDVIEW_HPP_ */
