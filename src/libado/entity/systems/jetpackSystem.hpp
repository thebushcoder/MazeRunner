/*
 * jetpackSystem.hpp
 *
 *  Created on: 29 Jul 2018
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_SYSTEMS_JETPACKSYSTEM_HPP_
#define LIBADO_ENTITY_SYSTEMS_JETPACKSYSTEM_HPP_

#include <anax/System.hpp>

#include "../components/componentsCollection.hpp"
#include "../entityFactory.hpp"

struct JetPackSystem : anax::System<anax::Requires<JetPackComponent>>{
	JetPackSystem(ParticleSystem* pSys, sf::RenderWindow* w, sf::View* v) :
		particleSys(pSys), window(w), mapView(v){}

	void update(sf::Time& delta){
		debug.restart();

		for(auto e : getEntities()){
			JetPackComponent& j = e.getComponent<JetPackComponent>();
			if(j.fireTurbo){
				turboThrust(j, e, delta);
			}else if(j.isFired){
				thrustEntity(j, e, delta);
			}else if(j.getCharge() < 1.0){
				regenCharge(j, delta);
			}
		}

//		printf("JetPackSystem > debugTime: %f\n", debug.restart().asSeconds());
	}

	// regenerate "x" amt fuel per second
	void regenCharge(JetPackComponent& j, sf::Time& delta){
		if(j.getCharge() < 1.0){
			j.addCharge(j.getRegenSpeed() * delta.asSeconds());
		}
	}

	void thrustEntity(JetPackComponent& j, anax::Entity e, sf::Time& delta){
		if(j.getCharge() > 0.0){

			checkVelocity(j, e);
			// drain charge; consumption = 1/2 fly speed / sec
			j.addCharge((j.getFlySpeed() * -j.getNormFuel()) * delta.asSeconds());

			JumpComponent& jump = e.getComponent<JumpComponent>();
			jump.inAir = true;

			// thrust entity in mouse direction
			calcThrustDir(j, e);

			j.setYVel(j.getFlySpeed() * delta.asSeconds());
			j.setXVel(j.getFlySpeed() * delta.asSeconds());

			// emit particles
			particleSys->createJetStream(2, e);
		}else{
			j.vel.x = j.vel.y = 0;
		}
	}

	void turboThrust(JetPackComponent& j, anax::Entity e, sf::Time& delta){
		//	consume turbo charge + init turbo
		if(!j.initTurbo){
			if(j.getCharge() >= 1.0 && j.getTurboCharges() > 0){
				checkVelocity(j, e);
				if(!sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)){
					j.setTurboCharges(j.getTurboCharges() - 1);
				}
				j.initTurbo = true;
				j.isFired = true;
			}
		}

		if(j.initTurbo){
			JumpComponent& jump = e.getComponent<JumpComponent>();
			jump.inAir = true;

			//	consume fuel
			j.addCharge((j.getFlySpeed() * -j.getTurboFuel()) * delta.asSeconds());

			calcThrustDir(j, e);

			//	set speed
			j.setYTurboVel((j.getFlySpeed() * j.getTurboMulti()) * delta.asSeconds());
			j.setXTurboVel((j.getFlySpeed() * j.getTurboMulti()) * delta.asSeconds());

			// emit particles
			particleSys->createTurboStream(3, e);

			//	end turbo thrust
			if(j.getCharge() <= 0.0){
				j.resetJetpack();
			}
		}
	}

private:
	ParticleSystem* particleSys;
	sf::RenderWindow* window;
	sf::View* mapView;

	sf::Clock debug;

	// calculate entity thrust direction(in mouse direction)
	void calcThrustDir(JetPackComponent& j, anax::Entity e){
		sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
		sf::Vector2f worldVec = window->mapPixelToCoords(mousePos, *(mapView));

		PositionComponent& p = e.getComponent<PositionComponent>();

		// get normalized direction player to mouse
		float magnitude = sf::LineShape::calcLineMag(p.screenPosition, worldVec);
		float lenX = (worldVec.x - p.screenPosition.x);
		float lenY = (worldVec.y - p.screenPosition.y);
		j.dir.x = lenX / magnitude;
		j.dir.y = lenY / magnitude;

		SpriteComponent& s = e.getComponent<SpriteComponent>();
		if(j.dir.x > 0 && !s.isSpriteFlipped()){
			s.flipSpriteRight();
		}else if(j.dir.x < 0 && s.isSpriteFlipped()){
			s.flipSpriteLeft();
		}
	}

	void checkVelocity(JetPackComponent& j, anax::Entity e){
		MovementComponent& m = e.getComponent<MovementComponent>();

		if((j.vel.x == 0 && j.vel.y == 0) && (m.currentVel.x != 0 || m.currentVel.y != 0)){
			j.vel.x = m.currentVel.x;
			if(m.currentVel.y < 0){
				j.vel.y = std::abs(m.currentVel.y);
			}
		}
	}
};

#endif /* LIBADO_ENTITY_SYSTEMS_JETPACKSYSTEM_HPP_ */
