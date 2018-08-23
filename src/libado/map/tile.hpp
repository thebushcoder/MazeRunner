/*
 * tile.hpp
 *
 *  Created on: 20 Aug. 2018
 *      Author: linbob
 */

#ifndef LIBADO_MAP_TILE_HPP_
#define LIBADO_MAP_TILE_HPP_

#define TILESIZE 64

#include <SFML/Graphics.hpp>

class Tile{
public:
	enum Type{
		SOLID,
		DESTRUCTIBLE,
		AIR,
		LAVA
	};

	sf::Time timer; //	used for special effects/shaders
	Type type;

	Tile() : body(std::move(std::unique_ptr<sf::RectangleShape>(nullptr))){}
	Tile(int i, Type t, std::unique_ptr<sf::RectangleShape>& b);
	Tile(Tile* t);

	sf::RectangleShape& getBody();
	int getId();
	Type getType();
	void setPosition(int x, int y);
protected:
	int id = 0;

	std::unique_ptr<sf::RectangleShape> body;
};


#endif /* LIBADO_MAP_TILE_HPP_ */
