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
			if(j.isFired){
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
			// drain charge; consumption = 1/2 fly speed / sec
			j.addCharge((j.getFlySpeed() * -0.16) * delta.asSeconds());

			JumpComponent& jump = e.getComponent<JumpComponent>();
			jump.inAir = true;

			// thrust entity in mouse direction
			sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
			sf::Vector2f worldVec = window->mapPixelToCoords(mousePos, *(mapView));

			PositionComponent& p = e.getComponent<PositionComponent>();

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

			j.setYVec(j.getFlySpeed() * delta.asSeconds());
			j.setXVec(j.getFlySpeed() * delta.asSeconds());

			// emit particles
			particleSys->createJetStream(2, e);
		}else{
			j.vel.x = j.vel.y = 0;
		}
	}

	void turboThrust(anax::Entity e, sf::Time& delta){

	}

private:
	ParticleSystem* particleSys;
	sf::RenderWindow* window;
	sf::View* mapView;

	sf::Clock debug;
};

#endif /* LIBADO_ENTITY_SYSTEMS_JETPACKSYSTEM_HPP_ */
