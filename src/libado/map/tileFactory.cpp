/*
 * tileFactory.cpp
 *
 *  Created on: 25 Feb 2016
 *      Author: linbob
 */

#include "tileFactory.hpp"

Tile::Tile(int i, Type t, std::unique_ptr<sf::RectangleShape>& b)
	:id(i), type(t), body(std::move(b)){}

Tile::Tile(Tile* t){
	id = t->id;
	type = t->type;
	body.reset(new sf::RectangleShape(sf::Vector2f(TILESIZE, TILESIZE)));
	body->setFillColor(t->getBody().getFillColor());
}

sf::RectangleShape& Tile::getBody(){
	return *Tile::body;
}
int Tile::getId(){
	return Tile::id;
}
Tile::Type Tile::getType(){
	return Tile::type;
}
void Tile::setPosition(int x, int y){
	sf::Vector2f pos(x, y);

	if(body != NULL){
		Tile::body->setPosition(pos);
	}
}

TileFactory::TileFactory(std::string jsonPath){
	if(jsonPath.empty()) return;
	char readBuffer[65536];
	rapidjson::Document d;

	FILE* fp = fopen(&jsonPath[0], "r");
	rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
	d.ParseStream(is);

	fclose(fp);

	for(int i = 0; i < d["tiles"].Size(); ++i){
		int id = d["tiles"][i]["id"].GetInt();

		std::unique_ptr<sf::RectangleShape> rect(new sf::RectangleShape(
				sf::Vector2f(TILESIZE, TILESIZE)));
		rect->setFillColor(sf::Color(d["tiles"][i]["colour"][0].GetInt(),
				d["tiles"][i]["colour"][1].GetInt(), d["tiles"][i]["colour"][2].GetInt()));
		rect->setOrigin(TILESIZE / 2, TILESIZE / 2);
		TileFactory::sourceTiles.insert(std::make_pair(id,
				std::unique_ptr<Tile>(new Tile(d["tiles"][i]["id"].GetInt(),
						Tile::Type(d["tiles"][i]["type"].GetInt()), rect)))
		);
	}
}
std::unique_ptr<Tile> TileFactory::createTile(int id){
	return std::unique_ptr<Tile>(new Tile(sourceTiles[id].get()));
}
std::map<int, std::unique_ptr<Tile>>& TileFactory::getTiles(){
	return sourceTiles;
}
