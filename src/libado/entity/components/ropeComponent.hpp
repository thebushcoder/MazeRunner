/*
 * ropeComponent.hpp
 *
 *  Created on: 15 Apr 2018
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_COMPONENTS_ROPECOMPONENT_HPP_
#define LIBADO_ENTITY_COMPONENTS_ROPECOMPONENT_HPP_

struct RopeComponent : anax::Component{
	bool isFired = false, lButtonDown = false;
	anax::Entity rope;

	RopeComponent(){}

	const anax::Entity& getRopeEntity() const {
		return ropeEntity;
	}
	void setRopeEntity(anax::Entity ropeEntity) {
		this->ropeEntity = ropeEntity;
	}
private:
	anax::Entity ropeEntity;
};



#endif /* LIBADO_ENTITY_COMPONENTS_ROPECOMPONENT_HPP_ */
