/*
 * entityLayer.cpp
 *
 *  Created on: 28 Mar 2016
 *      Author: linbob
 */

#include "entityLayer.hpp"

#include "tileMap.hpp"

EntityLayer::EntityLayer(EntityFactory* factory, TileMap* tileMap, int width, int height) :
	factory(factory), tileMap(tileMap){
	map.resize(width);
	for (int i = 0; i < width; ++i){
		map[i].resize(height);
	}
	for(int x = 0; x < map.size(); ++x){
		for(int y = 0; y < map[x].size(); ++y){
			map[x][y] = std::vector<int>();
		}
	}
}
void EntityLayer::resize(int w, int h){
	int wDiff = w - map.size();
	int hDiff = h - map[0].size();

	map.resize(w);
	for (int i = 0; i < w; ++i){
		map[i].resize(h);
	}

	for(int x = map.size() - wDiff; x < map.size(); ++x){
		for(int y = map[0].size() - hDiff; y < map[x].size(); ++y){
			map[x][y] = std::vector<int>();
		}
	}
}
void EntityLayer::removeEntity(int x, int y, int id){
	map[x][y].erase(std::remove_if(map[x][y].begin(), map[x][y].end(),
	[&](int const& n) -> bool
	{
		return n == id;
	}));
}
std::vector<int>& EntityLayer::getEntitiesAt(int x, int y){
	return map[x][y];
}
bool EntityLayer::isOccupied(int x, int y){
	return !map[x][y].empty();
}
void EntityLayer::setEntity(int x, int y, int id){
	map[x][y].push_back(id);
}
int EntityLayer::getWidth(){
	return map.size();
}
int EntityLayer::getHeight(){
	return map[0].size();
}
void EntityLayer::load(rapidjson::Value& layerData){

}
void EntityLayer::save(rapidjson::Value& layerData,
		rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& dAlloc){

}
void EntityLayer::render(sf::RenderWindow* window){
	for(int x = 0; x < getWidth(); ++x){
		for(int y = 0; y < getHeight(); ++y){
			if(map[x][y].empty()) continue;

			sf::RectangleShape rect(sf::Vector2f(TILESIZE, TILESIZE));
			rect.setPosition(x * TILESIZE, y * TILESIZE);
			rect.setFillColor(sf::Color::Magenta);
			window->draw(rect);
		}
	}
}
