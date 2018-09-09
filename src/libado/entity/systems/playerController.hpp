/*
 * playerController.hpp
 *
 *  Created on: 2 Apr 2018
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_SYSTEMS_PLAYERCONTROLLER_HPP_
#define LIBADO_ENTITY_SYSTEMS_PLAYERCONTROLLER_HPP_

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <map>

#include <anax/System.hpp>

#include "../entityFactory.hpp"
#include "../components/componentsCollection.hpp"

struct PlayerController : anax::System<anax::Requires<PlayerComponent>>{
	PlayerController(EntityFactory* f, sf::RenderWindow* w, sf::View* v) :
		factory(f), window(w), mapView(v){}
	void update(sf::Time& delta){
		for(auto e : getEntities()){
			MovementComponent& s = e.getComponent<MovementComponent>();
			JumpComponent& j = e.getComponent<JumpComponent>();
			SpriteComponent& sprite = e.getComponent<SpriteComponent>();
			JetPackComponent& jet = e.getComponent<JetPackComponent>();

			// FIRE JETPACK
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)){
				e.getComponent<JetPackComponent>().isFired = true;
			}else if(!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) &&
					jet.isFired && !jet.fireTurbo){
//				e.getComponent<JetPackComponent>().isFired = false;
				e.getComponent<JetPackComponent>().resetJetpack();
			}

			// FIRE JETPACK TURBO
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)){
				e.getComponent<JetPackComponent>().fireTurbo = true;
			}else if(!sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) &&
					jet.fireTurbo){
				e.getComponent<JetPackComponent>().fireTurbo = false;
				e.getComponent<JetPackComponent>().resetJetpack();
			}

			// MOVE LEFT
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
				s.currentAcc.x = -1;
				if(sprite.isSpriteFlipped()){
					sprite.flipSpriteLeft();
				}
			}else if(!sf::Keyboard::isKeyPressed(sf::Keyboard::A) &&
					s.currentAcc.x < 0){
				s.currentAcc.x = 0;
			}
			// MOVE RIGHT
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
				s.currentAcc.x = 1;
				if(!sprite.isSpriteFlipped()){
					sprite.flipSpriteRight();
				}
			}else if(!sf::Keyboard::isKeyPressed(sf::Keyboard::D) &&
					s.currentAcc.x > 0){
				s.currentAcc.x = 0;
			}

			if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){

			}else if(!sf::Keyboard::isKeyPressed(sf::Keyboard::S)){

			}

			// JUMP
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) ||
					sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
				// DO NOTHING - prevents bunny hopping/pogo sticking
				if(j.jPressed){
					return;
				}

				if(!j.inAir && !j.jumping){
					// REGULAR JUMP
					j.toggleJump(true);
					j.jPressed = true;
				}else if(j.wallStick != JumpComponent::NONE && j.smlWallJump == j.smlWallJump.Zero){
					// WALL JUMP
					j.toggleJump(true);
					s.currentVel.y = 0;
					j.jPressed = true;
				}else if(j.inAir && j.doubleJ){
					// DOUBLE JUMP
					j.toggleJump(true);
					s.currentVel.y = 0;
					j.jPressed = true;
				}
			}else if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Space) &&
					!sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
				if(j.jPressed){
					j.jPressed = false;
				}
				if(j.jumping){
					j.toggleJump(false);
				}
			}
		}
	}
private:
	EntityFactory* factory;
	sf::RenderWindow* window;
	sf::View* mapView;
};



#endif /* LIBADO_ENTITY_SYSTEMS_PLAYERCONTROLLER_HPP_ */
