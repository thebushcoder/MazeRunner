/*
 * bodyComponent.hpp
 *
 *  Created on: 24 Mar 2018
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_COMPONENTS_BODYCOMPONENT_HPP_
#define LIBADO_ENTITY_COMPONENTS_BODYCOMPONENT_HPP_

#include <SFML/Graphics.hpp>

struct BodyComponent : anax::Component{
	/*		_	_	_
	 * 	  | _ | _ | _ |
	 * 	  | _ | _ | _ |
	 * 	  | _ | _ | _ |
	 */
	std::vector<bool> collisionGrid;

	bool swingCollision = false;

	BodyComponent(){
		initGrid();
	}

	void initGrid(){
		collisionGrid.resize(8);
	}

	void resetGrid(){
		for(int x = 0; x < collisionGrid.size(); ++x){
			collisionGrid[x] = false;
		}
	}

	void addBodyComponent(std::string key, std::unique_ptr<sf::Shape>& comp){
		bodyComponents[key] = std::move(comp);
	}
	sf::Shape* getShape(std::string key){
		return bodyComponents[key].get();
	}
	std::map<std::string, std::unique_ptr<sf::Shape> >& getBodyComponents(){
		return bodyComponents;
	}
	const std::string& getCollisionMod() const {
		return collisionMod;
	}
	void setCollisionMod(const std::string& collisionMod) {
		this->collisionMod = collisionMod;
	}
private:
	std::string collisionMod;
	std::map<std::string, std::unique_ptr<sf::Shape> > bodyComponents;
};

#endif /* LIBADO_ENTITY_COMPONENTS_BODYCOMPONENT_HPP_ */
