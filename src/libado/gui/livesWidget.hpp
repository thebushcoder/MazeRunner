/*
 * livesWidget.hpp
 *
 *  Created on: 14 Jul 2018
 *      Author: linbob
 */

#ifndef LIBADO_GUI_LIVESWIDGET_HPP_
#define LIBADO_GUI_LIVESWIDGET_HPP_

#include <TGUI/TGUI.hpp>
#include "../map/tileMap.hpp"
#include "../entity/components/componentsCollection.hpp"

class LivesWidget : public tgui::Panel{
public:
	typedef std::shared_ptr<LivesWidget> Ptr; ///< Shared widget pointer
	typedef std::shared_ptr<const LivesWidget> ConstPtr;

	LivesWidget(anax::Entity p, float screenW, float screenH);

	void init(sf::RenderWindow* w);

	virtual void update(sf::Time elapsedTime);
protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	anax::Entity player;

	int currentCount;

	std::vector<sf::RectangleShape> shapes;
};

#endif /* LIBADO_GUI_LIVESWIDGET_HPP_ */
