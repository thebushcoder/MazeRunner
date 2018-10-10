/*
 * speedComponent.hpp
 *
 *  Created on: 24 Mar 2018
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_COMPONENTS_MOVEMENTCOMPONENT_HPP_
#define LIBADO_ENTITY_COMPONENTS_MOVEMENTCOMPONENT_HPP_

#define LEFT_NORM -1;
#define RIGHT_NORM 1;

struct MovementComponent : anax::Component{
	sf::Vector2f currentVel;
	sf::Vector2f currentAcc;

	MovementComponent(float s) : maxAcc(s), minAcc(s * 0.005){}

	float getMaxAcc(){
		return maxAcc;
	}
	float getMinAcc(){
		return minAcc;
	}
	void setXVel(float s){
		if(std::abs(currentVel.x + s) < maxAcc){
			currentVel.x += s;
		}
	}
	void setYVel(float s){
		if(std::abs(currentVel.y + s) < maxAcc){
			currentVel.y += s;
		}
	}

private:
	float maxAcc;
	float minAcc;
};

#endif /* LIBADO_ENTITY_COMPONENTS_MOVEMENTCOMPONENT_HPP_ */
