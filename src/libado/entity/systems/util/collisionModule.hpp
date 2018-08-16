/*
 * collisionModule.hpp
 *
 *  Created on: 8 Jul 2018
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_SYSTEMS_UTIL_COLLISIONMODULE_HPP_
#define LIBADO_ENTITY_SYSTEMS_UTIL_COLLISIONMODULE_HPP_

#include "../../../map/tileMap.hpp"

class CollisionModule{
public:
	CollisionModule(TileMap* m, bool preC, bool postC) : pre(preC), post(postC), map(m){}
	virtual ~CollisionModule(){}

	virtual void preCheck(anax::Entity entity){}
	virtual void postCheck(anax::Entity entity){}
	virtual void roofCollision(anax::Entity entity, int tileX, int tileY){}
	virtual void floorCollision(anax::Entity entity, int tileX, int tileY){}
	virtual void wallCollision(anax::Entity entity, int tileX, int tileY){}
	virtual void entityCollision(anax::Entity entity, anax::Entity collider, int tileX, int tileY){}

	bool hasPostCheck(){
		return post;
	}
	bool hasPreCheck(){
		return pre;
	}

protected:
	bool pre = false, post = false;
	TileMap* map;
};



#endif /* LIBADO_ENTITY_SYSTEMS_UTIL_COLLISIONMODULE_HPP_ */
