/*
 * playerCollision.cpp
 *
 *  Created on: 8 Jul 2018
 *      Author: linbob
 */

#include "playerCollision.hpp"
#include "../../../../gameScreen.hpp"
#include "../../../../mainMenu.hpp"

PlayerCollision::PlayerCollision(TileMap* map, GameScreen* s) :
	CollisionModule(map, true, true), screen(s),
	particleSys(s->getParticleSystem()){}
PlayerCollision::~PlayerCollision(){}

void PlayerCollision::preCheck(anax::Entity entity){}

void PlayerCollision::postCheck(anax::Entity entity){
	BodyComponent& b = entity.getComponent<BodyComponent>();

	// check if still on floor
	if(!b.collisionGrid[DirectionEnum::Direction::SW] &&
			!b.collisionGrid[DirectionEnum::Direction::S] &&
			!b.collisionGrid[DirectionEnum::Direction::SE]){
		JumpComponent& j = entity.getComponent<JumpComponent>();
		j.setInAir(true);
	}
}

void PlayerCollision::damagePlayer(anax::Entity entity){
	if(!entity.hasComponent<InvulnerabilityComponent>()){
		LivesComponent& lC = entity.getComponent<LivesComponent>();
		lC.decrementCounter(1);

		if(lC.getLivesCounter() == 0){
			screen->setStatus(QUIT);
			MainMenu* s = new MainMenu();
			s->setManager(screen->getManager());
			screen->getManager()->setCurrentScreen(s);

		}else{
			float invulnTime = screen->getEntityFactory()->
					getRawEntityData("player")["invuln"].GetFloat();

			SpriteComponent& s = entity.getComponent<SpriteComponent>();
			entity.addComponent<InvulnerabilityComponent>(invulnTime);

			entity.addComponent<ShaderComponent>("bin/data/flash_shader",
					s.getTexture()).getShader().setUniform("u_time", 0.0f);;

			entity.activate();
		}
	}
}

