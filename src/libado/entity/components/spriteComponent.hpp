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

	void setTexture(std::unique_ptr<sf::Texture>& t){
		tex = std::move(t);
		sprite = std::unique_ptr<sf::Sprite>(new sf::Sprite(*tex.get()));
	}
	void setShape(std::unique_ptr<sf::Shape>& s){
		shape = std::move(s);
	}
	sf::Sprite* getSprite(){
		return sprite.get();
	}
	sf::Texture* getTexture(){
		return tex.get();
	}
	sf::Shape* getShape(){
		return shape.get();
	}
	bool hasTexture(){
		return tex.get() != nullptr;
	}
	bool hasShape(){
		return tex.get() != nullptr;
	}
private:
	std::unique_ptr<sf::Texture> tex;
	std::unique_ptr<sf::Shape> shape;
	std::unique_ptr<sf::Sprite> sprite;
};

#endif /* LIBADO_ENTITY_COMPONENTS_SPRITECOMPONENT_HPP_ */
