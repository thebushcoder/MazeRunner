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
	BodyComponent& colB = collider.getComponent<BodyComponent>();
	sf::CircleShape* colBody = (sf::CircleShape*)b.getShape("main");
	MovementComponent& colM = collider.getComponent<MovementComponent>();

	if(collider.hasComponent<PlayerComponent>()){

	}else if(collider.hasComponent<EnemyComponent>()){
		// Circle-circle collision
		if(sf::LineShape::calcLineMag(body->getPosition(), colBody->getPosition()) <
				body->getRadius() + colBody->getRadius()){
			float bRad = body->getRadius();
			float colRad = colBody->getRadius();

			sf::Vector2f newVel, newColVel;
			sf::Vector2f curVel = m.currentAcc * m.currentVel;
			sf::Vector2f curColVel = colM.currentAcc * colM.currentVel;

			newVel.x = (m. * () + ()) / ();

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
}
void EnemyCollision::floorCollision(anax::Entity entity, int tileX, int tileY){
	MovementComponent& m = entity.getComponent<MovementComponent>();
	BodyComponent& b = entity.getComponent<BodyComponent>();

	if(m.currentAcc.y < 0 || !b.collisionGrid[DirectionEnum::S]) return;

	m.currentAcc.y = -m.currentAcc.y;

	sf::CircleShape* body = (sf::CircleShape*)b.getShape("main");

	body->setPosition(body->getPosition().x,
			((tileY + 1) * TILESIZE) - body->getGlobalBounds().height);
}
void EnemyCollision::wallCollision(anax::Entity entity, int tileX, int tileY){
	MovementComponent& m = entity.getComponent<MovementComponent>();
	BodyComponent& b = entity.getComponent<BodyComponent>();
	sf::Shape* body = b.getShape("main");

	if(b.collisionGrid[DirectionEnum::E] && m.currentAcc.x > 0){
		// RIGHT
		m.currentAcc.x = -m.currentAcc.x;
		body->setPosition(((tileX + 1) * TILESIZE) - body->getGlobalBounds().width, body->getPosition().y);
	}else if(b.collisionGrid[DirectionEnum::W] && m.currentAcc.x < 0){
		// LEFT
		m.currentAcc.x = -m.currentAcc.x;
		body->setPosition(((tileX - 1) * TILESIZE) + TILESIZE,
				body->getPosition().y);
	}
}