void PlayerCollision::entityCollision(anax::Entity entity, anax::Entity collider,
		int tileX, int tileY){
	if(collider.hasComponent<CheckpointComponent>()){
		// trigger entity collision
		particleSys->createCPExplosion(16,
				collider.getComponent<PositionComponent>().screenPosition
		);

		map->getEntityLayer().removeEntity(tileX, tileY, collider.getId().index);

		collider.kill();
	}

	if(collider.hasComponent<EnemyComponent>()){
		if(!entity.hasComponent<InvulnerabilityComponent>()){
			damagePlayer(entity);
		}
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

			JetPackComponent& jet = entity.getComponent<JetPackComponent>();

			// tile top
			if(b.collisionGrid[DirectionEnum::N]){
				if(jet.fireTurbo && jet.isFired && !boundaryCheck(tileX, tileY - 1)){
					destroyTile(tileX, tileY - 1, jet);
				}else{
					Tile* t = map->getTileLayer().getTile(tileX, tileY - 1).get();

					if(t->getType() == Tile::Type::LAVA){
						damagePlayer(entity);
					}

					setRoofCollision(body->getPosition().x,
							t->getBody().getPosition().y +
							t->getBody().getGlobalBounds().height - 2,
							body, j, s);
				}
			}
			// tile top-LEFT
			if(b.collisionGrid[DirectionEnum::NW] &&
					!b.collisionGrid[DirectionEnum::W] &&
					(s.currentVel.y < 0 || jet.isFired)){
				if(jet.fireTurbo && jet.isFired && !boundaryCheck(tileX - 1, tileY - 1)){
					destroyTile(tileX - 1, tileY - 1, jet);
				}else{
					Tile* t = map->getTileLayer().getTile(tileX - 1, tileY - 1).get();
					if(t->getType() == Tile::Type::LAVA){
						damagePlayer(entity);
					}

					setRoofCollision(p.screenPosition.x, p.screenPosition.y,
							body, j, s);
				}
			}
			// tile top-RIGHT
			if(b.collisionGrid[DirectionEnum::NE] &&
					!b.collisionGrid[DirectionEnum::E] &&
					(s.currentVel.y < 0 || jet.isFired)){
				if(jet.fireTurbo && jet.isFired&& !boundaryCheck(tileX + 1, tileY - 1)){
					destroyTile(tileX + 1, tileY - 1, jet);
				}else{
					Tile* t = map->getTileLayer().getTile(tileX + 1, tileY - 1).get();
					if(t->getType() == Tile::Type::LAVA){
						damagePlayer(entity);
					}

					setRoofCollision(p.screenPosition.x, p.screenPosition.y,
							body, j, s);
				}
			}
		}
	}
}
void PlayerCollision::floorCollision(anax::Entity entity, int tileX, int tileY){
	JumpComponent& j = entity.getComponent<JumpComponent>();
	JetPackComponent& jet = entity.getComponent<JetPackComponent>();

	if(j.inAir){
		// if not jumping and jet is not firing into the ground
		if(!j.jumping && !(jet.isFired && jet.dir.y < 0.0)){
			BodyComponent& b = entity.getComponent<BodyComponent>();
			MovementComponent& s = entity.getComponent<MovementComponent>();
			sf::RectangleShape* body = (sf::RectangleShape*)b.getShape("main");
			// tile down
			if(b.collisionGrid[DirectionEnum::Direction::S]){
				if(jet.fireTurbo && jet.isFired && !boundaryCheck(tileX, tileY + 1)){
					destroyTile(tileX, tileY + 1, jet);
				}else{

					Tile* t = map->getTileLayer().getTile(tileX, tileY + 1).get();
					if(t->getType() == Tile::Type::LAVA){
						damagePlayer(entity);
					}

					setFloorCollision(body->getPosition().x,
							((tileY + 1) * TILESIZE) - body->getGlobalBounds().height,
							body, j, s);
				}
			}
			// tile down-left
			if(b.collisionGrid[DirectionEnum::Direction::SW] &&
					!b.collisionGrid[DirectionEnum::Direction::W]){
				if(jet.fireTurbo && jet.isFired && !boundaryCheck(tileX - 1, tileY + 1)){
					destroyTile(tileX - 1, tileY + 1, jet);
				}else{

					Tile* t = map->getTileLayer().getTile(tileX - 1, tileY + 1).get();
					if(t->getType() == Tile::Type::LAVA){
						damagePlayer(entity);
					}

					setFloorCollision(body->getPosition().x,
							((tileY + 1) * TILESIZE) - body->getGlobalBounds().height,
							body, j, s);
				}
			}
			// tile down-right
			if(b.collisionGrid[DirectionEnum::Direction::SE] &&
					!b.collisionGrid[DirectionEnum::Direction::E]){
				if(jet.fireTurbo && jet.isFired && !boundaryCheck(tileX + 1, tileY + 1)){
					destroyTile(tileX + 1, tileY + 1, jet);
				}else{

					Tile* t = map->getTileLayer().getTile(tileX + 1, tileY + 1).get();
					if(t->getType() == Tile::Type::LAVA){
						damagePlayer(entity);
					}

					setFloorCollision(body->getPosition().x,
							((tileY + 1) * TILESIZE) - body->getGlobalBounds().height,
							body, j, s);
				}
			}
		}
	}
}
void PlayerCollision::wallCollision(anax::Entity entity, int tileX, int tileY){
	JumpComponent& j = entity.getComponent<JumpComponent>();
	BodyComponent& b = entity.getComponent<BodyComponent>();
	MovementComponent& s = entity.getComponent<MovementComponent>();
	JetPackComponent& jet = entity.getComponent<JetPackComponent>();
	sf::RectangleShape* body = (sf::RectangleShape*)b.getShape("main");

	// if collide w/ west wall and moving horizontally(not on rope) or swinging on rope
	if(b.collisionGrid[DirectionEnum::Direction::W] && (s.currentVel.x < 0 ||
			jet.isFired)){
		if(jet.fireTurbo && jet.isFired && !boundaryCheck(tileX - 1, tileY)){
			destroyTile(tileX - 1, tileY, jet);
		}else{
			if(!jet.isFired){
				// stick to wall in air
				stickToWall(j, s, sf::Keyboard::A, JumpComponent::LEFT);
			}

			Tile* t = map->getTileLayer().getTile(tileX - 1, tileY).get();
			if(t->getType() == Tile::Type::LAVA){
				damagePlayer(entity);
			}

			body->setPosition(
					t->getBody().getPosition().x + t->getBody().getGlobalBounds().width - 2,
					body->getPosition().y);
			s.currentVel.x = 0;
		}
	// if collide w/ east wall and moving horizontally(not on rope) or swinging on rope
	}else if(b.collisionGrid[DirectionEnum::Direction::E] && (s.currentVel.x > 0 ||
			jet.isFired)){
		if(jet.fireTurbo && jet.isFired && !boundaryCheck(tileX + 1, tileY)){
			destroyTile(tileX + 1, tileY, jet);
		}else{
			if(!jet.isFired){
				// stick to wall in air
				stickToWall(j, s, sf::Keyboard::D, JumpComponent::RIGHT);
			}

			Tile* t = map->getTileLayer().getTile(tileX + 1, tileY).get();
			if(t->getType() == Tile::Type::LAVA){
				damagePlayer(entity);
			}

			body->setPosition(t->getBody().getPosition().x - body->getGlobalBounds().width - 2,
					body->getPosition().y);
			s.currentVel.x = 0;
		}
	}
}
