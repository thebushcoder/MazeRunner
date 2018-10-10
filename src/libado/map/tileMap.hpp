/*
 * tileMap.hpp
 *
 *  Created on: 9 Feb 2016
 *      Author: linbob
 */

#ifndef TILEMAP_HPP_
#define TILEMAP_HPP_

#include <vector>
#include <string>
#include <map>
#include <memory>
#include <iostream>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <anax/World.hpp>
#include <anax/Entity.hpp>

#include <rapidjson/filereadstream.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>

#include "tileFactory.hpp"
#include "tileLayer.hpp"
#include "entityLayer.hpp"
#include "../entity/entityFactory.hpp"

class TileMap{
public:
	TileMap(){}
	TileMap(EntityFactory* factory, std::shared_ptr<TileFactory> tileFactory, int w, int h);
	TileMap(EntityFactory* factory, std::string jsonPath, int w, int h);

	void load(std::string filePath);
	void save(std::string filePath);
	void save(rapidjson::Document& json);
	void load(rapidjson::Document& json);
	virtual void update(sf::Time& delta){}
	void render(sf::RenderWindow* window);
	void resize(int w, int h);
	int getWidth();
	int getHeight();
	std::shared_ptr<TileFactory> getFactory();
	TileLayer& getTileLayer();
	EntityLayer& getEntityLayer();
	Tile::Type getCost(int dx, int dy);
	std::vector<sf::Vector2i> getNeighbours(int x, int y);
	TileMap& operator = (TileMap& other);
	std::string getName(){
		return mapName;
	}
	sf::Vector2i getTilePosition(sf::Vector2f position, float w, float h){
		sf::Vector2i vec;
		vec.x = std::floor((position.x +
				(w / 2)) / TILESIZE);
		vec.y = std::floor((position.y +
				(h / 2)) / TILESIZE);

		if(vec.x <= 0){
			vec.x = 1;
		}else if(vec.x >= getWidth()){
			vec.x = getWidth() - 1;
		}
		if(vec.y <= 0){
			vec.y = 1;
		}else if(vec.y >= getHeight()){
			vec.y = getHeight() - 1;
		}

		return vec;
	}
protected:
	std::shared_ptr<TileFactory> factory;
	std::unique_ptr<TileLayer> tileLayer;
	std::unique_ptr<EntityLayer> entityLayer;
	int width, height;
	std::string mapName;
};

#endif /* TILEMAP_HPP_ */
