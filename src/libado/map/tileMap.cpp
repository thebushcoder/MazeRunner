/*
 * tileMap.cpp
 *
 *  Created on: 9 Feb 2016
 *      Author: linbob
 */

#include "tileMap.hpp"

TileMap::TileMap(EntityFactory* f, std::shared_ptr<TileFactory> tileFactory, int w, int h) :
	width(w), height(h){
	tileLayer = std::unique_ptr<TileLayer>(new TileLayer(width, height));
	entityLayer = std::unique_ptr<EntityLayer>(new EntityLayer(f, this, width, height));
	TileMap::factory = tileFactory;
}
TileMap::TileMap(EntityFactory* f, std::string jsonPath, int w, int h) : width(w), height(h){
	TileMap::factory = std::shared_ptr<TileFactory>(new TileFactory(jsonPath));

	tileLayer = std::unique_ptr<TileLayer>(new TileLayer(width, height));
	entityLayer = std::unique_ptr<EntityLayer>(new EntityLayer(f, this, width, height));
}
void TileMap::render(sf::RenderWindow* window){
	tileLayer->render(window);
//	entityLayer->render(window);
}
void TileMap::resize(int w, int h){
	width = w;
	height = h;

	tileLayer->resize(w, h);
}
void TileMap::load(std::string filePath){
	mapName = filePath.substr(filePath.find_last_of("/") + 1);
	//READ MAP JSON
	char readBuffer[65536];
	rapidjson::Document d;

	FILE* fp = fopen(&filePath[0], "r");
	rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
	d.ParseStream(is);

	fclose(fp);

	//LOAD MAP SIZE
	width = d["width"].GetInt();
	height = d["height"].GetInt();

	//reinitialize layers with saved width/height
	tileLayer = std::unique_ptr<TileLayer>(new TileLayer(width, height));

	tileLayer->load(d["layers"]["tiles"], *factory);
}
void TileMap::save(std::string filePath){
	//CREATE JSON
	rapidjson::Document d;
	d.SetObject();

	//JSON-WRITE MAP SIZE
	d.AddMember("width", width, d.GetAllocator());
	d.AddMember("height", height, d.GetAllocator());

	//JSON-WRITE LAYERS + TILES
	rapidjson::Value layersJSON(rapidjson::kObjectType);

	rapidjson::Value layer(rapidjson::kArrayType);

	tileLayer->save(layer, d.GetAllocator());
	layersJSON.AddMember("tiles", layer, d.GetAllocator());

	d.AddMember("layers", layersJSON, d.GetAllocator());

	//WRITE JSON TO FILE
	FILE* fp = fopen(filePath.c_str(), "w");

	char writeBuffer[65536];
	rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
	rapidjson::PrettyWriter<rapidjson::FileWriteStream> writer(os);
	d.Accept(writer);

	fclose(fp);
}
void TileMap::load(rapidjson::Document& d){
	//LOAD MAP SIZE
	width = d["width"].GetInt();
	height = d["height"].GetInt();

	//reinitialize layers with saved width/height
	tileLayer = std::unique_ptr<TileLayer>(new TileLayer(width, height));

	tileLayer->load(d["layers"]["tiles"], *factory);
}
void TileMap::save(rapidjson::Document& d){
	//JSON-WRITE MAP SIZE
	d.AddMember("width", width, d.GetAllocator());
	d.AddMember("height", height, d.GetAllocator());

	//JSON-WRITE LAYERS + TILES
	rapidjson::Value layersJSON(rapidjson::kObjectType);

	rapidjson::Value layer(rapidjson::kArrayType);

	tileLayer->save(layer, d.GetAllocator());
	layersJSON.AddMember("tiles", layer, d.GetAllocator());

	d.AddMember("layers", layersJSON, d.GetAllocator());
}
TileMap& TileMap::operator = (TileMap& other){
	TileMap::width = other.width;
	TileMap::height = other.height;

	TileMap::factory = std::move(other.factory);

	tileLayer = std::move(other.tileLayer);
	entityLayer = std::move(other.entityLayer);

	return *this;
}
std::vector<sf::Vector2i> TileMap::getNeighbours(int x, int y){
	std::vector<sf::Vector2i> list;
	if(x - 1 >= 0 && tileLayer->isOccupied(x - 1, y)){
		list.push_back(sf::Vector2i(x - 1, y));
	}
	if(x + 1 < width && tileLayer->isOccupied(x + 1, y)){
		list.push_back(sf::Vector2i(x + 1, y));
	}
	if(y - 1 >= 0 && tileLayer->isOccupied(x, y - 1)){
		list.push_back(sf::Vector2i(x, y - 1));
	}
	if(y + 1 < height && tileLayer->isOccupied(x, y + 1)){
		list.push_back(sf::Vector2i(x, y + 1));
	}

	return list;
}
Tile::Type TileMap::getCost(int dx, int dy){
	if((dx < 0 || dx >= width) || (dy < 0 || dy >= height)) return Tile::Type::SOLID;

	if(tileLayer->isOccupied(dx, dy)) return tileLayer->getTile(dx, dy)->getType();

	return Tile::Type::AIR;
}
TileLayer& TileMap::getTileLayer(){
	return *tileLayer;
}
std::shared_ptr<TileFactory> TileMap::getFactory(){
	return factory;
}
EntityLayer& TileMap::getEntityLayer(){
	return *entityLayer;
}
int TileMap::getWidth(){
	return TileMap::width;
}
int TileMap::getHeight(){
	return TileMap::height;
}
