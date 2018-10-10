/*
 * tileLayer.cpp
 *
 *  Created on: 28 Mar 2016
 *      Author: linbob
 */

#include "tileLayer.hpp"

TileLayer::TileLayer(int width, int height){
	map.resize(width);
	for (int i = 0; i < width; ++i){
		TileLayer::map[i].resize(height);
	}
	for(int x = 0; x < map.size(); ++x){
		for(int y = 0; y < map[x].size(); ++y){
			map[x][y] = nullptr;
		}
	}
}
void TileLayer::addShader(Tile::Type t, std::string path){
	shaders[t] = std::make_shared<sf::Shader>();
	shaders[t]->loadFromFile(path + ".vs", path + ".fs");
}
void TileLayer::resize(int w, int h){
	int wDiff = w - map.size();
	int hDiff = h - map[0].size();

	map.resize(w);
	for (int i = 0; i < w; ++i){
		TileLayer::map[i].resize(h);
	}

	for(int x = map.size() - wDiff; x < map.size(); ++x){
		for(int y = map[0].size() - hDiff; y < map[x].size(); ++y){
			map[x][y] = nullptr;
		}
	}
}
void TileLayer::load(rapidjson::Value& layerData, TileFactory& factory){
	for(int j = 0; j < layerData.Size(); ++j){
		int x = layerData[j]["x"].GetInt();
		int y = layerData[j]["y"].GetInt();
		int id = layerData[j]["id"].GetInt();
		setTile(x, y, factory.createTile(id));
		map[x][y]->setPosition(x * TILESIZE, y * TILESIZE);
	}
}
void TileLayer::save(rapidjson::Value& layerData,
		rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& dAlloc){
	for(int x = 0; x < map.size(); ++x){
		for(int y = 0; y < map[x].size(); ++y){
			if(!map[x][y]) continue;
				rapidjson::Value t(rapidjson::kObjectType);
				t.AddMember("x", x, dAlloc);
				t.AddMember("y", y, dAlloc);
				t.AddMember("id", map[x][y]->getId(), dAlloc);

				layerData.PushBack(t, dAlloc);
		}
	}
}
void TileLayer::render(sf::RenderWindow* window){
	for(int x = 0; x < map.size(); ++x){
		for(int y = 0; y < map[x].size(); ++y){
			//RENDER TILE
			if(!map[x][y]) continue;

			if(shaders.find(map[x][y]->getType()) != shaders.end()){
				sf::Shader* s = shaders[map[x][y]->getType()].get();
				if(map[x][y]->getType() == Tile::Type::LAVA){
					s->setUniform("iTime", map[x][y]->timer.asSeconds());
//					s->setUniform("tex", *map[x][y]->getBody().getTexture());
				}
				window->draw(map[x][y]->getBody(), s);
			}else{
				window->draw(map[x][y]->getBody());
			}

			//debug dots
			sf::RectangleShape point;
			point.setSize(sf::Vector2f(2, 2));
			point.setFillColor(sf::Color::Magenta);
			point.setPosition(x * TILESIZE, y * TILESIZE);

			window->draw(point);
		}
	}
}
std::vector<Tile*> TileLayer::getNeighbours(float x, float y){
	std::vector<Tile*> list;

	if(map[x][y + 1]){
		list.emplace_back(map[x][y + 1].get());
	}
	if(map[x][y - 1]){
		list.emplace_back(map[x][y - 1].get());
	}
	if(map[x + 1][y]){
		list.emplace_back(map[x + 1][y].get());
	}
	if(map[x - 1][y]){
		list.emplace_back(map[x - 1][y].get());
	}
	if(map[x + 1][y + 1]){
		list.emplace_back(map[x + 1][y + 1].get());
	}
	if(map[x - 1][y + 1]){
		list.emplace_back(map[x - 1][y + 1].get());
	}
	if(map[x + 1][y - 1]){
		list.emplace_back(map[x + 1][y - 1].get());
	}
	if(map[x - 1][y - 1]){
		list.emplace_back(map[x - 1][y - 1].get());
	}

	return list;
}
int TileLayer::getWidth(){
	return map.size();
}
int TileLayer::getHeight(){
	return map[0].size();
}
bool TileLayer::isOccupied(int x, int y){
	return map[x][y] != nullptr;
}
std::unique_ptr<Tile>& TileLayer::getTile(int x, int y){
	return map[x][y];
}
void TileLayer::setTile(int x, int y, std::unique_ptr<Tile> t){
//	t->
	t->setPosition(x * TILESIZE, y * TILESIZE);
	map[x][y] = std::move(t);
}
void TileLayer::deleteTile(int x, int y){
	if(!map[x][y]) return;
	map[x][y] = nullptr;
}
