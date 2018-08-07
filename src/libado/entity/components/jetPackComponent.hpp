/*
 * jetPackComponent.hpp
 *
 *  Created on: 29 Jul 2018
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_COMPONENTS_JETPACKCOMPONENT_HPP_
#define LIBADO_ENTITY_COMPONENTS_JETPACKCOMPONENT_HPP_

#include <SFML/Graphics.hpp>

struct JetPackComponent : anax::Component{
	bool isFired = false, fireTurbo = false;
	sf::Vector2f vel;
	sf::Vector2f dir;

	JetPackComponent(rapidjson::Value& jsonData){
		flySpeed = jsonData["flyS"].GetFloat();
		regenSpeed = jsonData["regen"].GetFloat();
		turboCharges = jsonData["turbo"].GetInt();
		maxTurbo = jsonData["maxTurbo"].GetInt();
	}

	void setXVec(float s){
		if(std::abs(vel.x + s) < flySpeed){
			vel.x += s;
		}
	}
	void setYVec(float s){
		if(std::abs(vel.y + s) < flySpeed){
			vel.y += s;
		}
	}

	float getCharge() const {
		return charge;
	}
	void setCharge(float charge) {
		this->charge = charge;
	}
	void addCharge(float charge) {
		this->charge += charge;

		if(charge > 1.0) charge = 1.0;
	}
	float getFlySpeed() const {
		return flySpeed;
	}
	float getRegenSpeed() const {
		return regenSpeed;
	}
	int getTurboCharges() const {
		return turboCharges;
	}
	void setTurboCharges(int turboCharges) {
		this->turboCharges = turboCharges;
	}
	int getMaxTurboCharges(){
		return maxTurbo;
	}
private:
	float charge = 1.0;
	float flySpeed;
	float regenSpeed;
	int turboCharges;
	int maxTurbo;
};

#endif /* LIBADO_ENTITY_COMPONENTS_JETPACKCOMPONENT_HPP_ */
