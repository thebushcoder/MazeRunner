/*
 * animationSystem.hpp
 *
 *  Created on: 18 Jun 2018
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_SYSTEMS_ANIMATIONSYSTEM_HPP_
#define LIBADO_ENTITY_SYSTEMS_ANIMATIONSYSTEM_HPP_

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <anax/System.hpp>

#include "../components/componentsCollection.hpp"

struct AnimationSystem : anax::System<anax::Requires<AnimationComponent>>{
	AnimationSystem(){
		shaderClock.restart(); // start the timer
	}

	void update(sf::Time& delta){
		for(auto e : getEntities()){
			PositionComponent& p = e.getComponent<PositionComponent>();
			AnimationComponent& a = e.getComponent<AnimationComponent>();

			updateHover(delta, p, a);

			if(e.hasComponent<ShaderComponent>()){
				e.getComponent<ShaderComponent>().getShader().setUniform("u_time",
						shaderClock.getElapsedTime().asSeconds());
			}
		}
	}

	void updateHover(sf::Time& delta, PositionComponent& p, AnimationComponent& a){
		if(a.getHoverSpeed() != 0){
			if(std::abs(p.offsetPosition.y) >= a.getHoverOffset()){
				a.toggleHoverDir();
			}

			float off = a.getHoverSpeed() * delta.asSeconds();
			off *= a.getHoverDir();
			p.offsetPosition.y += off;
		}
	}

private:
	sf::Clock shaderClock;
};

#endif /* LIBADO_ENTITY_SYSTEMS_ANIMATIONSYSTEM_HPP_ */
