/*
 * collisionSystem.hpp
 *
 *  Created on: 28 Jun 2018
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_SYSTEMS_COLLISIONSYSTEM_HPP_
#define LIBADO_ENTITY_SYSTEMS_COLLISIONSYSTEM_HPP_

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <anax/System.hpp>

#include "../components/componentsCollection.hpp"
#include "../../util/quadtree.hpp"
#include "../../util/lineShape.hpp"
#include "../../entity/systems/particleSystem.hpp"
#include "../../map/tileMap.hpp"
#include "util/collisionModule.hpp"
#include "util/playerCollision.hpp"
#include "util/enemyCollision.hpp"

class GameScreen;
struct CollisionSystem : anax::System<anax::Requires<BodyComponent, MovementComponent>>{
	CollisionSystem(TileMap* m, GameScreen* s) : map(m){
		modules["player"] = std::unique_ptr<CollisionModule>(new PlayerCollision(
				m, s));
		modules["enemy"] = std::unique_ptr<CollisionModule>(new EnemyCollision(map));
	}

    /// Updates the MovementSystem
    /// \param deltaTime The change in time
    void update(sf::Time& delta){
    	debug.restart();

    	for (auto entity : getEntities()){
			PositionComponent& p = entity.getComponent<PositionComponent>();
			BodyComponent& b = entity.getComponent<BodyComponent>();
			sf::Shape* body = b.getShape("main");

			sf::Vector2i tilePos = map->getTilePosition(body->getPosition(),
					body->getGlobalBounds().width, body->getGlobalBounds().height);

			map->getEntityLayer().removeEntity(tilePos.x, tilePos.y, entity.getId().index);

			// check for collisons at new position
			b.resetGrid();
			checkCollisions(b, tilePos.x, tilePos.y, [](DirectionEnum::Direction d,
					BodyComponent& b){
				b.collisionGrid[d] = true;
			});

			if(modules[b.getCollisionMod()]->hasPreCheck()){
				modules[b.getCollisionMod()]->preCheck(entity);
			}

			// roof
			if(b.collisionGrid[DirectionEnum::NE] || b.collisionGrid[DirectionEnum::N] ||
					b.collisionGrid[DirectionEnum::NW]){
				modules[b.getCollisionMod()]->roofCollision(entity, tilePos.x, tilePos.y);
			}

			// floor
			if(b.collisionGrid[DirectionEnum::SE] || b.collisionGrid[DirectionEnum::S] ||
					b.collisionGrid[DirectionEnum::SW]){
				modules[b.getCollisionMod()]->floorCollision(entity, tilePos.x, tilePos.y);
			}

			// walls
			modules[b.getCollisionMod()]->wallCollision(entity, tilePos.x, tilePos.y);

			if(modules[b.getCollisionMod()]->hasPostCheck()){
				modules[b.getCollisionMod()]->postCheck(entity);
			}

			tilePos = map->getTilePosition(body->getPosition(),
					body->getGlobalBounds().width, body->getGlobalBounds().height);

			// entity col check
			checkEntityCollisions(tilePos.x, tilePos.y, entity, body);

			tilePos = map->getTilePosition(body->getPosition(),
					body->getGlobalBounds().width, body->getGlobalBounds().height);

			//POSITION ENTITY
			p.screenPosition.x = body->getPosition().x;
			p.screenPosition.y = body->getPosition().y;

			map->getEntityLayer().setEntity(tilePos.x, tilePos.y, entity.getId().index);
    	}
    }

private:
    TileMap* map;

    std::map<std::string, std::unique_ptr<CollisionModule>> modules;

    sf::Clock debug;

    void checkEntityCollisions(int tileX, int tileY, anax::Entity e,  sf::Shape* body){
    	BodyComponent& b = e.getComponent<BodyComponent>();
    	if(boundaryCheck(tileX, tileY + 1) && b.collisionGrid[DirectionEnum::S]){
        	checkNeighbour(tileX, tileY + 1, e, body);
    	}
    	if(boundaryCheck(tileX + 1, tileY) && b.collisionGrid[DirectionEnum::E]){
    		checkNeighbour(tileX + 1, tileY, e, body);
    	}
    	if(boundaryCheck(tileX - 1, tileY) && b.collisionGrid[DirectionEnum::W]){
        	checkNeighbour(tileX - 1, tileY, e, body);
    	}
    	if(boundaryCheck(tileX, tileY - 1) && b.collisionGrid[DirectionEnum::N]){
        	checkNeighbour(tileX, tileY - 1, e, body);
    	}
    	if(boundaryCheck(tileX + 1, tileY + 1) && b.collisionGrid[DirectionEnum::SE]){
    		checkNeighbour(tileX + 1, tileY + 1, e, body);
    	}
    	if(boundaryCheck(tileX - 1, tileY - 1) && b.collisionGrid[DirectionEnum::NW]){
    		checkNeighbour(tileX - 1, tileY - 1, e, body);
    	}
    	if(boundaryCheck(tileX - 1, tileY + 1) && b.collisionGrid[DirectionEnum::SW]){
    		checkNeighbour(tileX - 1, tileY + 1, e, body);
    	}
    	if(boundaryCheck(tileX + 1, tileY - 1) && b.collisionGrid[DirectionEnum::NE]){
    		checkNeighbour(tileX + 1, tileY - 1, e, body);
    	}
    	if(boundaryCheck(tileX, tileY)){
    		checkNeighbour(tileX, tileY, e, body);
    	}
    }

    void checkNeighbour(int tileX, int tileY, anax::Entity e, sf::Shape* body){
		if(map->getEntityLayer().isOccupied(tileX, tileY)){
			auto& l = map->getEntityLayer().getEntitiesAt(tileX, tileY);
			for(int i = 0; i < l.size(); ++i){
				anax::Entity n = getWorld().getEntity(l[i]);

				if(e.getId().index == n.getId().index) continue;
				BodyComponent& nB = n.getComponent<BodyComponent>();
				sf::Shape* nBody = nB.getShape("main");

				if(nBody->getGlobalBounds().intersects(body->getGlobalBounds())){
					BodyComponent& b = e.getComponent<BodyComponent>();
					modules[b.getCollisionMod()]->entityCollision(e, n, tileX, tileY);
				}

			}
		}
    }

    void checkCollisions(BodyComponent& b, int tileX, int tileY,
    		std::function<void(DirectionEnum::Direction dir, BodyComponent& b)> func){
		sf::RectangleShape* body = (sf::RectangleShape*)b.getShape("main");
		// tile down
		if(boundaryCheck(tileX, tileY - 1) &&
				map->getCost(tileX, tileY + 1) != Tile::Type::AIR){
			Tile* t = map->getTileLayer().getTile(tileX, tileY + 1).get();
			if(t->getBody().getGlobalBounds().intersects(body->getGlobalBounds())){
				func(DirectionEnum::Direction::S, b);
			}
		}
		// tile down-left
		if(boundaryCheck(tileX - 1, tileY + 1) &&
				map->getCost(tileX - 1, tileY + 1) != Tile::Type::AIR){
			Tile* t = map->getTileLayer().getTile(tileX - 1, tileY + 1).get();
			if(t->getBody().getGlobalBounds().intersects(body->getGlobalBounds())){
				func(DirectionEnum::Direction::SW, b);
			}
		}
		// tile down-right
		if(boundaryCheck(tileX + 1, tileY + 1) &&
				map->getCost(tileX + 1, tileY + 1) != Tile::Type::AIR){
			Tile* t = map->getTileLayer().getTile(tileX + 1, tileY + 1).get();
			if(t->getBody().getGlobalBounds().intersects(body->getGlobalBounds())){
				func(DirectionEnum::Direction::SE, b);
			}
		}
		// tile left
		if(boundaryCheck(tileX - 1, tileY) &&
				map->getCost(tileX - 1, tileY) != Tile::Type::AIR){
			Tile* t = map->getTileLayer().getTile(tileX - 1, tileY).get();
			if(t->getBody().getGlobalBounds().intersects(body->getGlobalBounds())){
				func(DirectionEnum::Direction::W, b);
			}
		}
		// tile right
		if(boundaryCheck(tileX + 1, tileY) &&
				map->getCost(tileX + 1, tileY) != Tile::Type::AIR){
			Tile* t = map->getTileLayer().getTile(tileX + 1, tileY).get();
			if(t->getBody().getGlobalBounds().intersects(body->getGlobalBounds())){
				func(DirectionEnum::Direction::E, b);
			}
		}
		// tile up
		if(boundaryCheck(tileX, tileY - 1) &&
				map->getCost(tileX, tileY - 1) != Tile::Type::AIR){
			Tile* t = map->getTileLayer().getTile(tileX, tileY - 1).get();
			if(t->getBody().getGlobalBounds().intersects(body->getGlobalBounds())){
				func(DirectionEnum::Direction::N, b);
			}
		}
		// tile up-left
		if(boundaryCheck(tileX - 1, tileY - 1) &&
				map->getCost(tileX - 1, tileY - 1) != Tile::Type::AIR){
			Tile* t = map->getTileLayer().getTile(tileX - 1, tileY - 1).get();
			if(t->getBody().getGlobalBounds().intersects(body->getGlobalBounds())){
				func(DirectionEnum::Direction::NW, b);
			}
		}
		// tile up-right
		if(boundaryCheck(tileX + 1, tileY - 1) &&
				map->getCost(tileX + 1, tileY - 1) != Tile::Type::AIR){
			Tile* t = map->getTileLayer().getTile(tileX + 1, tileY - 1).get();
			if(t->getBody().getGlobalBounds().intersects(body->getGlobalBounds())){
				func(DirectionEnum::Direction::NE, b);
			}
		}
	}

	bool boundaryCheck(int tileX, int tileY){
		if(tileX >= 0 && tileY >= 0 && tileX <= map->getWidth() - 1 &&
				tileY <= map->getHeight() - 1){
			return true;
		}
		return false;
	}
};

#endif /* LIBADO_ENTITY_SYSTEMS_COLLISIONSYSTEM_HPP_ */
