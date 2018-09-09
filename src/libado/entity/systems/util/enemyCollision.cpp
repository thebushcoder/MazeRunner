/*
 * enemyCollision.cpp
 *
 *  Created on: 8 Jul 2018
 *      Author: linbob
 */

#include "enemyCollision.hpp"

EnemyCollision::EnemyCollision(TileMap* map) : CollisionModule(map, false, false){}
EnemyCollision::~EnemyCollision(){}

void EnemyCollision::preCheck(anax::Entity entity){}
void EnemyCollision::postCheck(anax::Entity entity){}

void EnemyCollision::entityCollision(anax::Entity entity, anax::Entity collider, int tileX, int tileY){
	BodyComponent& b = entity.getComponent<BodyComponent>();
	sf::CircleShape* body = (sf::CircleShape*)b.getShape("main");
	MovementComponent& m = entity.getComponent<MovementComponent>();

	if(collider.hasComponent<PlayerComponent>()){

	}else if(collider.hasComponent<EnemyComponent>()){
		BodyComponent& colB = collider.getComponent<BodyComponent>();
		sf::CircleShape* colBody = (sf::CircleShape*)colB.getShape("main");
		MovementComponent& colM = collider.getComponent<MovementComponent>();

		// Circle-circle collision
		if(sf::LineShape::calcLineMag(body->getPosition(), colBody->getPosition()) <
				body->getRadius() + colBody->getRadius()){
			float bRad = body->getRadius();
			float colRad = colBody->getRadius();

			// calc current velocities
			sf::Vector2f newVel, newColVel;
			sf::Vector2f curVel = m.currentAcc;
			curVel.x *= m.currentVel.x;
			curVel.y *= m.currentVel.y;
			sf::Vector2f curColVel = colM.currentAcc;
			curColVel.x *= colM.currentVel.x;
			curColVel.y *= colM.currentVel.y;

			// calc collision points + new velocities
			newVel.x = (curVel.x * (bRad - colRad) + (2 * colRad * curColVel.x)) / (bRad + colRad);
			newVel.y = (curVel.y * (bRad - colRad) + (2 * colRad * curColVel.y)) / (bRad + colRad);
			newColVel.x = (curColVel.x * (colRad - bRad) + (2 * bRad * curVel.x)) / (bRad + colRad);
			newColVel.y = (curColVel.y * (colRad - bRad) + (2 * bRad * curVel.y)) / (bRad + colRad);

			sf::Vector2f prevPos = body->getPosition();
			sf::Vector2f prevColPos = colBody->getPosition();

			int colTileX = std::floor((colBody->getPosition().x +
					(colBody->getGlobalBounds().width / 2)) / TILESIZE);
			int colTileY = std::floor((colBody->getPosition().y +
					(colBody->getGlobalBounds().height / 2)) / TILESIZE);

//			map->getEntityLayer().removeEntity(tileX, tileY, entity.getId().index);
			map->getEntityLayer().removeEntity(colTileX, colTileY, collider.getId().index);

			body->move(newVel);
			colBody->move(newColVel);

			colTileX = std::floor((colBody->getPosition().x +
					(colBody->getGlobalBounds().width / 2)) / TILESIZE);
			colTileY = std::floor((colBody->getPosition().y +
					(colBody->getGlobalBounds().height / 2)) / TILESIZE);

//			map->getEntityLayer().setEntity(tileX, tileY, entity.getId().index);
			map->getEntityLayer().setEntity(colTileX, colTileY, collider.getId().index);

			// calc new normalized directions
			float mag = sf::LineShape::calcLineMag(body->getPosition(), prevPos);
			float colMag = sf::LineShape::calcLineMag(colBody->getPosition(), prevColPos);

			m.currentAcc.x = (body->getPosition().x - prevPos.x) / mag;
			m.currentAcc.y = (body->getPosition().y - prevPos.y) / mag;
			colM.currentAcc.x = (colBody->getPosition().x - prevColPos.x) / colMag;
			colM.currentAcc.y = (colBody->getPosition().y - prevColPos.y) / colMag;

			//	calc new accelerations
			m.currentVel.x = newVel.x / m.currentAcc.x;
			m.currentVel.y = newVel.y / m.currentAcc.y;
			colM.currentVel.x = newColVel.x / colM.currentAcc.x;
			colM.currentVel.y = newColVel.y / colM.currentAcc.y;
		}
	}
}
void EnemyCollision::roofCollision(anax::Entity entity, int tileX, int tileY){
	MovementComponent& m = entity.getComponent<MovementComponent>();
	BodyComponent& b = entity.getComponent<BodyComponent>();

	if(m.currentAcc.y > 0 || !b.collisionGrid[DirectionEnum::N]) return;

	m.currentAcc.y = -m.currentAcc.y;

	sf::Shape* body = b.getShape("main");

	body->setPosition(body->getPosition().x,
			((tileY - 1) * TILESIZE) + TILESIZE);

	((MazeTileMap*)map)->addLavaTile(map->getTileLayer().getTile(tileX, tileY - 1).get());
}
void EnemyCollision::floorCollision(anax::Entity entity, int tileX, int tileY){
	MovementComponent& m = entity.getComponent<MovementComponent>();
	BodyComponent& b = entity.getComponent<BodyComponent>();

	if(m.currentAcc.y < 0 || !b.collisionGrid[DirectionEnum::S]) return;

	m.currentAcc.y = -m.currentAcc.y;

	sf::CircleShape* body = (sf::CircleShape*)b.getShape("main");

	body->setPosition(body->getPosition().x,
			((tileY + 1) * TILESIZE) - body->getGlobalBounds().height);

	((MazeTileMap*)map)->addLavaTile(map->getTileLayer().getTile(tileX, tileY + 1).get());
}
void EnemyCollision::wallCollision(anax::Entity entity, int tileX, int tileY){
	MovementComponent& m = entity.getComponent<MovementComponent>();
	BodyComponent& b = entity.getComponent<BodyComponent>();
	sf::Shape* body = b.getShape("main");

	if(b.collisionGrid[DirectionEnum::E] && m.currentAcc.x > 0){
		// RIGHT
		m.currentAcc.x = -m.currentAcc.x;
		body->setPosition(((tileX + 1) * TILESIZE) - body->getGlobalBounds().width,
				body->getPosition().y);

		((MazeTileMap*)map)->addLavaTile(map->getTileLayer().getTile(tileX + 1, tileY).get());
	}else if(b.collisionGrid[DirectionEnum::W] && m.currentAcc.x < 0){
		// LEFT
		m.currentAcc.x = -m.currentAcc.x;
		body->setPosition(((tileX - 1) * TILESIZE) + TILESIZE,
				body->getPosition().y);

		((MazeTileMap*)map)->addLavaTile(map->getTileLayer().getTile(tileX - 1, tileY).get());
	}
}
