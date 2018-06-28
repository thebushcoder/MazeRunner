/*
 * directionEnum.hpp
 *
 *  Created on: 6 Jun 2018
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_SYSTEMS_DIRECTIONENUM_HPP_
#define LIBADO_ENTITY_SYSTEMS_DIRECTIONENUM_HPP_

namespace DirectionEnum{
	enum Direction{
		NW,
		N,
		NE,
		W,
		E,
		SW,
		S,
		SE
	};

	static sf::Vector2f getDirVec(Direction d){
		switch(d){
		case NW:
			return sf::Vector2f(-1, -1);
			break;
		case N:
			return sf::Vector2f(0, -1);
			break;
		case NE:
			return sf::Vector2f(1, -1);
			break;
		case W:
			return sf::Vector2f(-1, 0);
			break;
		case E:
			return sf::Vector2f(1, 0);
			break;
		case SW:
			return sf::Vector2f(-1, 1);
			break;
		case S:
			return sf::Vector2f(0, 1);
			break;
		case SE:
			return sf::Vector2f(1, 1);
			break;
		}
		return sf::Vector2f();
	}

}



#endif /* LIBADO_ENTITY_SYSTEMS_DIRECTIONENUM_HPP_ */
