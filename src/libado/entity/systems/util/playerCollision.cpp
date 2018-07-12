/*
 * playerCollision.cpp
 *
 *  Created on: 8 Jul 2018
 *      Author: linbob
 */

#include "playerCollision.hpp"

PlayerCollision::PlayerCollision(TileMap* map, ParticleSystem* pSys) :
	CollisionModule(map, true, true), particleSys(pSys){}
PlayerCollision::~PlayerCollision(){}

void PlayerCollision::preCheck(anax::Entity entity){
	// rope check
	if(entity.hasComponent<RopeComponent>()){
		anax::Entity rope = entity.getComponent<RopeComponent>().rope;
		if(rope.isValid()){
			ropeIsValid = true;
			hasRopeAnchor = rope.getComponent<RopeDetailsComponent>().isAnchored;
		}else{
			ropeIsValid = false;
			hasRopeAnchor = false;
		}
	}
}
void PlayerCollision::postCheck(anax::Entity entity){
	BodyComponent& b = entity.getComponent<BodyComponent>();
	// check rope length
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

	// check if still on floor
	if(!b.collisionGrid[DirectionEnum::Direction::SW] &&
			!b.collisionGrid[DirectionEnum::Direction::S] &&
			!b.collisionGrid[DirectionEnum::Direction::SE]){
		JumpComponent& j = entity.getComponent<JumpComponent>();
		j.setInAir(true);
	}
}
void PlayerCollision::entityCollision(anax::Entity entity, anax::Entity collider,
		int tileX, int tileY){
	if(collider.hasComponent<CheckpointComponent>()){
		// trigger entity collision
		particleSys->createExplosion(16,
				collider.getComponent<PositionComponent>().screenPosition
		);

		map->getEntityLayer().removeEntity(tileX, tileY, collider.getId().index);

		collider.kill();
	}
}
void PlayerCollision::roofCollision(anax::Entity entity, int tileX, int tileY){
	JumpComponent& j = entity.getComponent<JumpComponent>();
	if(j.inAir){
		if(!j.jumping){
			BodyComponent& b = entity.getComponent<BodyComponent>();
			PositionComponent& p = entity.getComponent<PositionComponent>();
			MovementComponent& s = entity.getComponent<MovementComponent>();
			sf::RectangleShape* body = (sf::RectangleShape*)b.getShape("main");
			// tile top
			if(b.collisionGrid[DirectionEnum::N]){

				Tile* t = map->getTileLayer().getTile(tileX, tileY - 1).get();

				setRoofCollision(body->getPosition().x,
						t->getBody().getPosition().y +
						t->getBody().getGlobalBounds().height - 2,
						body, j, s);
			}
			// tile top-LEFT
			if(b.collisionGrid[DirectionEnum::NW] &&
					!b.collisionGrid[DirectionEnum::W] && s.currentVel.y < 0){
				setRoofCollision(p.screenPosition.x, p.screenPosition.y,
						body, j, s);
			}
			// tile top-RIGHT
			if(b.collisionGrid[DirectionEnum::NE] &&
					!b.collisionGrid[DirectionEnum::E] && s.currentVel.y < 0){
				setRoofCollision(p.screenPosition.x, p.screenPosition.y,
						body, j, s);
			}
		}
	}
}
void PlayerCollision::floorCollision(anax::Entity entity, int tileX, int tileY){
	JumpComponent& j = entity.getComponent<JumpComponent>();
	if(j.inAir){
		if(!j.jumping){
			BodyComponent& b = entity.getComponent<BodyComponent>();
			MovementComponent& s = entity.getComponent<MovementComponent>();
			sf::RectangleShape* body = (sf::RectangleShape*)b.getShape("main");
			// tile down
			if(b.collisionGrid[DirectionEnum::Direction::S]){
				setFloorCollision(body->getPosition().x,
						((tileY + 1) * TILESIZE) - body->getGlobalBounds().height,
						body, j, s);
			}
			// tile down-left
			if(b.collisionGrid[DirectionEnum::Direction::SW] &&
					!b.collisionGrid[DirectionEnum::Direction::W]){
				setFloorCollision(body->getPosition().x,
						((tileY + 1) * TILESIZE) - body->getGlobalBounds().height,
						body, j, s);
			}
			// tile down-right
			if(b.collisionGrid[DirectionEnum::Direction::SE] &&
					!b.collisionGrid[DirectionEnum::Direction::E]){
				setFloorCollision(body->getPosition().x,
						((tileY + 1) * TILESIZE) - body->getGlobalBounds().height,
						body, j, s);
			}
		}
	}
}
void PlayerCollision::wallCollision(anax::Entity entity, int tileX, int tileY){
	JumpComponent& j = entity.getComponent<JumpComponent>();
	BodyComponent& b = entity.getComponent<BodyComponent>();
	MovementComponent& s = entity.getComponent<MovementComponent>();
	sf::RectangleShape* body = (sf::RectangleShape*)b.getShape("main");

	// if collide w/ west wall and moving horizontally(not on rope) or swinging on rope
	if(b.collisionGrid[DirectionEnum::Direction::W]  &&
			((!ropeIsValid && s.currentVel.x < 0) || ropeIsValid)){

		if(!ropeIsValid){
			// stick to wall in air
			stickToWall(j, s, sf::Keyboard::A, JumpComponent::LEFT);
		}

		Tile* t = map->getTileLayer().getTile(tileX - 1, tileY).get();

		body->setPosition(
				t->getBody().getPosition().x + t->getBody().getGlobalBounds().width - 2,
				body->getPosition().y);
		s.currentVel.x = 0;
	// if collide w/ east wall and moving horizontally(not on rope) or swinging on rope
	}else if(b.collisionGrid[DirectionEnum::Direction::E] &&
			((!ropeIsValid && s.currentVel.x > 0) || ropeIsValid)){

		if(!ropeIsValid){
			// stick to wall in air
			stickToWall(j, s, sf::Keyboard::D, JumpComponent::RIGHT);
		}
		Tile* t = map->getTileLayer().getTile(tileX + 1, tileY).get();

		body->setPosition(t->getBody().getPosition().x - body->getGlobalBounds().width - 2,
				body->getPosition().y);
		s.currentVel.x = 0;
	}
}