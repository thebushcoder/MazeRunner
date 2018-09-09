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
	void flipSpriteLeft(){
		sprite->setScale(-1.0, 1.0);
		sprite->setOrigin({sprite->getLocalBounds().width, 0});
		spriteFlip = false;
	}
	void flipSpriteRight(){
		sprite->setScale(1.0, 1.0);
		sprite->setOrigin({0, 0});
		spriteFlip = true;
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
	bool isSpriteFlipped(){
		return spriteFlip;
	}
private:
	std::unique_ptr<sf::Texture> tex;
	std::unique_ptr<sf::Shape> shape;
	std::unique_ptr<sf::Sprite> sprite;

	bool spriteFlip = false;
};

#endif /* LIBADO_ENTITY_COMPONENTS_SPRITECOMPONENT_HPP_ */
