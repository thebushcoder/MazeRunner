/*
 * tileFactory.hpp
 *
 *  Created on: 25 Feb 2016
 *      Author: linbob
 */

#ifndef TILEFACTORY_HPP_
#define TILEFACTORY_HPP_

#include <vector>
#include <string>
#include <map>
#include <memory>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <rapidjson/filereadstream.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

#include "../textureAtlas.hpp"
#include "tile.hpp"

class TileFactory{
public:
	TileFactory(){}
	TileFactory(std::string jsonPath);

	std::unique_ptr<Tile> createTile(int id);
	std::map<int, std::unique_ptr<Tile>>& getTiles();
private:
	std::map<int, std::unique_ptr<Tile>> sourceTiles;
};


#endif /* TILEFACTORY_HPP_ */
