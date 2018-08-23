/*
 * enemyCollision.hpp
 *
 *  Created on: 8 Jul 2018
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_SYSTEMS_ENEMYCOLLISION_HPP_
#define LIBADO_ENTITY_SYSTEMS_ENEMYCOLLISION_HPP_

#include <anax/Entity.hpp>
#include "../../components/componentsCollection.hpp"
#include "../../../map/tileMap.hpp"
#include "../../../map/mazeTileMap.hpp"
#include "../directionEnum.hpp"
#include "collisionModule.hpp"

class EnemyCollision : public CollisionModule{
public:
	EnemyCollision(TileMap* map);
	~EnemyCollision();

	void preCheck(anax::Entity entity);
	void postCheck(anax::Entity entity);
	void roofCollision(anax::Entity entity, int tileX, int tileY);
	void floorCollision(anax::Entity entity, int tileX, int tileY);
	void wallCollision(anax::Entity entity, int tileX, int tileY);
	void entityCollision(anax::Entity entity, anax::Entity collider, int tileX, int tileY);

};



#endif /* LIBADO_ENTITY_SYSTEMS_ENEMYCOLLISION_HPP_ */
