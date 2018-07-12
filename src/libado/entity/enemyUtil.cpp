/*
 * enemyUtil.cpp
 *
 *  Created on: 3 Jul 2018
 *      Author: linbob
 */

#include <random>

#include "enemyUtil.hpp"

using namespace EnemyUtil;

void EnemyUtil::placeEnemy(TileMap* map, anax::Entity e, std::vector<anax::Entity>& list,
		float scrX, float scrY){
	e.getComponent<PositionComponent>().setPosition(scrX, scrY);

	BodyComponent& b = e.getComponent<BodyComponent>();
	sf::Shape* body = (sf::Shape*)b.getShape("main");

	body->setPosition(scrX, scrY);

	int tX = std::floor((scrX + (body->getGlobalBounds().width / 2)) /
			TILESIZE);
	int tY = std::floor((scrY + (body->getGlobalBounds().height / 2)) /
			TILESIZE);

	map->getEntityLayer().setEntity(tX, tY, e.getId().index);

	list.push_back(e);
}

void EnemyUtil::createEnemies(int numPerCp, TileMap* map, EntityFactory* factory,
		std::vector<anax::Entity>& checkpoints){
	std::vector<anax::Entity> list;

	int searchDepth = 64;

	std::random_device rd;
	std::mt19937_64 gen = std::mt19937_64(rd());
	std::uniform_int_distribution<> searchDist = std::uniform_int_distribution<>(-searchDepth, searchDepth);
	std::uniform_real_distribution<> normDist = std::uniform_real_distribution<>(-1, 1);

	PositionComponent& playerPos = factory->getPlayer().getComponent<PositionComponent>();

	// for each checkpoint
	for(int i = 0; i < checkpoints.size(); ++i){
		PositionComponent& cpPos = checkpoints[i].getComponent<PositionComponent>();

		int tX = std::floor(cpPos.screenPosition.x / TILESIZE);
		int tY = std::floor(cpPos.screenPosition.y / TILESIZE);

		// create and place 'numPerCp' enemies
		for(int j = 0; j < numPerCp; ++j){
			anax::Entity e = factory->createEntity("smlEnemy");
			MovementComponent& m = e.getComponent<MovementComponent>();

			m.currentAcc.x = normDist(gen);
			m.currentAcc.y = normDist(gen);

			int tileX, tileY;

			bool f = true;

			// loop to find suitable placement
			while(f){
				tileX = tX + searchDist(gen);
				tileY = tY + searchDist(gen);

				if(tileX <= 0 || tileX >= map->getWidth() || tileY <= 0 ||
						tileY >= map->getHeight()) continue;

				float magCp = sf::LineShape::calcLineMag(sf::Vector2f(
						cpPos.screenPosition.x / TILESIZE, cpPos.screenPosition.y / TILESIZE),
						sf::Vector2f(tileX, tileY));

				// check tile type and dist from cp
				if(map->getCost(tileX, tileY) == Tile::Type::SOLID || magCp <= 2)	continue;

				float magPlayer = sf::LineShape::calcLineMag(sf::Vector2f(
						playerPos.screenPosition.x / TILESIZE,
						playerPos.screenPosition.y / TILESIZE),
						sf::Vector2f(tileX, tileY));

				float scrX = (tileX * TILESIZE);
				float scrY = (tileY * TILESIZE);

				// check dist from player
				if(magPlayer >= 10){
					if(list.empty()){
						placeEnemy(map, e, list, scrX, scrY);
						f = false;
					}else{
						f = false;

						// check new enemy is far enough from other enemies
						for(auto e : list){
							PositionComponent& p = e.getComponent<PositionComponent>();
							float mag = sf::LineShape::calcLineMag(sf::Vector2f(
									tileX, tileY), sf::Vector2f(p.screenPosition.x / TILESIZE,
											p.screenPosition.y / TILESIZE));

							if(mag <= 7){
								f = true;
							}
						}

						if(!f){
							placeEnemy(map, e, list, scrX, scrY);
						}else{
							f = true;
						}
					}
				}
			}
		}
	}
}
