/*
 * checkpointUtil.cpp
 *
 *  Created on: 11 Jun 2018
 *      Author: linbob
 */

#include <random>

#include "checkpointUtil.hpp"

using namespace CheckpointUtil;

std::vector<anax::Entity> CheckpointUtil::createCheckPoints(int num, TileMap* map,
		EntityFactory* factory){
	std::vector<anax::Entity> list;

	std::random_device rd;
	std::mt19937_64 gen = std::mt19937_64(rd());
	std::uniform_int_distribution<> widthDist = std::uniform_int_distribution<>(0, map->getWidth());
	std::uniform_int_distribution<> heightDist = std::uniform_int_distribution<>(0, map->getHeight());

	PositionComponent& pos = factory->getPlayer().getComponent<PositionComponent>();

	int prevX = 0, prevY = 0;
	for(int i = 0; i < num; ++i){
		anax::Entity cp = factory->createEntity("checkpoint");

		int tileX;
		int tileY;

		bool f = true;

		while(f){
			tileX = widthDist(gen);
			tileY = heightDist(gen);

			/*
			 * 	position must be > 25 tiles from player
			 * 	position must be > 14 tiles from other checkpoints
			 */

			float magPlayer = sf::LineShape::calcLineMag(sf::Vector2f(
					pos.screenPosition.x / TILESIZE, pos.screenPosition.y / TILESIZE),
					sf::Vector2f(tileX, tileY));

			float scrX = (tileX * TILESIZE);
			float scrY = (tileY * TILESIZE);

			if(magPlayer >= 25){
				if(prevX == 0 && prevY == 0){
					// no previous CPs, check position + place
					if(map->getCost(tileX, tileY) != Tile::Type::SOLID
							&& map->getCost(tileX, tileY + 1) == Tile::Type::SOLID){

						cp.getComponent<PositionComponent>().setPosition(scrX, scrY);

						BodyComponent& b = cp.getComponent<BodyComponent>();
						sf::Shape* body = (sf::Shape*)b.getShape("main");

						body->setPosition(scrX, scrY);

						int tX = std::floor((scrX + (body->getGlobalBounds().width / 2)) /
								TILESIZE);
						int tY = std::floor((scrY + (body->getGlobalBounds().height / 2)) /
								TILESIZE);

						map->getEntityLayer().setEntity(tX, tY, cp.getId().index);

						list.push_back(cp);
						prevX = tileX;
						prevY = tileY;
						f = false;
					}
				}else{
					f = false;

					// check new CP is far enough from other CPs
					for(auto e : list){
						PositionComponent& p = e.getComponent<PositionComponent>();
						float mag = sf::LineShape::calcLineMag(sf::Vector2f(
								pos.screenPosition.x / TILESIZE, pos.screenPosition.y / TILESIZE),
								sf::Vector2f(p.screenPosition.x / TILESIZE,
										p.screenPosition.y / TILESIZE));

						if(mag <= 16){
							f = true;
						}
					}

					if(!f && map->getCost(tileX, tileY) != Tile::Type::SOLID
							&& map->getCost(tileX, tileY + 1) == Tile::Type::SOLID){

						cp.getComponent<PositionComponent>().setPosition(scrX, scrY);
						BodyComponent& b = cp.getComponent<BodyComponent>();
						sf::Shape* body = (sf::Shape*)b.getShape("main");

						body->setPosition(scrX, scrY);

						int tX = std::floor((scrX + (body->getGlobalBounds().width / 2)) /
								TILESIZE);
						int tY = std::floor((scrY + (body->getGlobalBounds().height / 2)) /
								TILESIZE);

						map->getEntityLayer().setEntity(tX, tY, cp.getId().index);

						list.push_back(cp);
						prevX = tileX;
						prevY = tileY;
					}else{
						f = true;
					}
				}
			}

		}
	}

	return list;
}


