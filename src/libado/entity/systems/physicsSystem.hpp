/*
 * physicsSystem.hpp
 *
 *  Created on: 27 Feb 2016
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_SYSTEMS_PHYSICSSYSTEM_HPP_
#define LIBADO_ENTITY_SYSTEMS_PHYSICSSYSTEM_HPP_
#define _USE_MATH_DEFINES

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <anax/System.hpp>
#include <map>
#include <math.h>

#include "../components/componentsCollection.hpp"
#include "../../util/quadtree.hpp"
#include "../../util/lineShape.hpp"
#include "../../map/tileMap.hpp"
#include "directionEnum.hpp"

static float const GRAVITY = 9.00f;

struct PhysicsSystem : anax::System<anax::Requires<BodyComponent, MovementComponent>>{
	PhysicsSystem(TileMap* m) : map(m){}

    /// Updates the MovementSystem
    /// \param deltaTime The change in time
    void update(sf::Time& delta){
    	debug.restart();

    	for (auto entity : getEntities()){
			PositionComponent& p = entity.getComponent<PositionComponent>();
			MovementComponent& s = entity.getComponent<MovementComponent>();
			BodyComponent& b = entity.getComponent<BodyComponent>();
			sf::Shape* body = b.getShape("main");

			int tileX = std::floor((p.screenPosition.x +
					(body->getGlobalBounds().width / 2)) / TILESIZE);
			int tileY = std::floor((p.screenPosition.y +
					(body->getGlobalBounds().height / 2)) / TILESIZE);

			map->getEntityLayer().removeEntity(tileX, tileY, entity.getId().index);

			if(entity.hasComponent<PlayerComponent>()){
				JetPackComponent& jet = entity.getComponent<JetPackComponent>();

				body->setPosition(p.screenPosition.x, p.screenPosition.y);

				if(jet.isFired && jet.getCharge() > 0.0){
					// remove gravity effects
					s.currentVel.y = 0;

					body->move(jet.vel.x * jet.dir.x, jet.vel.y * jet.dir.y);
				}else{
					// move player
					updatePlayer(entity, delta);

					body->move(s.currentVel.x, s.currentVel.y);
				}

			}else{
				// move an entity
				s.setXVec(s.getMaxAcc() * delta.asSeconds());
				s.setYVec(s.getMaxAcc() * delta.asSeconds());

				body->setPosition(p.screenPosition.x, p.screenPosition.y);
				body->move(s.currentAcc.x * s.currentVel.x, s.currentAcc.y * s.currentVel.y);
			}

			tileX = std::floor((body->getPosition().x +
					(body->getGlobalBounds().width / 2)) / TILESIZE);
			tileY = std::floor((body->getPosition().y +
					(body->getGlobalBounds().height / 2)) / TILESIZE);

			map->getEntityLayer().setEntity(tileX, tileY, entity.getId().index);
		}
//    	printf("PhysicsSystem > debugTime: %f\n", debug.restart().asSeconds());
    }

private:
    TileMap* map;

    sf::Clock debug;

    void updatePlayer(anax::Entity entity, sf::Time& delta){
		MovementComponent& s = entity.getComponent<MovementComponent>();
		JumpComponent& j = entity.getComponent<JumpComponent>();

		// jump and gravity
		updateY(s, j, delta);

    	// horizontal movement
		s.setXVec(s.currentAcc.x * delta.asSeconds());
		s.currentVel.x *= 0.962f;	// friction
		if(j.inAir){
			s.currentVel.x *= 0.982f;	// additional air friction - more natural arc
		}

		//	stop player moving if speed below min threshold(prevents infinite sliding)
		if(s.currentVel.x != 0 && std::abs(s.currentVel.x) < s.getMinAcc()){
			s.currentVel.x = 0;
		}
    }

    void updateY(MovementComponent& s, JumpComponent& j, sf::Time& delta){
    	//CALCULATE JUMP / Y POSITION
		if(j.jumping && j.jPressed){
			j.jumpTime += delta;

			// keep jumping?
			if(j.jumpTime.asSeconds() >= j.getMaxJumpTime()){
				j.toggleJump(false);
			}

			wallJump(s, j, delta);

			s.setYVec(-j.getJumpAcc() * delta.asSeconds());
		}

		// apply gravity
		if(j.inAir){
			s.currentVel.y += (GRAVITY * delta.asSeconds());
		}
    }

    void wallJump(MovementComponent& s, JumpComponent& j, sf::Time& delta){
    	if(j.wallStick != JumpComponent::NONE){
			// player - left
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
				if(j.wallStick == JumpComponent::LEFT){
					// short-x, upward jump
					s.currentAcc.x = s.getMaxAcc() * 3;
					j.smlWallJump += delta;
					if(j.smlWallJump.asSeconds() >= j.getMaxSmlWallJ()){
						j.wallStick = JumpComponent::NONE;
					}
					s.setYVec((-j.getJumpAcc() * 0.2) * delta.asSeconds());
				}else{
					// jump away from wall
					j.wallStick = JumpComponent::NONE;
				}
			// player - right
			}else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
				if(j.wallStick == JumpComponent::RIGHT){
					// short-x, upward jump
					s.currentAcc.x = -s.getMaxAcc() * 3;
					j.smlWallJump += delta;
					if(j.smlWallJump.asSeconds() >= j.getMaxSmlWallJ()){
						j.wallStick = JumpComponent::NONE;
					}
					s.setYVec((-j.getJumpAcc() * 0.2) * delta.asSeconds());
				}else{
					// jump away from wall
					j.wallStick = JumpComponent::NONE;
				}
			}
		}
    }
};

#endif /* LIBADO_ENTITY_SYSTEMS_PHYSICSSYSTEM_HPP_ */
