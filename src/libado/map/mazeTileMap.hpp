/*
 * mazeTileMap.hpp
 *
 *  Created on: 20 Aug. 2018
 *      Author: linbob
 */

#ifndef LIBADO_MAP_MAZETILEMAP_HPP_
#define LIBADO_MAP_MAZETILEMAP_HPP_

#include "tileMap.hpp"

class MazeTileMap : public TileMap{
public:
	MazeTileMap(EntityFactory* factory, std::shared_ptr<TileFactory> tileFactory, int w, int h) :
		TileMap(factory, tileFactory, w, h){}
	MazeTileMap(EntityFactory* factory, std::string jsonPath, int w, int h) :
		TileMap(factory, jsonPath, w, h){}

	virtual ~MazeTileMap(){}

	void update(sf::Time& delta){
//		printf("\n==========\n START List size : %d\n", lavaList.size());
		for(auto p : lavaList){
			p->timer += delta;
			if(p->timer.asSeconds() >= maxLavaTime){
				p->type = Tile::Type::SOLID;
				lavaList.erase(std::remove(lavaList.begin(), lavaList.end(), p), lavaList.end());
			}
		}
//		printf(" END List size : %d\n==========\n", lavaList.size());
	}
	void addLavaTile(Tile* t){
		t->type = Tile::Type::LAVA;
		t->timer = sf::Time::Zero;
		lavaList.push_back(t);
	}

private:
	float maxLavaTime = 2.0;

	std::vector<Tile*> lavaList;
};



#endif /* LIBADO_MAP_MAZETILEMAP_HPP_ */
