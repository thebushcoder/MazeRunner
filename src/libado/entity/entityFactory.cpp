/*
 * entityFactory.cpp
 *
 *  Created on: 27 Feb 2016
 *      Author: linbob
 */

#include "entityFactory.hpp"

/*
 * STORES THE JSON PROFILE COLLECTION AND REFERENCEs IT DURING ENTITY CREATION
 */
EntityFactory::EntityFactory(std::string jsonPath, anax::World* world) : world(world){
	loadEntityProfiles(jsonPath);
}
//used for read only operations. DO NOT ADD TO JSON VALUE
rapidjson::Value& EntityFactory::getRawEntityData(std::string name){
	return entityProfiles[name.c_str()];
}

void EntityFactory::loadEntityProfiles(std::string jsonPath){
	char readBuffer[65536];

	FILE* fp = fopen(jsonPath.c_str(), "r"); // non-Windows use "r"
	rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
	entityProfiles.ParseStream(is);

	fclose(fp);
}

anax::Entity EntityFactory::createEntity(std::string name){
	anax::Entity e = world->createEntity();

	e.addComponent<PositionComponent>();

	rapidjson::Value& profile = entityProfiles[name.c_str()];
	//Must be non-const member iterator so that refs can be passed
	for (rapidjson::Value::MemberIterator itr = profile.MemberBegin();
	    itr != profile.MemberEnd(); ++itr)	{

		if(strcmp(itr->name.GetString(), "sprite") == 0){
			e.addComponent<SpriteComponent>();
			SpriteComponent& s = e.getComponent<SpriteComponent>();
			for (auto& i : itr->value.GetArray()){
				if(i.HasMember("rect")){
					std::unique_ptr<sf::Shape> r = std::unique_ptr<sf::Shape>(
							new sf::RectangleShape());
					((sf::RectangleShape*)r.get())->setSize(sf::Vector2f(
							i["rect"]["w"].GetFloat(),
							i["rect"]["h"].GetFloat()
							));
					r->setFillColor(sf::Color(
							i["rect"]["c"][0].GetInt(),
							i["rect"]["c"][1].GetInt(),
							i["rect"]["c"][2].GetInt()
							));
					if(i["rect"].HasMember("p")){
						r->setOrigin(i["rect"]["p"][0].GetFloat(),
								i["rect"]["p"][1].GetFloat());
					}
					s.addImgComponent(r);
				}else if(i.HasMember("line")){
					std::unique_ptr<sf::Shape> l = std::unique_ptr<sf::Shape>(
							new sf::LineShape(sf::Vector2f(0,0), sf::Vector2f(0,0))
					);
					((sf::LineShape*)l.get())->setThickness(
							i["line"]["w"].GetFloat());
					l->setFillColor(sf::Color(
							i["line"]["c"][0].GetInt(),
							i["line"]["c"][1].GetInt(),
							i["line"]["c"][2].GetInt()
							));

					s.addImgComponent(l);
				}else if(i.HasMember("tri")){
					std::unique_ptr<sf::Shape> l = std::unique_ptr<sf::Shape>(
							new sf::CircleShape(i["tri"]["w"].GetFloat(), 3)
					);
					l->setFillColor(sf::Color(
							i["tri"]["c"][0].GetInt(),
							i["tri"]["c"][1].GetInt(),
							i["tri"]["c"][2].GetInt()
							));
					if(i["tri"].HasMember("r")){
						l->setRotation(i["tri"]["r"].GetFloat());
					}

					// offset origin; triangles present weird positioning by default
					l->setOrigin(l->getGlobalBounds().width * 1.2,
							l->getGlobalBounds().height * 1.2);

					s.addImgComponent(l);
				}else if(i.HasMember("circle")){
					std::unique_ptr<sf::Shape> c = std::unique_ptr<sf::Shape>(
							new sf::CircleShape(i["circle"]["w"].GetFloat())
					);

					c->setFillColor(sf::Color(
							i["circle"]["c"][0].GetInt(),
							i["circle"]["c"][1].GetInt(),
							i["circle"]["c"][2].GetInt()
							));

					if(i["circle"].HasMember("olt")){
						c->setOutlineThickness(i["circle"]["olt"].GetFloat());
					}
					if(i["circle"].HasMember("olc")){
						c->setOutlineColor(sf::Color(
								i["circle"]["olc"][0].GetInt(),
								i["circle"]["olc"][1].GetInt(),
								i["circle"]["olc"][2].GetInt()
								));
					}

					s.addImgComponent(c);
				}
			}
		}
		if(strcmp(itr->name.GetString(), "body") == 0){
			e.addComponent<BodyComponent>();
			BodyComponent& b = e.getComponent<BodyComponent>();
			for (auto& i : itr->value.GetArray()){
				if(i.HasMember("collisionMod")){
					b.setCollisionMod(i["collisionMod"].GetString());
				}else if(i.HasMember("rect")){
					std::unique_ptr<sf::Shape> r = std::unique_ptr<sf::Shape>(
							new sf::RectangleShape());
					((sf::RectangleShape*)r.get())->setSize(sf::Vector2f(
							i["rect"]["w"].GetFloat(),
							i["rect"]["h"].GetFloat()
							));
					if(i["rect"].HasMember("c")){
						r->setFillColor(sf::Color(
								i["rect"]["c"][0].GetInt(),
								i["rect"]["c"][1].GetInt(),
								i["rect"]["c"][2].GetInt()
								));
					}
					if(i["rect"].HasMember("p")){
						r->setOrigin(i["rect"]["p"][0].GetFloat(),
								i["rect"]["p"][1].GetFloat());
					}
					b.addBodyComponent(i["rect"]["k"].GetString(), r);
				}else if(i.HasMember("circle")){
					std::unique_ptr<sf::Shape> c = std::unique_ptr<sf::Shape>(
							new sf::CircleShape(i["circle"]["w"].GetFloat())
					);

					b.addBodyComponent(i["circle"]["k"].GetString(), c);
				}
			}
		}
		if(strcmp(itr->name.GetString(), "checkp") == 0){
			e.addComponent<CheckpointComponent>();
		}
		if(strcmp(itr->name.GetString(), "rope") == 0){
			e.addComponent<RopeComponent>();
		}
		if(strcmp(itr->name.GetString(), "ropeD") == 0){
			e.addComponent<RopeDetailsComponent>(itr->value["extS"].GetFloat(),
					itr->value["maxL"].GetFloat(), itr->value["reelS"].GetFloat());
		}
		if(strcmp(itr->name.GetString(), "animation") == 0){
			e.addComponent<AnimationComponent>(itr->value);
		}
		if(strcmp(itr->name.GetString(), "particle") == 0){
			e.addComponent<ParticleComponent>(itr->value);
		}
		if(strcmp(itr->name.GetString(), "enemy") == 0){
			e.addComponent<EnemyComponent>();
		}
		if(strcmp(itr->name.GetString(), "move") == 0){
			e.addComponent<MovementComponent>(itr->value.GetFloat());
		}
		if(strcmp(itr->name.GetString(), "player") == 0){
			e.addComponent<PlayerComponent>();
			player = e;
		}
		if(strcmp(itr->name.GetString(), "jump") == 0){
			e.addComponent<JumpComponent>(itr->value["jAcc"].GetFloat(),
					itr->value["jTime"].GetFloat(), itr->value["jSmlWall"].GetFloat(),
					itr->value["jRest"].GetFloat());
		}
	}

	e.activate();

	return e;
}
void EntityFactory::saveEntity(anax::Entity& e, rapidjson::Value& entData,
		rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& dAlloc){

	rapidjson::Value t(rapidjson::kObjectType);

	for(anax::Component* c : e.getComponents()){
		//entity component vectors are initialized with 64 null pointers > hence null check below
		if(!c) continue;

		if(SerializableComponent* s = dynamic_cast<SerializableComponent*>(c)){
			s->save(t, dAlloc);
		}
	}

	if(entData.IsArray()){
		entData.PushBack(t, dAlloc);
	}else{
		entData.AddMember("entity", t, dAlloc);
	}
}
void EntityFactory::loadEntity(anax::Entity& e, rapidjson::Value& jsonData){
	for (rapidjson::Value::MemberIterator itr = jsonData.MemberBegin();
		itr != jsonData.MemberEnd(); ++itr)	{

	}
}
anax::Entity& EntityFactory::getPlayer(){
	return player;
}
