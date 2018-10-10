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
	PhysicsSystem(TileMap* m) : map(m){
	}

    /// \param deltaTime The change in time
    void update(sf::Time& delta){
    	debug.restart();

    	for (auto entity : getEntities()){
			PositionComponent& p = entity.getComponent<PositionComponent>();
			MovementComponent& s = entity.getComponent<MovementComponent>();
			BodyComponent& b = entity.getComponent<BodyComponent>();
			sf::Shape* body = b.getShape("main");

			sf::Vector2i tilePos = map->getTilePosition(p.screenPosition,
					body->getGlobalBounds().width, body->getGlobalBounds().height);

			map->getEntityLayer().removeEntity(tilePos.x, tilePos.y, entity.getId().index);

			if(entity.hasComponent<PlayerComponent>()){
				JetPackComponent& jet = entity.getComponent<JetPackComponent>();

				body->setPosition(p.screenPosition.x, p.screenPosition.y);

				if(jet.vel.x != 0 || jet.vel.y != 0){
					// remove gravity effects
					s.currentVel.y = 0;

					body->move(jet.vel.x * jet.dir.x, jet.vel.y * jet.dir.y);
				}else{
					// move player
					updatePlayer(entity, delta);

					body->move(s.currentAcc.x * s.currentVel.x, s.currentVel.y);
				}

			}else{
				// move an entity
				s.setXVel(s.getMaxAcc() * delta.asSeconds());
				s.setYVel(s.getMaxAcc() * delta.asSeconds());

				body->move(s.currentAcc.x * s.currentVel.x, s.currentAcc.y * s.currentVel.y);
			}

			tilePos = map->getTilePosition(body->getPosition(), body->getGlobalBounds().width,
					body->getGlobalBounds().height);

			map->getEntityLayer().setEntity(tilePos.x, tilePos.y, entity.getId().index);
		}
//    	printf("PhysicsSystem > debugTime: %f\n", debug.restart().asSeconds());
    }

private:
    TileMap* map;

    sf::Clock debug;

    void updatePlayer(anax::Entity entity, sf::Time& delta){
		MovementComponent& s = entity.getComponent<MovementComponent>();
		JumpComponent& j = entity.getComponent<JumpComponent>();

    	// horizontal movement
		if(s.currentAcc.x != 0 && !j.jumping){
			s.setXVel((s.getMaxAcc() * 3.0) * delta.asSeconds());
		}

		// jump and gravity
		updateY(s, j, entity, delta);

		if(j.inAir){
			s.currentVel.x *= 0.9989f;	// additional air friction - more natural arc
		}else if(s.currentAcc.x == 0){	// only apply friction when not receiving L/R input
			s.currentVel.x *= 0.992f;	// friction
		}

		//	stop player moving if speed below min threshold(prevents infinite sliding)
		if(s.currentVel.x != 0 && std::abs(s.currentVel.x) < s.getMinAcc()){
			s.currentVel.x = 0;
		}
    }

    void updateY(MovementComponent& s, JumpComponent& j, anax::Entity entity, sf::Time& delta){
    	//CALCULATE JUMP / Y POSITION
		if(j.jumping && j.jPressed){
			if(j.wallStick != JumpComponent::NONE){
				wallJump(s, j, entity, delta);
			}else{
				// keep jumping?
				if(j.jumpTime.asSeconds() >= j.getMaxJumpTime()){
					j.toggleJump(false);
				}else{
					j.jumpTime += delta;
					s.setYVel(-(j.getJumpAcc() * 3.0) * delta.asSeconds());
				}
			}
		}

		// apply gravity
		if(j.inAir){
			s.currentVel.y += (GRAVITY * delta.asSeconds());
		}
    }

    void wallJump(MovementComponent& s, JumpComponent& j, anax::Entity entity,
    		sf::Time& delta){
		BodyComponent& b = entity.getComponent<BodyComponent>();
		sf::Shape* body = b.getShape("main");

    	if(j.smlWallJump.asSeconds() >= j.getMaxSmlWallJ()){
			j.wallStick = JumpComponent::NONE;
			s.currentAcc.x = -s.currentAcc.x;
    	}else{
    		if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) &&
    				j.wallStick == JumpComponent::LEFT){
    			s.currentAcc.x = 1;
    			body->move(4, 0);
    		}else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D) &&
    				j.wallStick == JumpComponent::RIGHT){
    			s.currentAcc.x = -1;
    			body->move(-4, 0);
    		}

    		float diff = (j.getJumpAcc() * 1) * delta.asSeconds();
    		if(diff < s.getMaxAcc() * 0.5){
    			s.currentVel.x += diff;
    		}

			diff = (-j.getJumpAcc() * 2.5) * delta.asSeconds();
			if(diff < j.getJumpAcc()){
				s.currentVel.y += diff;
			}

			j.smlWallJump += delta;
    	}
    }
};

#endif /* LIBADO_ENTITY_SYSTEMS_PHYSICSSYSTEM_HPP_ */
