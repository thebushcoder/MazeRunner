/*
 * jumpComponent.hpp
 *
 *  Created on: 27 Mar 2018
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_COMPONENTS_JUMPCOMPONENT_HPP_
#define LIBADO_ENTITY_COMPONENTS_JUMPCOMPONENT_HPP_

#include <SFML/System/Time.hpp>

struct JumpComponent : anax::Component{
	enum WallStickType{
		NONE,
		LEFT,
		RIGHT
	};
	bool inAir = true, jumping = false, jPressed = false, doubleJ = false;
	WallStickType wallStick = NONE;
	sf::Time jumpTime, smlWallJump;

	JumpComponent(float jAcc, float jTime, float jSmlWall, float jRest) :
		jumpAcc(jAcc), maxJumpTime(jTime), maxSmlWallJ(jSmlWall), jumpRest(jRest){}

	void toggleJump(bool f){
		if(f){
			if(inAir && doubleJ){
				jumpTime = jumpTime.Zero;
				doubleJ = false;
			}else{
				jumpTime = jumpTime.Zero;
				setInAir(true);
			}
		}
		jumping = f;
	}
	void setInAir(bool f){
		if(!f){
			doubleJ = true;
			wallStick = NONE;
			smlWallJump = smlWallJump.Zero;
		}
		inAir = f;
	}
	float getJumpAcc() const {
		return jumpAcc;
	}
	float getMaxJumpTime() const {
		return maxJumpTime;
	}
	float getMaxSmlWallJ() const {
		return maxSmlWallJ;
	}
	float getJumpRest() const {
		return jumpRest;
	}

private:
	float jumpAcc, maxJumpTime, maxSmlWallJ, jumpRest;
};

#endif /* LIBADO_ENTITY_COMPONENTS_JUMPCOMPONENT_HPP_ */
