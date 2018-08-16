/*
 * checkpointUtil.hpp
 *
 *  Created on: 11 Jun 2018
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_CHECKPOINTUTIL_HPP_
#define LIBADO_ENTITY_CHECKPOINTUTIL_HPP_

#include "entityFactory.hpp"
#include "../map/tileMap.hpp"

namespace CheckpointUtil{
	void placeCheckpoint(TileMap* map, anax::Entity cp, std::vector<anax::Entity>& list, float scrX, float scrY);
	std::vector<anax::Entity> createCheckPoints(int num, TileMap* map, EntityFactory* factory);
};



#endif /* LIBADO_ENTITY_CHECKPOINTUTIL_HPP_ */
