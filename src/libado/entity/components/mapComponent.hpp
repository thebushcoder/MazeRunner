/*
 * mapComponent.hpp
 *
 *  Created on: 12 Jul 2018
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_COMPONENTS_MAPCOMPONENT_HPP_
#define LIBADO_ENTITY_COMPONENTS_MAPCOMPONENT_HPP_

struct MapComponent : anax::Component{
	MapComponent(sf::Color c) : col(c){}

	sf::Color& getColor(){
		return col;
	}
private:
	sf::Color col;
};

#endif /* LIBADO_ENTITY_COMPONENTS_MAPCOMPONENT_HPP_ */
