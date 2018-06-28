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
	if(jsonData.HasMember("glow")){
		sf::Color minCol(jsonData["glow"]["min"][0].GetInt(),
				jsonData["glow"]["min"][1].GetInt(),
				jsonData["glow"]["min"][2].GetInt(), 255);

		sf::Color maxCol(jsonData["glow"]["max"][0].GetInt(),
				jsonData["glow"]["max"][1].GetInt(),
				jsonData["glow"]["max"][2].GetInt(), 255);

		glowShader.loadFromFile("bin/data/glow_shader", sf::Shader::Fragment);
		glowShader.setUniform("u_minColor", sf::Glsl::Vec4(minCol));
		glowShader.setUniform("u_maxColor", sf::Glsl::Vec4(maxCol));

		canGlow = true;
	}
}
