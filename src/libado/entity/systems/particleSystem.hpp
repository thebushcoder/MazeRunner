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

/*
 * POSSIBLE BUG WITH PARTICLE SPEED / FORCE
 */
struct ParticleSystem : anax::System<anax::Requires<ParticleComponent>>{
	ParticleSystem(EntityFactory* f) : factory(f){
		std::random_device rd;
		gen = std::mt19937_64(rd());
		normDist = std::uniform_real_distribution<>(-1.0, 1.0);
		normVaryDist = std::uniform_real_distribution<>(-0.8, 0.8);
		turboVaryDist = std::uniform_real_distribution<>(-1.2, 1.2);
		colourDist = std::uniform_int_distribution<>(0, 1);
	}

	void update(sf::Time& delta){
		debug.restart();

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
				continue;
			}

			if(e.hasComponent<ShaderComponent>()){
				ShaderComponent& shade = e.getComponent<ShaderComponent>();

				shade.getShader().setUniform("elapsedTime", particle.currentTime.asSeconds());
			}
		}

//		printf("ParticleSystem > debugTime: %f\n", debug.restart().asSeconds());
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

	void createJetStream(int numParticle, anax::Entity player){
		for(int i = 0; i < numParticle; ++i){
			//	create particle entity + set shader texture
			anax::Entity e = factory->createEntity("jetParticle");

			PositionComponent& pos = e.getComponent<PositionComponent>();
			ParticleComponent& particle = e.getComponent<ParticleComponent>();
			ShaderComponent& shade = e.getComponent<ShaderComponent>();
			SpriteComponent& s = e.getComponent<SpriteComponent>();
			shade.getShader().setUniform("tex", *s.getTexture());

			//	set particle's start position to player position
			PositionComponent& playerPos = player.getComponent<PositionComponent>();
			BodyComponent& b = player.getComponent<BodyComponent>();
			JetPackComponent& j = player.getComponent<JetPackComponent>();

			pos.screenPosition.x = playerPos.screenPosition.x +
					b.getShape("main")->getGlobalBounds().width * 0.5;
			pos.screenPosition.y = playerPos.screenPosition.y +
					b.getShape("main")->getGlobalBounds().height * 0.9;

			//	set particle move direction(normalized dir)
			particle.setDirection(sf::Vector2f(-(j.dir.x + normVaryDist(gen)),
					-(j.dir.y + normVaryDist(gen))));
		}
	}

	void createTurboStream(int numParticle, anax::Entity player){
		for(int i = 0; i < numParticle; ++i){
			//	create particle entity + set shader texture
			anax::Entity e = factory->createEntity("turboParticle");

			PositionComponent& pos = e.getComponent<PositionComponent>();
			ParticleComponent& particle = e.getComponent<ParticleComponent>();
			ShaderComponent& shade = e.getComponent<ShaderComponent>();
			SpriteComponent& s = e.getComponent<SpriteComponent>();
			shade.getShader().setUniform("tex", *s.getTexture());

			// set random base colour
			shade.getShader().setUniform("u_baseColor",
					sf::Glsl::Vec3(colourDist(gen),colourDist(gen),colourDist(gen)));

			//	set particle's start position to player position
			PositionComponent& playerPos = player.getComponent<PositionComponent>();
			BodyComponent& b = player.getComponent<BodyComponent>();
			JetPackComponent& j = player.getComponent<JetPackComponent>();

			pos.screenPosition.x = playerPos.screenPosition.x +
					b.getShape("main")->getGlobalBounds().width * 0.5;
			pos.screenPosition.y = playerPos.screenPosition.y +
					b.getShape("main")->getGlobalBounds().height * 0.9;

			//	set particle move direction(normalized dir)
			particle.setDirection(sf::Vector2f(-(j.dir.x + turboVaryDist(gen)),
					-(j.dir.y + turboVaryDist(gen))));
		}
	}

private:
	EntityFactory* factory;

	std::mt19937_64 gen;
	std::uniform_real_distribution<> normDist;
	std::uniform_real_distribution<> normVaryDist;
	std::uniform_real_distribution<> turboVaryDist;
	std::uniform_int_distribution<> colourDist;

	sf::Clock debug;
};


#endif /* LIBADO_ENTITY_SYSTEMS_PARTICLESYSTEM_HPP_ */
