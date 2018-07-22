/*
 * particleSystem.hpp
 *
 *  Created on: 25 Jun 2018
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_SYSTEMS_PARTICLESYSTEM_HPP_
#define LIBADO_ENTITY_SYSTEMS_PARTICLESYSTEM_HPP_

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <anax/System.hpp>

#include "../components/componentsCollection.hpp"
#include "../entityFactory.hpp"

struct ParticleSystem : anax::System<anax::Requires<ParticleComponent>>{
	ParticleSystem(EntityFactory* f) : factory(f){
		std::random_device rd;
		gen = std::mt19937_64(rd());
		normDist = std::uniform_real_distribution<>(-1.0, 1.0);
	}

	void update(sf::Time& delta){
		for(auto e : getEntities()){
			PositionComponent& pos = e.getComponent<PositionComponent>();
			ParticleComponent& particle = e.getComponent<ParticleComponent>();

			// calc velocity
			particle.setVelX(particle.getForce() * delta.asSeconds());
			particle.setVelY(particle.getForce() * delta.asSeconds());

			// move particle
			pos.screenPosition.x += particle.getDirection().x * particle.getVel().x;
			pos.screenPosition.y += particle.getDirection().y * particle.getVel().y;

			// kill particle if reached max time
			particle.currentTime += delta;
			if(particle.currentTime.asSeconds() >= particle.getMaxTime()){
				e.kill();
			}
		}
	}

	void createExplosion(int numParticle, sf::Vector2f center){
		for(int i = 0; i < numParticle; ++i){
			anax::Entity e = factory->createEntity("cpParticle");

			PositionComponent& pos = e.getComponent<PositionComponent>();
			ParticleComponent& particle = e.getComponent<ParticleComponent>();

			pos.screenPosition = center;

			//	set particle move direction
			particle.setDirection(sf::Vector2f(normDist(gen), normDist(gen)));
		}
	}

private:
	EntityFactory* factory;

	std::mt19937_64 gen;
	std::uniform_real_distribution<> normDist;
};


#endif /* LIBADO_ENTITY_SYSTEMS_PARTICLESYSTEM_HPP_ */
