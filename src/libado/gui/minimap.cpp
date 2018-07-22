/*
 * minimap.cpp
 *
 *  Created on: 10 Jun 2018
 *      Author: linbob
 */

#include "minimap.hpp"
#include "../../gameScreen.hpp"

MiniMap::MiniMap(tgui::Theme::Ptr theme, GameScreen* screen) :
	world(screen->getEntityFactory()->world), map(screen->getTileMap()){

	setBackgroundColor(tgui::Color(0, 0, 204, 198));
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
	image.create(canvas->getSize().x, canvas->getSize().y, sf::Color(71,71,71));

	// draw maze walls/floors
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

	// draw entities
	for(auto& e : world->getEntities()){
		if(e.hasComponent<MapComponent>()){
			MapComponent& mC = e.getComponent<MapComponent>();
			PositionComponent& pos = e.getComponent<PositionComponent>();

			int tileX = std::floor(pos.screenPosition.x / TILESIZE);
			int tileY = std::floor(pos.screenPosition.y / TILESIZE);

			tileX = (tileX == 0 ? tileX : tileX * 2);
			tileY = (tileY == 0 ? tileY : tileY * 2);

			int j = e.hasComponent<PlayerComponent>() ||
					e.hasComponent<CheckpointComponent>() ? 5 : 3;

			for(int x = 0; x < j; ++x){
				for(int y = 0; y < j; ++y){
					image.setPixel(tileX + x, tileY + y, mC.getColor());
				}
			}
		}
	}

	texture.update(image);
}
