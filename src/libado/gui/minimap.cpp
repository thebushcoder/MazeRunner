/*
 * minimap.cpp
 *
 *  Created on: 10 Jun 2018
 *      Author: linbob
 */

#include "minimap.hpp"
#include "../../gameScreen.hpp"

MiniMap::MiniMap(tgui::Theme::Ptr theme, GameScreen* screen) :
	player(screen->getEntityFactory()->getPlayer()), checkpoints(screen->getCheckpoints()),
	map(screen->getTileMap()){

	setBackgroundColor(tgui::Color(0, 0, 204, 198));
//	getRenderer()->setBorderColor(tgui::Color(255, 255, 255));
//	getRenderer()->setBorders(2, 2);
}

void MiniMap::init(sf::RenderWindow* w){
	setSize(8 + map->getWidth() * 2, 8 + map->getHeight() * 2);
	setPosition(w->getSize().x - getSize().x , 0);

    canvas = std::make_shared<tgui::Canvas>(map->getWidth() * 2, map->getHeight() * 2);
    add(canvas, "map_canvas");
    canvas->setPosition(4, 4);
}
void MiniMap::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	tgui::Panel::draw(target, states);
	canvas->clear();
	canvas->draw(sf::Sprite(texture), states);
}

// draw maze and player position
void MiniMap::update(sf::Time elapsedTime){
	texture = sf::Texture();
	texture.create(canvas->getSize().x, canvas->getSize().y);

	sf::Image image = sf::Image();
	image.create(canvas->getSize().x, canvas->getSize().y, sf::Color::Black);

	for(int x = 0; x < map->getWidth(); ++x){
		for(int y = 0; y < map->getHeight(); ++y){
			Tile::Type t = map->getCost(x, y);

			if(t == Tile::Type::SOLID){
				int mX = (x == 0 ? x : x * 2);
				int mY = (y == 0 ? y : y * 2);

				image.setPixel(mX, mY, sf::Color::White);
				image.setPixel(mX, mY + 1, sf::Color::White);
				image.setPixel(mX + 1, mY, sf::Color::White);
				image.setPixel(mX + 1, mY + 1, sf::Color::White);
			}
		}
	}

	BodyComponent& b = player.getComponent<BodyComponent>();
	sf::RectangleShape* body = (sf::RectangleShape*)b.getShape("main");

	int tileX = std::floor((body->getPosition().x +
			(body->getGlobalBounds().width / 2)) / TILESIZE);
	int tileY = std::floor((body->getPosition().y +
			(body->getGlobalBounds().height / 2)) / TILESIZE);

	tileX = (tileX == 0 ? tileX : tileX * 2);
	tileY = (tileY == 0 ? tileY : tileY * 2);

	image.setPixel(tileX, tileY, sf::Color::Red);
	image.setPixel(tileX, tileY + 1, sf::Color::Red);
	image.setPixel(tileX, tileY + 2, sf::Color::Red);
	image.setPixel(tileX + 1, tileY, sf::Color::Red);
	image.setPixel(tileX + 2, tileY, sf::Color::Red);
	image.setPixel(tileX + 1, tileY + 1, sf::Color::Red);
	image.setPixel(tileX + 1, tileY + 2, sf::Color::Red);
	image.setPixel(tileX + 2, tileY + 1, sf::Color::Red);
	image.setPixel(tileX + 2, tileY + 2, sf::Color::Red);

	for(auto e : checkpoints){
		PositionComponent& pos = e.getComponent<PositionComponent>();

		tileX = std::floor(pos.screenPosition.x / TILESIZE);
		tileY = std::floor(pos.screenPosition.y / TILESIZE);

		tileX = (tileX == 0 ? tileX : tileX * 2);
		tileY = (tileY == 0 ? tileY : tileY * 2);

		sf::Color c = sf::Color(0, 153, 204);
		image.setPixel(tileX, tileY, c);
		image.setPixel(tileX, tileY + 1, c);
		image.setPixel(tileX, tileY + 2, c);
		image.setPixel(tileX + 1, tileY, c);
		image.setPixel(tileX + 2, tileY, c);
		image.setPixel(tileX + 1, tileY + 1, c);
		image.setPixel(tileX + 1, tileY + 2, c);
		image.setPixel(tileX + 2, tileY + 1, c);
		image.setPixel(tileX + 2, tileY + 2, c);
	}

	texture.update(image);
}
