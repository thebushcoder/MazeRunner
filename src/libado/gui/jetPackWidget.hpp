/*
 * jetPackWidget.hpp
 *
 *  Created on: 30 Jul 2018
 *      Author: linbob
 */

#ifndef LIBADO_GUI_JETPACKWIDGET_HPP_
#define LIBADO_GUI_JETPACKWIDGET_HPP_

#include <TGUI/TGUI.hpp>
#include "../map/tileMap.hpp"
#include "../entity/components/componentsCollection.hpp"

class JetPackWidget : public tgui::Panel{
public:
	typedef std::shared_ptr<JetPackWidget> Ptr; ///< Shared widget pointer
	typedef std::shared_ptr<const JetPackWidget> ConstPtr;

	JetPackWidget(tgui::Theme::Ptr theme, anax::Entity p, float screenW, float screenH);

	void init(sf::RenderWindow* w){}

	virtual void update(sf::Time elapsedTime);
protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	anax::Entity player;

	sf::RectangleShape chargeOutline;
	sf::RectangleShape chargeBar;

	int currentCount;
	std::vector<sf::RectangleShape> turboWidgets;
};

#endif /* LIBADO_GUI_JETPACKWIDGET_HPP_ */
