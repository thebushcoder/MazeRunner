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
#include "../../map/tileMap.hpp"

static float const GRAVITY = 9.00f;

struct CollisionSystem : anax::System<anax::Requires<BodyComponent, MovementComponent>>{
	CollisionSystem(TileMap* m) : map(m){}

    /// Updates the MovementSystem
    /// \param deltaTime The change in time
    void update(sf::Time& delta){
    	for (auto entity : getEntities()){
			PositionComponent& p = entity.getComponent<PositionComponent>();
			MovementComponent& s = entity.getComponent<MovementComponent>();
			JumpComponent& j = entity.getComponent<JumpComponent>();
			BodyComponent& b = entity.getComponent<BodyComponent>();
			sf::RectangleShape* body = (sf::RectangleShape*)b.getShape("main");

			int tileX = std::floor((body->getPosition().x +
					(body->getGlobalBounds().width / 2)) / TILESIZE);
			int tileY = std::floor((body->getPosition().y +
					(body->getGlobalBounds().height / 2)) / TILESIZE);

			// check for collisons at new position
			b.resetGrid();
			checkCollisions(b, tileX, tileY, [](DirectionEnum::Direction d,
					BodyComponent& b){
				b.collisionGrid[d] = true;
			});

			if(j.inAir){
				/* 	dont apply collisions during the first 55% of jump
				 * 	prevents false-positive collisions during start of jump
				 */
				if(!(j.jumping)){
					checkAirCollisions(s, j, p, b, body, tileX, tileY);
				}
			}else{
				checkOnGround(j, b);
			}

			// rope swing physics
			bool hasRopeAnchor = false, ropeIsValid = false;
	    	if(entity.hasComponent<RopeComponent>()){
	    		anax::Entity rope = entity.getComponent<RopeComponent>().rope;
	    		if(rope.isValid()){
	    			ropeIsValid = true;
	    			hasRopeAnchor = rope.getComponent<RopeDetailsComponent>().isAnchored;
	    		}
	    	}

			// CHECK LEFT/RIGHT WALL COLLISIONS
			leftRightWallCheck(tileX, tileY, ropeIsValid, s, j, b, body);

			// if on rope & collision detected, adjust rope length
			checkRopeLen(hasRopeAnchor, b, entity);

			//POSITION ENTITY
			p.screenPosition.x = body->getPosition().x;
			p.screenPosition.y = body->getPosition().y;
    	}
    }

private:
    TileMap* map;

    void checkRopeLen(bool hasRopeAnchor, BodyComponent& b, anax::Entity entity){
		if(hasRopeAnchor){
			bool f = false;
			for(auto c : b.collisionGrid){
				if(c) f = true;	break;
			}
			if(!f){
				anax::Entity rope = entity.getComponent<RopeComponent>().rope;
				RopeDetailsComponent& r = rope.getComponent<RopeDetailsComponent>();
				if(r.currentLen != r.tmpLen && r.tmpLen != 0){
					r.currentLen = r.tmpLen;
				}
			}
		}
    }

    void checkOnGround(JumpComponent& j, BodyComponent& b){
		if(!b.collisionGrid[DirectionEnum::Direction::SW] &&
				!b.collisionGrid[DirectionEnum::Direction::S] &&
				!b.collisionGrid[DirectionEnum::Direction::SE]){
			j.setInAir(true);
		}
    }

