/*
 * ropeDetailsComponent.hpp
 *
 *  Created on: 15 Apr 2018
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_COMPONENTS_ROPEDETAILSCOMPONENT_HPP_
#define LIBADO_ENTITY_COMPONENTS_ROPEDETAILSCOMPONENT_HPP_

struct RopeDetailsComponent : anax::Component{
	float extVel = 0, reelVel = 0;
	float swingVel = 0;	//	pendulum swing forces

	/*
	 * 	src = player pos
	 * 	anchor = rope anchor/end
	 * 	extEnd = rope end position when extending/firing (no collision)
	 * 	tgt = mouse click pos, anchor tgt
	 * 	normDir = normalized direction (src to extEnd/anchor)
	 */
	sf::Vector2f src, anchor, extEnd, tgt, normDir;

	float currentLen = 0;
	float tmpLen = 0;
	bool isAnchored = false;

	anax::Entity srcEntity;

	RopeDetailsComponent(float extS, float maxL, float reelS) : extendSpeed(extS),
			maxLen(maxL), reelAcc(reelS){}

	float getExtendSpeed() const {
		return extendSpeed;
	}
	float getMaxLen() const {
		return maxLen;
	}
	float getReelSpeed() const {
		return reelAcc;
	}
private:
	// constants
	float extendSpeed, maxLen, reelAcc;
};



#endif /* LIBADO_ENTITY_COMPONENTS_ROPEDETAILSCOMPONENT_HPP_ */
