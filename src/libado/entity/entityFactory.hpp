/*
 * entityFactory.hpp
 *
 *  Created on: 27 Feb 2016
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_ENTITYFACTORY_HPP_
#define LIBADO_ENTITY_ENTITYFACTORY_HPP_

#include <map>
#include <anax/World.hpp>

#include "components/componentsCollection.hpp"
#include "../util/fileLogger.hpp"
#include "../util/lineShape.hpp"

struct EntityFactory{
	anax::World* world;

	rapidjson::Document entityProfiles;

	EntityFactory(){}
	EntityFactory(std::string jsonPath, anax::World* world);

	rapidjson::Value& getRawEntityData(std::string name);
	anax::Entity createEntity(std::string name);
	void loadEntityProfiles(std::string jsonPath);
	void saveEntity(anax::Entity& e, rapidjson::Value& entData,
			rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& dAlloc);
	void loadEntity(anax::Entity& e, rapidjson::Value& jsonData);
	anax::Entity& getPlayer();
private:
	anax::Entity player;
	std::mt19937_64 gen;
};

#endif /* LIBADO_ENTITY_ENTITYFACTORY_HPP_ */
