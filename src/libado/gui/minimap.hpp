/*
 * minimap.hpp
 *
 *  Created on: 10 Jun 2018
 *      Author: linbob
 */

#ifndef LIBADO_GUI_MINIMAP_HPP_
#define LIBADO_GUI_MINIMAP_HPP_

#include <TGUI/TGUI.hpp>
#include "../map/tileMap.hpp"
#include "../entity/components/componentsCollection.hpp"

class GameScreen;
class MiniMap : public tgui::Panel{
public:
	typedef std::shared_ptr<MiniMap> Ptr; ///< Shared widget pointer
	typedef std::shared_ptr<const MiniMap> ConstPtr;

	MiniMap(GameScreen* screen);

	void init(sf::RenderWindow* w);

	virtual void update(sf::Time elapsedTime);
protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	anax::World* world;
	TileMap* map;

	tgui::Canvas::Ptr canvas;
	sf::Texture texture;
};

#endif /* LIBADO_GUI_MINIMAP_HPP_ */
