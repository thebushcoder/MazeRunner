/*
 * positionComponent.hpp
 *
 *  Created on: 24 Mar 2018
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_COMPONENTS_POSITIONCOMPONENT_HPP_
#define LIBADO_ENTITY_COMPONENTS_POSITIONCOMPONENT_HPP_

#include <anax/Component.hpp>
#include "serialComponent.hpp"

struct PositionComponent : anax::Component, public SerializableComponent{
	sf::Vector2f offsetPosition;
	sf::Vector2f screenPosition;

	PositionComponent(){}

	void setPosition(float screenX, float screenY){
		screenPosition.x = screenX;
		screenPosition.y = screenY;
	}

    bool save(rapidjson::Value& entData,
			rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& dAlloc){
    	rapidjson::Value t(rapidjson::kObjectType);

		t.AddMember("sX", screenPosition.x, dAlloc);
		t.AddMember("sY", screenPosition.y, dAlloc);

    	entData.AddMember("pos", t, dAlloc);
		return true;
	}
	bool load(rapidjson::Value& jsonData){
		screenPosition.x = jsonData["sX"].GetFloat();
		screenPosition.y = jsonData["sY"].GetFloat();
		return true;
	}
};



#endif /* LIBADO_ENTITY_COMPONENTS_POSITIONCOMPONENT_HPP_ */
