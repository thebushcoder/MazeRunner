/*
 * checkpointWidget.hpp
 *
 *  Created on: 1 Jul 2018
 *      Author: linbob
 */

#ifndef LIBADO_GUI_CHECKPOINTWIDGET_HPP_
#define LIBADO_GUI_CHECKPOINTWIDGET_HPP_

#include <TGUI/TGUI.hpp>
#include <anax/Entity.hpp>

class GameScreen;
class CheckpointWidget : public tgui::Panel{
public:
	typedef std::shared_ptr<CheckpointWidget> Ptr; ///< Shared widget pointer
	typedef std::shared_ptr<const CheckpointWidget> ConstPtr;

	CheckpointWidget(GameScreen* s, float screenW, float screenH, int num);

	void init(sf::RenderWindow* w);

	virtual void update(sf::Time elapsedTime);
protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	anax::Entity player;
	GameScreen* screen;

	int collected = 0;
	sf::Color fill;

	std::vector<sf::CircleShape> shapes;
};



#endif /* LIBADO_GUI_CHECKPOINTWIDGET_HPP_ */
