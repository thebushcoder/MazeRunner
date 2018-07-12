/*
 * enemyUtil.hpp
 *
 *  Created on: 3 Jul 2018
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_ENEMYUTIL_HPP_
#define LIBADO_ENTITY_ENEMYUTIL_HPP_

#include "entityFactory.hpp"
#include "../map/tileMap.hpp"

namespace EnemyUtil{
	void placeEnemy(TileMap* map, anax::Entity e, std::vector<anax::Entity>& list, float scrX, float scrY);
	void createEnemies(int numPerCp, TileMap* map, EntityFactory* factory,
			std::vector<anax::Entity>& checkpoints);
}

#endif /* LIBADO_ENTITY_ENEMYUTIL_HPP_ */
