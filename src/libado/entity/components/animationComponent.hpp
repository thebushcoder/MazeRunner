/*
 * animationComponent.hpp
 *
 *  Created on: 18 Jun 2018
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_COMPONENTS_ANIMATIONCOMPONENT_HPP_
#define LIBADO_ENTITY_COMPONENTS_ANIMATIONCOMPONENT_HPP_

#include <SFML/Graphics.hpp>
#include <anax/Component.hpp>
#include <rapidjson/document.h>
#include <vector>
#include <string>

struct AnimationComponent : anax::Component{
	AnimationComponent(rapidjson::Value& jsonData);

	void toggleHoverDir(){
		if(hoverDir > 0){
			hoverDir = -1;
		}else{
			hoverDir = 1;
		}
	}
	int getHoverDir() const {
		return hoverDir;
	}
	float getHoverOffset() const {
		return hoverOffset;
	}
	float getHoverSpeed() const {
		return hoverSpeed;
	}
private:
	float hoverOffset, hoverSpeed;
	int hoverDir = 1;
};


#endif /* LIBADO_ENTITY_COMPONENTS_ANIMATIONCOMPONENT_HPP_ */
