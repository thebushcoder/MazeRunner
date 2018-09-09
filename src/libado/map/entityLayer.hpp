/*
 * entityLayer.hpp
 *
 *  Created on: 28 Mar 2016
 *      Author: linbob
 */

#ifndef LIBADO_MAP_ENTITYLAYER_HPP_
#define LIBADO_MAP_ENTITYLAYER_HPP_

#include <vector>
#include <memory>

#include <SFML/Window.hpp>
#include <anax/Entity.hpp>
#include <anax/World.hpp>
#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>

#include "layer.hpp"
#include "../entity/entityFactory.hpp"

class TileMap;

class EntityLayer : Layer{
public:
	EntityLayer(){}
	EntityLayer(EntityFactory* factory, TileMap* tileMap, int width, int height);

	void load(rapidjson::Value& layerData);
	void save(rapidjson::Value& layerData,
			rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& dAlloc);
	virtual void render(sf::RenderWindow* window);
	virtual void resize(int w, int h);
	virtual bool isOccupied(int x, int y);
	std::vector<int>& getEntitiesAt(int x, int y);
	void setEntity(int x, int y, int id);
	void removeEntity(int x, int y, int id);
	EntityFactory* getFactory(){
		return factory;
	}
	virtual int getWidth();
	virtual int getHeight();
protected:
	EntityFactory* factory;
	TileMap* tileMap;

	std::vector<std::vector<std::vector<int>> > map;
};

#endif /* LIBADO_MAP_ENTITYLAYER_HPP_ */
