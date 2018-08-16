/*
 * LivesComponent.hpp
 *
 *  Created on: 14 Jul 2018
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_COMPONENTS_LIVESCOMPONENT_HPP_
#define LIBADO_ENTITY_COMPONENTS_LIVESCOMPONENT_HPP_

struct LivesComponent : anax::Component{
	LivesComponent(int num) : livesCounter(num){}

	int getLivesCounter() const {
		return livesCounter;
	}
	void decrementCounter(int livesCounter) {
		this->livesCounter -= livesCounter;
	}
private:
	int livesCounter;
};

#endif /* LIBADO_ENTITY_COMPONENTS_LIVESCOMPONENT_HPP_ */
