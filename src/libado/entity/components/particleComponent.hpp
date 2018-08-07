/*
 * particleComponent.hpp
 *
 *  Created on: 25 Jun 2018
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_COMPONENTS_PARTICLECOMPONENT_HPP_
#define LIBADO_ENTITY_COMPONENTS_PARTICLECOMPONENT_HPP_

#include <SFML/Graphics.hpp>
#include <anax/Component.hpp>
#include <rapidjson/document.h>

struct ParticleComponent : anax::Component{
	sf::Time currentTime;

	ParticleComponent(rapidjson::Value& jsonData){
		maxTime = jsonData["maxT"].GetFloat();
		force = jsonData["f"].GetFloat();
	}

	sf::Vector2f getDirection() const {
		return direction;
	}
	void setDirection(sf::Vector2f d) {
		this->direction = d;
	}
	float getForce() const {
		return force;
	}
	float getMaxTime() const {
		return maxTime;
	}
	void setVelX(float v){
		if(vel.x + v <= force){
			vel.x += v;
		}else{
			vel.x = force;
		}
	}
	void setVelY(float v){
		if(vel.y + v <= force){
			vel.y += v;
		}else{
			vel.y = force;
		}
	}
	sf::Vector2f& getVel(){
		return vel;
	}
private:
	float maxTime, force;
	sf::Vector2f direction, vel;
};

#endif /* LIBADO_ENTITY_COMPONENTS_PARTICLECOMPONENT_HPP_ */
