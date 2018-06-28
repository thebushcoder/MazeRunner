/*
 * speedComponent.hpp
 *
 *  Created on: 24 Mar 2018
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_COMPONENTS_MOVEMENTCOMPONENT_HPP_
#define LIBADO_ENTITY_COMPONENTS_MOVEMENTCOMPONENT_HPP_

struct MovementComponent : anax::Component{
	sf::Vector2f currentVel;
	sf::Vector2f currentAcc;
	float tgtVel;

	MovementComponent(float s) : maxAcc(s), minAcc(s * 0.005){}

	float getMaxAcc(){
		return maxAcc;
	}
	float getMinAcc(){
		return minAcc;
	}
	void setXVec(float s){
		if(std::abs(currentVel.x + s) < maxAcc){
			currentVel.x += s;
		}
	}
	void setYVec(float s){
		if(std::abs(currentVel.y + s) < maxAcc){
			currentVel.y += s;
		}
	}

private:
	float maxAcc;
	float minAcc;
};

#endif /* LIBADO_ENTITY_COMPONENTS_MOVEMENTCOMPONENT_HPP_ */
