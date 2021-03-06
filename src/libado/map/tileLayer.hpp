/*
 * tileLayer.hpp
 *
 *  Created on: 28 Mar 2016
 *      Author: linbob
 */

#ifndef LIBADO_MAP_TILELAYER_HPP_
#define LIBADO_MAP_TILELAYER_HPP_

#include <vector>
#include <memory>
#include <SFML/Window.hpp>

#include <rapidjson/filereadstream.h>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>

#include "tileFactory.hpp"
#include "layer.hpp"

class TileLayer : Layer{
public:
	TileLayer(){}
	TileLayer(int width, int height);

	void load(rapidjson::Value& layerData, TileFactory& factory);
	void save(rapidjson::Value& layerData,
			rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& dAlloc);

	virtual void render(sf::RenderWindow* window);
	virtual void resize(int w, int h);
	virtual int getWidth();
	virtual int getHeight();
	virtual bool isOccupied(int x, int y);
	std::unique_ptr<Tile>& getTile(int x, int y);
	void setTile(int x, int y, std::unique_ptr<Tile> t);
	std::vector<Tile*> getNeighbours(float x, float y);
	void deleteTile(int x, int y);
	void addShader(Tile::Type t, std::string path);
	sf::Shader* getShader(Tile::Type t){
		return shaders[t].get();
	}
protected:
	std::vector<std::vector<std::unique_ptr<Tile>> > map;

	std::map<Tile::Type, std::shared_ptr<sf::Shader> > shaders;
};

#endif /* LIBADO_MAP_TILELAYER_HPP_ */
