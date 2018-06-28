/*
 * RenderSystem.hpp
 *
 *  Created on: 26 Feb 2016
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_SYSTEMS_RENDERSYSTEM_HPP_
#define LIBADO_ENTITY_SYSTEMS_RENDERSYSTEM_HPP_

#include <anax/System.hpp>

#include <SFML/Graphics/RenderTarget.hpp>

#include "../components/componentsCollection.hpp"

struct RenderingSystem : anax::System<anax::Requires<SpriteComponent>>{
    /// Construct a SpriteRenderingSystem with no render target
    RenderingSystem(){}

    /// Construct a SpriteRenderingSystem
    /// \param renderTarget The render target you wish to render to
    RenderingSystem(sf::RenderWindow* window){
    	m_renderWindow = window;
    	shaderClock.restart(); // start the timer
    }

    /// Renders the system
    void render(sf::Time& delta){
		for (auto& entity : getEntities()) {
			SpriteComponent& s = entity.getComponent<SpriteComponent>();
			PositionComponent& p = entity.getComponent<PositionComponent>();

			if(entity.hasComponent<RopeDetailsComponent>()){
				for(auto& i : s.getImgComponents()){
					m_renderWindow->draw(*i);
				}
			}else{
				if(entity.hasComponent<AnimationComponent>() &&
						entity.getComponent<AnimationComponent>().getCanGlow()){
					sf::Shader* shader = entity.getComponent<AnimationComponent>().getGlowShader();

					// set elapsed time for shader. Needed for timed(delta-like) operations
					shader->setUniform("u_time", shaderClock.getElapsedTime().asSeconds());

					for(auto& i : s.getImgComponents()){
						i->setPosition(p.screenPosition.x, p.screenPosition.y);
						i->move(p.offsetPosition);
						m_renderWindow->draw(*i, shader);
					}
				}else{
					for(auto& i : s.getImgComponents()){
						i->setPosition(p.screenPosition.x, p.screenPosition.y);
						i->move(p.offsetPosition);
						m_renderWindow->draw(*i);
					}
				}
			}

//			if(entity.hasComponent<CheckpointComponent>()){
//				BodyComponent& b = entity.getComponent<BodyComponent>();
//
//				b.getShape("main")->setPosition(p.screenPosition.x, p.screenPosition.y);
//				b.getShape("main")->setFillColor(sf::Color::Magenta);
//				m_renderWindow->draw(*b.getShape("main"));
//			}
		}
    }

    /// Sets the render target
    /// \param renderTarget
    void setRenderTarget(sf::RenderWindow* window){
    	m_renderWindow = window;
    }

    /// \return The render target this system is rendering to
    sf::RenderWindow& getRenderWindow() const{
    	return *m_renderWindow;
    }

    /// Determines if this system is valid or not
    /// \return true if this system is value, false otherwise
    bool isValid() const{
    	return true;
    }

private:
    /// The render target to render to
    sf::RenderWindow* m_renderWindow;

	sf::Clock shaderClock;
};


#endif /* LIBADO_ENTITY_SYSTEMS_RENDERSYSTEM_HPP_ */