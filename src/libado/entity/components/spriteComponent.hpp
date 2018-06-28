/*
 * spriteComponent.hpp
 *
 *  Created on: 24 Mar 2018
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_COMPONENTS_SPRITECOMPONENT_HPP_
#define LIBADO_ENTITY_COMPONENTS_SPRITECOMPONENT_HPP_

#include <SFML/Graphics.hpp>

struct SpriteComponent : anax::Component{
	SpriteComponent(){}

	void addImgComponent(std::unique_ptr<sf::Shape>& comp){
		imgComponents.push_back(std::move(comp));
	}
	std::vector<std::unique_ptr<sf::Shape> >& getImgComponents(){
		return imgComponents;
	}
private:
	std::vector<std::unique_ptr<sf::Shape> > imgComponents;
};

#endif /* LIBADO_ENTITY_COMPONENTS_SPRITECOMPONENT_HPP_ */
