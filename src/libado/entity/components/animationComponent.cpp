/*
 * animationComponent.cpp
 *
 *  Created on: 23 Jun 2018
 *      Author: linbob
 */

#include "animationComponent.hpp"

AnimationComponent::AnimationComponent(rapidjson::Value& jsonData){
	if(jsonData.HasMember("hover")){
		hoverOffset = jsonData["hover"]["o"].GetFloat();
		hoverSpeed = jsonData["hover"]["s"].GetFloat();
	}
}
