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
	bool isFired = false, initTurbo = false, fireTurbo = false;
	sf::Vector2f vel;
	sf::Vector2f dir;

	JetPackComponent(rapidjson::Value& jsonData){
		flySpeed = jsonData["flyS"].GetFloat();
		regenSpeed = jsonData["regen"].GetFloat();
		normFuel = jsonData["fuel"].GetFloat();

		maxTurbo = jsonData["turbo"]["maxTurbo"].GetInt();
		turboCharges = jsonData["turbo"]["charges"].GetInt();
		turboMulti = jsonData["turbo"]["multi"].GetFloat();
		turboFuel = jsonData["turbo"]["fuel"].GetFloat();
	}

	void setXVel(float s){
		if(std::abs(vel.x + s) < flySpeed){
			vel.x += s;
		}
	}
	void setYVel(float s){
		if(std::abs(vel.y + s) < flySpeed){
			vel.y += s;
		}
	}
	void setXTurboVel(float s){
		float absVel = std::abs(vel.x + s);
		if(absVel < flySpeed * turboMulti){
			vel.x += s;
		}else if(absVel >= flySpeed * turboMulti){
			vel.x = flySpeed * turboMulti;
		}
	}
	void setYTurboVel(float s){
		float absVel = std::abs(vel.y + s);
		if(absVel < flySpeed * turboMulti){
			vel.y += s;
		}else if(absVel >= flySpeed * turboMulti){
			vel.y = flySpeed * turboMulti;
		}
	}

	void resetJetpack(){
		initTurbo = false;
		vel.x = vel.y = 0;
		isFired = false;
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
	const float getTurboMulti() const{
		return turboMulti;
	}
	int getMaxTurbo() const {
		return maxTurbo;
	}
	float getNormFuel() const {
		return normFuel;
	}
	float getTurboFuel() const {
		return turboFuel;
	}

private:
	float turboMulti;
	float charge = 1.0;
	float flySpeed;
	float normFuel, turboFuel;
	float regenSpeed;
	int turboCharges;
	int maxTurbo;
};

#endif /* LIBADO_ENTITY_COMPONENTS_JETPACKCOMPONENT_HPP_ */
