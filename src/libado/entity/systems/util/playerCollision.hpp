/*
 * playerCollision.hpp
 *
 *  Created on: 8 Jul 2018
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_SYSTEMS_PLAYERCOLLISION_HPP_
#define LIBADO_ENTITY_SYSTEMS_PLAYERCOLLISION_HPP_

#include <anax/Entity.hpp>
#include "../../components/componentsCollection.hpp"
#include "../../../map/tileMap.hpp"
#include "../directionEnum.hpp"
#include "collisionModule.hpp"
#include "../particleSystem.hpp"

class GameScreen;
class PlayerCollision : public CollisionModule{
public:
	PlayerCollision(TileMap* map, GameScreen* s);
	~PlayerCollision();

	void preCheck(anax::Entity entity);
	void postCheck(anax::Entity entity);
	void roofCollision(anax::Entity entity, int tileX, int tileY);
	void floorCollision(anax::Entity entity, int tileX, int tileY);
	void wallCollision(anax::Entity entity, int tileX, int tileY);
	void entityCollision(anax::Entity entity, anax::Entity collider, int tileX, int tileY);

private:
	GameScreen* screen;
	ParticleSystem* particleSys;

	void damagePlayer(anax::Entity entity);

	void destroyTile(int tileX, int tileY, JetPackComponent& jet){
		map->getTileLayer().deleteTile(tileX, tileY);
		particleSys->createTileExplosion(12,
				sf::Vector2f((tileX * TILESIZE) + TILESIZE / 2,
						(tileY * TILESIZE) + TILESIZE / 2), TILESIZE / 2);
	}

	void setRoofCollision(float bodyX, float bodyY, sf::RectangleShape* body, JumpComponent& j,
			MovementComponent& s){
		body->setPosition(bodyX, bodyY);

		if(j.jumping){
			j.toggleJump(false);
		}
		s.currentVel.y = 0;
	}
	void setFloorCollision(float bodyX, float bodyY, sf::RectangleShape* body, JumpComponent& j,
			MovementComponent& s){
		body->setPosition(bodyX, bodyY);
		if(j.inAir){
			j.setInAir(false);
		}
		if(j.jumping){
			j.toggleJump(false);
		}
		s.currentVel.y = 0;
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
	bool boundaryCheck(int tileX, int tileY){
		if(tileX <= 0 || tileY <= 0 || tileX >= map->getWidth() || tileY >= map->getHeight()){
			printf("GOT HERE\n");
			return true;
		}
		return false;
	}
};

#endif /* LIBADO_ENTITY_SYSTEMS_PLAYERCOLLISION_HPP_ */
