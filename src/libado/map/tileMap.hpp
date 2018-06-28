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

#include <uuid/uuid.h>
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

class TileMap{
public:
	TileMap(){}
	TileMap(std::shared_ptr<TileFactory> tileFactory, int w, int h);
	TileMap(std::string jsonPath, int w, int h);

	void load(std::string filePath);
	void save(std::string filePath);
	void save(rapidjson::Document& json);
	void load(rapidjson::Document& json);
	void render(sf::RenderWindow* window);
	void resize(int w, int h);
	int getWidth();
	int getHeight();
	std::shared_ptr<TileFactory> getFactory();
	TileLayer& getTileLayer();
	Tile::Type getCost(int dx, int dy);
	std::vector<sf::Vector2i> getNeighbours(int x, int y);
	TileMap& operator = (TileMap& other);
	std::string getName(){
		return mapName;
	}
protected:
	std::shared_ptr<TileFactory> factory;
	std::unique_ptr<TileLayer> tileLayer;
	int width, height;
	std::string mapName;
};

#endif /* TILEMAP_HPP_ */