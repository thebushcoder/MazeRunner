/*
 * screenManager.hpp
 *
 *  Created on: 15 Sep 2016
 *      Author: linbob
 */

#ifndef LIBADO_SCREENMANAGER_HPP_
#define LIBADO_SCREENMANAGER_HPP_

#include <string>
#include <map>
#include <memory>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>

#include "screen.hpp"

class ScreenManager{
public:
	ScreenManager(std::string fontPath, sf::RenderWindow* w) :
		current(nullptr){
		window = w;
		gui = std::make_shared<tgui::Gui>(*window);

		font = std::make_shared<sf::Font>();
		font->loadFromFile("bin/font/CaviarDreams_Bold.ttf");
		gui->setFont(tgui::Font(font));
		gui->getContainer()->setName("GUI_container");

		theme = std::make_shared<tgui::Theme>("bin/imgs/Black.txt");
	}
	~ScreenManager(){}

	Screen* getCurrentScreen(){
		return current.get();
	}
	void resize(sf::Event& e){
//		for(auto const &s : screens) {
//			if(s.second != current && s.second->getStatus() != UNINIT){
//				s.second->resize(e);
//			}
//		}
	}
	void setCurrentScreen(Screen* s){
		if(s){
			if(!current){
				current = std::unique_ptr<Screen>(std::move(s));
			}else if(!next){
				next = std::unique_ptr<Screen>(std::move(s));
			}else{
				next.reset(std::move(s));
			}
		}else{
			current = NULL;
		}
		gui->removeAllWidgets();
	}
	void run(){
		while(current){
			if(current->getStatus() == QUIT){
				if(!next) return;
				current.reset(next.release());
			}else{
				current->run();
			}
		}
	}
	tgui::Theme& getTheme(){
		return *theme.get();
	}
	tgui::Gui* getGui(){
		return gui.get();
	}
	sf::RenderWindow* getWindow(){
		return window;
	}
protected:
	std::unique_ptr<Screen> next;
	std::unique_ptr<Screen> current;

	sf::RenderWindow* window;

	std::shared_ptr<sf::Font> font;
	std::shared_ptr<tgui::Theme> theme;

	std::shared_ptr<tgui::Gui> gui;
};

#endif /* LIBADO_SCREENMANAGER_HPP_ */

