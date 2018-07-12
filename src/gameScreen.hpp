/*
 * gameScreen.hpp
 *
 *  Created on: 12 Mar 2018
 *      Author: linbob
 */

#ifndef GAMESCREEN_HPP_
#define GAMESCREEN_HPP_

#include <vector>
#include <string>
#include <memory>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <anax/World.hpp>
#include <anax/Entity.hpp>
#include <TGUI/TGUI.hpp>

#include <lua.hpp>
#define SOL_CHECK_ARGUMENTS
#include <sol.hpp>

#include "libado/screen.hpp"
#include "libado/screenManager.hpp"
#include "libado/worldView.hpp"
#include "libado/textureAtlas.hpp"
#include "libado/util/fileLogger.hpp"
#include "libado/gui/widgetLuaBind.hpp"
#include "libado/gui/minimap.hpp"
#include "libado/gui/checkpointWidget.hpp"
#include "libado/map/tileMap.hpp"
#include "libado/util/mazeGen.hpp"
#include "libado/entity/systems/renderSystem.hpp"
#include "libado/entity/systems/physicsSystem.hpp"
#include "libado/entity/systems/playerController.hpp"
#include "libado/entity/systems/ninjaRopeSystem.hpp"
#include "libado/entity/systems/animationSystem.hpp"
#include "libado/entity/systems/particleSystem.hpp"
#include "libado/entity/systems/collisionSystem.hpp"
#include "libado/entity/entityFactory.hpp"
#include "libado/entity/checkpointUtil.hpp"
#include "libado/entity/enemyUtil.hpp"

class GameScreen : public Screen{
public:
	~GameScreen(){}

	void init();
	int run();
	virtual void load(std::string path);
	virtual void save(std::string path = "");
	void update(sf::Time& delta);
	void render(sf::Time& delta);
	void resize(sf::Event& e);
	void handleInput(sf::Time& delta);
	void hookPlayerInput();
	void loadResources();
	TileMap* getTileMap(){
		return tileMap.get();
	}
	std::vector<anax::Entity> getCheckpoints(){
		return checkpoints;
	}
	EntityFactory* getEntityFactory(){
		return entFactory.get();
	}
protected:
	std::shared_ptr<TextureAtlas> atlas;

	std::shared_ptr<anax::World> entityWorld;
	std::shared_ptr<EntityFactory> entFactory;

	std::shared_ptr<PlayerController> controller;
	std::shared_ptr<NinjaRopeSystem> ropeSys;

	std::shared_ptr<RenderingSystem> renderSys;
	std::shared_ptr<AnimationSystem> animSys;
	std::shared_ptr<PhysicsSystem> physicsSys;
	std::shared_ptr<CollisionSystem> collisionSys;
	std::shared_ptr<ParticleSystem> particleSys;

	std::shared_ptr<TileMap> tileMap;
	std::unique_ptr<WorldView> mapView;

	std::vector<anax::Entity> checkpoints;

	sol::state lua;
};



#endif /* GAMESCREEN_HPP_ */