    void stickToWall(JumpComponent& j, MovementComponent& s, sf::Keyboard::Key k,
    		JumpComponent::WallStickType t){
		if(j.inAir && sf::Keyboard::isKeyPressed(k)){
			if(j.jumping){
				j.toggleJump(false);
				j.doubleJ = true;
				j.wallStick = t;
				j.smlWallJump = j.smlWallJump.Zero;
			}
			if(j.wallStick != t){
				j.doubleJ = true;
				j.wallStick = t;
				j.smlWallJump = j.smlWallJump.Zero;
			}
			//wall friction
			s.currentVel.y *= 0.93;
		}
    }
    void leftRightWallCheck(int tileX, int tileY, bool ropeAlive, MovementComponent& s,
    		JumpComponent& j, BodyComponent& b, sf::RectangleShape* body){
    	if(b.collisionGrid[DirectionEnum::Direction::W]  &&
				((!ropeAlive && s.currentVel.x < 0) || ropeAlive)){
    		if(!ropeAlive){
				// stick to wall in air
				stickToWall(j, s, sf::Keyboard::A, JumpComponent::LEFT);
    		}

			Tile* t = map->getTileLayer().getTile(tileX - 1, tileY).get();

			body->setPosition(
					t->getBody().getPosition().x + t->getBody().getGlobalBounds().width - 2,
					body->getPosition().y);
			s.currentVel.x = 0;
    	}else if(b.collisionGrid[DirectionEnum::Direction::E] &&
				((!ropeAlive && s.currentVel.x > 0) || ropeAlive)){
    		if(!ropeAlive){
				// stick to wall in air
				stickToWall(j, s, sf::Keyboard::D, JumpComponent::RIGHT);
    		}
			Tile* t = map->getTileLayer().getTile(tileX + 1, tileY).get();

			body->setPosition(t->getBody().getPosition().x - body->getGlobalBounds().width - 2,
					body->getPosition().y);
			s.currentVel.x = 0;
    	}

    }
    void checkAirCollisions(MovementComponent& s, JumpComponent& j, PositionComponent& p,
    		 BodyComponent& b, sf::RectangleShape* body, int tileX, int tileY){
    	// tile down
		if(b.collisionGrid[DirectionEnum::Direction::S]){
			body->setPosition(body->getPosition().x,
					((tileY + 1) * TILESIZE) - body->getGlobalBounds().height);
			if(j.inAir){
				j.setInAir(false);
			}
			if(j.jumping){
				j.toggleJump(false);
			}
			s.currentVel.y = 0;
		}
		// tile down-left
		if(b.collisionGrid[DirectionEnum::Direction::SW] &&
				!b.collisionGrid[DirectionEnum::Direction::W]){
			body->setPosition(body->getPosition().x,
					((tileY + 1) * TILESIZE) - body->getGlobalBounds().height);
			if(j.inAir){
				j.setInAir(false);
			}
			if(j.jumping){
				j.toggleJump(false);
			}
			s.currentVel.y = 0;
		}
		// tile down-right
		if(b.collisionGrid[DirectionEnum::Direction::SE] &&
				!b.collisionGrid[DirectionEnum::Direction::E]){
			body->setPosition(body->getPosition().x,
					((tileY + 1) * TILESIZE) - body->getGlobalBounds().height);
			if(j.inAir){
				j.setInAir(false);
			}
			if(j.jumping){
				j.toggleJump(false);
			}
			s.currentVel.y = 0;
		}
		///////////////////////////////////////////////
		// tile top
		if(tileY - 1 >= 0 && map->getCost(tileX, tileY - 1) == Tile::Type::SOLID){
			Tile* t = map->getTileLayer().getTile(tileX, tileY - 1).get();
			if(t->getBody().getGlobalBounds().intersects(body->getGlobalBounds())){
				body->setPosition(body->getPosition().x,
						t->getBody().getPosition().y +
						t->getBody().getGlobalBounds().height - 2);
				if(j.jumping){
					j.toggleJump(false);
				}
				s.currentVel.y = 0;
			}
		}
		// tile top-LEFT
		if((tileY - 1 >= 0 && tileX - 1 >= 0) &&
				map->getCost(tileX - 1, tileY - 1) == Tile::Type::SOLID &&
				map->getCost(tileX - 1, tileY) == Tile::Type::AIR &&
				s.currentVel.y < 0){
			Tile* t = map->getTileLayer().getTile(tileX - 1, tileY - 1).get();
			if(t->getBody().getGlobalBounds().intersects(body->getGlobalBounds())){
				body->setPosition(p.screenPosition.x, p.screenPosition.y);
				if(j.jumping){
					j.toggleJump(false);
				}
				s.currentVel.y = 0;
			}
		}
		// tile top-RIGHT
		if((tileY - 1 >= 0 && tileX + 1 < map->getWidth()) &&
				map->getCost(tileX + 1, tileY - 1) == Tile::Type::SOLID &&
				map->getCost(tileX + 1, tileY) == Tile::Type::AIR &&
				s.currentVel.y < 0){
			Tile* t = map->getTileLayer().getTile(tileX + 1, tileY - 1).get();
			if(t->getBody().getGlobalBounds().intersects(body->getGlobalBounds())){
				body->setPosition(p.screenPosition.x, p.screenPosition.y);
				if(j.jumping){
					j.toggleJump(false);
				}
				s.currentVel.y = 0;
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
