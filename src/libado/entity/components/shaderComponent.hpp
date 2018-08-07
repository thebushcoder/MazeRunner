/*
 * shaderComponent.hpp
 *
 *  Created on: 31 Jul 2018
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_COMPONENTS_SHADERCOMPONENT_HPP_
#define LIBADO_ENTITY_COMPONENTS_SHADERCOMPONENT_HPP_

#include <SFML/Graphics.hpp>
#include <anax/Component.hpp>

struct ShaderComponent : anax::Component{

	ShaderComponent(std::string path, sf::Texture* tex){
		shader.loadFromFile(path, sf::Shader::Fragment);

		if(tex){
			shader.setUniform("tex", *tex);
		}
	}

	ShaderComponent(rapidjson::Value& jsonData, sf::Texture* tex){
		shader.loadFromFile(jsonData["path"].GetString(), sf::Shader::Fragment);

		for(auto& e : jsonData["options"].GetArray()){
			if(e["v"].IsArray()){
				shader.setUniform(e["k"].GetString(),
						sf::Glsl::Vec4(e["v"][0].GetFloat(), e["v"][1].GetFloat(),
								e["v"][2].GetFloat(), e["v"][3].GetFloat()));
			}else if(e["v"].IsFloat()){
				shader.setUniform(e["k"].GetString(), e["v"].GetFloat());
			}
		}

		if(tex){
			shader.setUniform("tex", *tex);
		}
	}

	sf::Shader& getShader(){
		return shader;
	}
private:
	sf::Shader shader;
};



#endif /* LIBADO_ENTITY_COMPONENTS_SHADERCOMPONENT_HPP_ */
