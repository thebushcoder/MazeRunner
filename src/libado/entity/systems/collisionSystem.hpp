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

			int tileX = std::floor((body->getPosition().x +
					(body->getGlobalBounds().width / 2)) / TILESIZE);
			int tileY = std::floor((body->getPosition().y +
					(body->getGlobalBounds().height / 2)) / TILESIZE);

			map->getEntityLayer().removeEntity(tileX, tileY, entity.getId().index);

			// check for collisons at new position
			b.resetGrid();
			checkCollisions(b, tileX, tileY, [](DirectionEnum::Direction d,
					BodyComponent& b){
				b.collisionGrid[d] = true;
			});

			if(modules[b.getCollisionMod()]->hasPreCheck()){
				modules[b.getCollisionMod()]->preCheck(entity);
			}

			// roof
			if(b.collisionGrid[DirectionEnum::NE] || b.collisionGrid[DirectionEnum::N] ||
					b.collisionGrid[DirectionEnum::NW]){
				modules[b.getCollisionMod()]->roofCollision(entity, tileX, tileY);
			}

			// floor
			if(b.collisionGrid[DirectionEnum::SE] || b.collisionGrid[DirectionEnum::S] ||
					b.collisionGrid[DirectionEnum::SW]){
				modules[b.getCollisionMod()]->floorCollision(entity, tileX, tileY);
			}

			// walls
			modules[b.getCollisionMod()]->wallCollision(entity, tileX, tileY);

			if(modules[b.getCollisionMod()]->hasPostCheck()){
				modules[b.getCollisionMod()]->postCheck(entity);
			}

			tileX = std::floor((body->getPosition().x +
					(body->getGlobalBounds().width / 2)) / TILESIZE);
			tileY = std::floor((body->getPosition().y +
					(body->getGlobalBounds().height / 2)) / TILESIZE);

			// entity col check
			checkEntityCollisions(tileX, tileY, entity, body);

			tileX = std::floor((body->getPosition().x +
					(body->getGlobalBounds().width / 2)) / TILESIZE);
			tileY = std::floor((body->getPosition().y +
					(body->getGlobalBounds().height / 2)) / TILESIZE);

			//POSITION ENTITY
			p.screenPosition.x = body->getPosition().x;
			p.screenPosition.y = body->getPosition().y;

			map->getEntityLayer().setEntity(tileX, tileY, entity.getId().index);
    	}

//    	printf("CollisionSystem > debugTime: %f\n", debug.restart().asSeconds());
    }

private:
    TileMap* map;

    std::map<std::string, std::unique_ptr<CollisionModule>> modules;

    sf::Clock debug;

    void checkEntityCollisions(int tileX, int tileY, anax::Entity e,  sf::Shape* body){
    	checkNeighbour(tileX, tileY + 1, e, body);
    	checkNeighbour(tileX + 1, tileY, e, body);
    	checkNeighbour(tileX - 1, tileY, e, body);
    	checkNeighbour(tileX, tileY - 1, e, body);
    	checkNeighbour(tileX + 1, tileY + 1, e, body);
    	checkNeighbour(tileX - 1, tileY - 1, e, body);
    	checkNeighbour(tileX - 1, tileY + 1, e, body);
    	checkNeighbour(tileX + 1, tileY - 1, e, body);
    	checkNeighbour(tileX, tileY, e, body);
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
		if(tileY + 1 < map->getHeight() && map->getCost(tileX, tileY + 1) == Tile::Type::SOLID){
			Tile* t = map->getTileLayer().getTile(tileX, tileY + 1).get();
			if(t->getBody().getGlobalBounds().intersects(body->getGlobalBounds())){
				func(DirectionEnum::Direction::S, b);
			}
		}
		// tile down-left
		if((tileY + 1 < map->getHeight() && tileX - 1 >= 0) &&
				map->getCost(tileX - 1, tileY + 1) == Tile::Type::SOLID){
			Tile* t = map->getTileLayer().getTile(tileX - 1, tileY + 1).get();
			if(t->getBody().getGlobalBounds().intersects(body->getGlobalBounds())){
				func(DirectionEnum::Direction::SW, b);
			}
		}
		// tile down-right
		if((tileY + 1 < map->getHeight() && tileX + 1 < map->getWidth()) &&
				map->getCost(tileX + 1, tileY + 1) == Tile::Type::SOLID){
			Tile* t = map->getTileLayer().getTile(tileX + 1, tileY + 1).get();
			if(t->getBody().getGlobalBounds().intersects(body->getGlobalBounds())){
				func(DirectionEnum::Direction::SE, b);
			}
		}
		// tile left
		if(tileX - 1 >= 0 && map->getCost(tileX - 1, tileY) == Tile::Type::SOLID){
			Tile* t = map->getTileLayer().getTile(tileX - 1, tileY).get();
			if(t->getBody().getGlobalBounds().intersects(body->getGlobalBounds())){
				func(DirectionEnum::Direction::W, b);
			}
		}
		// tile right
		if(tileX + 1 >= 0 && map->getCost(tileX + 1, tileY) == Tile::Type::SOLID){
			Tile* t = map->getTileLayer().getTile(tileX + 1, tileY).get();
			if(t->getBody().getGlobalBounds().intersects(body->getGlobalBounds())){
				func(DirectionEnum::Direction::E, b);
			}
		}
		// tile up
		if(tileY - 1 >= 0 && map->getCost(tileX, tileY - 1) == Tile::Type::SOLID){
			Tile* t = map->getTileLayer().getTile(tileX, tileY - 1).get();
			if(t->getBody().getGlobalBounds().intersects(body->getGlobalBounds())){
				func(DirectionEnum::Direction::N, b);
			}
		}
		// tile up-left
		if((tileY - 1 >= 0 && tileX - 1 >= 0) &&
				map->getCost(tileX - 1, tileY - 1) == Tile::Type::SOLID){
			Tile* t = map->getTileLayer().getTile(tileX - 1, tileY - 1).get();
			if(t->getBody().getGlobalBounds().intersects(body->getGlobalBounds())){
				func(DirectionEnum::Direction::NW, b);
			}
		}
		// tile up-right
		if((tileY - 1 >= 0 && tileX + 1 < map->getWidth()) &&
				map->getCost(tileX + 1, tileY - 1) == Tile::Type::SOLID){
			Tile* t = map->getTileLayer().getTile(tileX + 1, tileY - 1).get();
			if(t->getBody().getGlobalBounds().intersects(body->getGlobalBounds())){
				func(DirectionEnum::Direction::NE, b);
			}
		}
	}
};

#endif /* LIBADO_ENTITY_SYSTEMS_COLLISIONSYSTEM_HPP_ */
