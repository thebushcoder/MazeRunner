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
    }

    /// Renders the system
    void render(sf::Time& delta){
    	debug.restart();

		for (auto& entity : getEntities()) {
			SpriteComponent& s = entity.getComponent<SpriteComponent>();
			PositionComponent& p = entity.getComponent<PositionComponent>();

			sf::Shader* shader = nullptr;
			if(entity.hasComponent<ShaderComponent>()){
				shader = &entity.getComponent<ShaderComponent>().getShader();
			}

			if(s.hasTexture()){
				s.getSprite()->setPosition(p.screenPosition.x, p.screenPosition.y);
				s.getSprite()->move(p.offsetPosition);
				if(shader){
					m_renderWindow->draw(*s.getSprite(), shader);
				}else{
					m_renderWindow->draw(*s.getSprite());
				}
			}else{
				s.getShape()->setPosition(p.screenPosition.x, p.screenPosition.y);
				s.getShape()->move(p.offsetPosition);
				if(shader){
					m_renderWindow->draw(*s.getShape(), shader);
				}else{
					m_renderWindow->draw(*s.getShape());
				}
			}
		}
//		printf("RenderingSystem > debugTime: %f\n", debug.restart().asSeconds());
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

    sf::Clock debug;
};


#endif /* LIBADO_ENTITY_SYSTEMS_RENDERSYSTEM_HPP_ */
