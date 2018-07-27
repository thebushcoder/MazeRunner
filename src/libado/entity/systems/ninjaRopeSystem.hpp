/*
 * ninjaRopeSystem.hpp
 *
 *  Created on: 15 Apr 2018
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_SYSTEMS_NINJAROPESYSTEM_HPP_
#define LIBADO_ENTITY_SYSTEMS_NINJAROPESYSTEM_HPP_

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <map>

#include <anax/System.hpp>
#include "../../map/tileMap.hpp"

#include "../components/componentsCollection.hpp"

struct NinjaRopeSystem : anax::System<anax::Requires<RopeDetailsComponent>>{
    sf::LineShape gravityLine, swingDir, oppositeDir;

	NinjaRopeSystem(TileMap* m) : map(m){
		gravityLine = sf::LineShape();
		swingDir = sf::LineShape();
		oppositeDir = sf::LineShape();
	}

	void update(sf::Time& delta){
		for(auto e : getEntities()){
			RopeDetailsComponent& r = e.getComponent<RopeDetailsComponent>();

			if(r.currentLen >= r.getMaxLen() && !r.isAnchored){
				r.srcEntity.getComponent<RopeComponent>().isFired = false;
				e.kill();
				continue;
			}

			float magnitude, lenX, lenY;
			if(!r.isAnchored){
				magnitude = sf::LineShape::calcLineMag(r.src, r.tgt);
				lenX = (r.tgt.x - r.src.x);
				lenY = (r.tgt.y - r.src.y);
				r.normDir.x = lenX / magnitude;
				r.normDir.y = lenY / magnitude;
			}else{
				magnitude = sf::LineShape::calcLineMag(r.src, r.extEnd);
				lenX = (r.extEnd.x - r.src.x);
				lenY = (r.extEnd.y - r.src.y);
				r.normDir.x = lenX / magnitude;
				r.normDir.y = lenY / magnitude;
			}

			// EXTEND ROPE
			if(r.currentLen < r.getMaxLen() && !r.isAnchored){
				// ext speed
				r.extVel += r.getExtendSpeed() * delta.asSeconds();
				if(r.extVel > r.getExtendSpeed()){
					r.extVel = r.getExtendSpeed();
				}

				// new rope len
				r.currentLen += r.extVel;

				// calc rope end position
				float dX = r.normDir.x * r.extVel;
				float dY = r.normDir.y * r.extVel;
				r.extEnd.x += dX;
				r.extEnd.y += dY;
				if(std::abs(r.extEnd.x - r.src.x) > r.getMaxLen()){
					r.extEnd.x = r.src.x + r.getMaxLen() * r.normDir.x;
				}
				if(std::abs(r.extEnd.y - r.src.y) > r.getMaxLen()){
					r.extEnd.y = r.src.y + r.getMaxLen() * r.normDir.y;
				}
			}

			// CHECK FOR ANCHOR COLLISION
			if(!r.isAnchored){
				//	GET ROPE EXT TILE POSITION
				int tileX = std::floor((r.extEnd.x) / TILESIZE);
				int tileY = std::floor((r.extEnd.y) / TILESIZE);

				// determine point of intersection, rect edge + rope line, set anchor to intersec point
				if(map->getTileLayer().isOccupied(tileX, tileY)){
					Tile* centerTile = map->getTileLayer().getTile(tileX, tileY).get();
					if(centerTile->getBody().getGlobalBounds().contains(r.extEnd)){
						sf::Vector2f intersect = checkRectIntersection(centerTile, r);
						if(intersect.x != 0 && intersect.y != 0){
							r.extEnd = intersect;
							r.isAnchored = true;
							r.anchor = r.extEnd;
						}
					}
				}
			}else if(r.isAnchored){

				updateRopeSwing(e, delta);

				// check if anchored rope is too long
				if(magnitude >= r.getMaxLen()){
					e.kill();
					continue;
				}

			}

			PositionComponent& p = r.srcEntity.getComponent<PositionComponent>();
			r.src = p.screenPosition;

			SpriteComponent& s = e.getComponent<SpriteComponent>();
			((sf::LineShape*)s.getShape())->init(r.src, r.extEnd);
		}
	}

    bool isDrawingDebug = false;
    void drawDebug(sf::RenderWindow* window){
    	if(!isDrawingDebug) return;

    	for (auto entity : getEntities()){
			RopeDetailsComponent& r = entity.getComponent<RopeDetailsComponent>();
			MovementComponent& s = r.srcEntity.getComponent<MovementComponent>();

			float len = 192;
			gravityLine = sf::LineShape(r.src, r.src + (-r.normDir) * len);
			gravityLine.setFillColor(sf::Color::Blue);
			window->draw(gravityLine);

			if(s.currentAcc.x > 0){
				swingDir = sf::LineShape(r.src, r.src + sf::Vector2f(-r.normDir.y,
						r.normDir.x) * len);
				oppositeDir = sf::LineShape(r.src, r.src + sf::Vector2f(r.normDir.y,
						-r.normDir.x) * len);
			}else{
				swingDir = sf::LineShape(r.src, r.src + sf::Vector2f(r.normDir.y,
						-r.normDir.x) * len);
				oppositeDir = sf::LineShape(r.src, r.src + sf::Vector2f(-r.normDir.y,
						r.normDir.x) * len);
			}

			oppositeDir.setFillColor(sf::Color::White);
			if(s.currentAcc.x != 0){
				swingDir.setFillColor(sf::Color::Green);
			}else{
				swingDir.setFillColor(sf::Color::White);
			}

			window->draw(oppositeDir);
			window->draw(swingDir);
		}
    }

private:
	TileMap* map;

    /*
     * 	Pendulum swing physics
     * 	https://www.khanacademy.org/computing/computer-programming/programming-natural-simulations/
     * 		programming-oscillations/a/trig-and-forces-the-pendulum
     * 	https://rosettacode.org/wiki/Animate_a_pendulum#C.2B.2B
     */
    void updateRopeSwing(anax::Entity rope, sf::Time& delta){
		if(rope.isValid()){
			RopeDetailsComponent& r = rope.getComponent<RopeDetailsComponent>();
			SpriteComponent& spr = rope.getComponent<SpriteComponent>();

			JumpComponent& j = r.srcEntity.getComponent<JumpComponent>();
			MovementComponent& s = r.srcEntity.getComponent<MovementComponent>();
			PositionComponent& p = r.srcEntity.getComponent<PositionComponent>();
			BodyComponent& b = r.srcEntity.getComponent<BodyComponent>();
			sf::RectangleShape* body = (sf::RectangleShape*)b.getShape("main");

			r.tmpLen = 0;
			// reel/change rope length
			if(r.isAnchored && r.reelVel != 0){
				r.tmpLen = r.currentLen + (r.reelVel * delta.asSeconds());

				if(r.tmpLen < 12 || r.tmpLen >= r.getMaxLen()) r.tmpLen = 0;
			}

			float len = (r.tmpLen == 0 ? r.currentLen : r.tmpLen);
			sf::Vector2f newPos;

			if(r.isAnchored && j.inAir){
				s.currentVel.y = 0;

				//	current rope length
				sf::Vector2f diff = r.anchor - r.src;

				// L/R swing OR apply gravity
				if(s.currentAcc.x != 0){
					// normalize dir
					diff /= len;

					// Set L/R swing dir
					sf::Vector2f perpDir;
					if(s.currentAcc.x > 0){
						perpDir = sf::Vector2f(-diff.y, diff.x);
					}else{
						perpDir = sf::Vector2f(diff.y, -diff.x);
					}
					// calc move amt in swing dir
					perpDir *= 4.0f;

					// add swing dir + amt to player position
					newPos = p.screenPosition + perpDir;

					float mag = sf::LineShape::calcLineMag(newPos, r.anchor);
					if(mag != r.currentLen){
						float d = r.currentLen - mag;

						sf::Vector2f norm = (r.anchor - newPos) / mag;

						newPos.x += -d * norm.x;
						newPos.y += -d * norm.y;
					}

					// move body to new pos
					sf::RectangleShape* body = (sf::RectangleShape*)b.getShape("main");
					body->setPosition(newPos.x, newPos.y);

				}else{
					float angle = std::atan2(-1 * diff.y, diff.x) - M_PI/ 2;
					float gravityF = (-1 * GRAVITY) / (len * 0.8);
					float acc = (gravityF) * std::sin(angle);
					r.swingVel += acc * delta.asSeconds();

					// swing friction
					r.swingVel *= 0.99300;

					// adjust angle according to swing velocity
					angle += r.swingVel;

					newPos = sf::Vector2f(len * std::sin(angle), len * std::cos(angle));

					newPos += r.anchor;
				}

				int tileX = std::floor((p.screenPosition.x +
						(body->getGlobalBounds().width / 2)) / TILESIZE);
				int tileY = std::floor((p.screenPosition.y +
						(body->getGlobalBounds().height / 2)) / TILESIZE);

				map->getEntityLayer().removeEntity(tileX, tileY, r.srcEntity.getId().index);

				p.screenPosition = newPos;
				r.src = newPos;

				tileX = std::floor((p.screenPosition.x +
						(body->getGlobalBounds().width / 2)) / TILESIZE);
				tileY = std::floor((p.screenPosition.y +
						(body->getGlobalBounds().height / 2)) / TILESIZE);

				map->getEntityLayer().setEntity(tileX, tileY, r.srcEntity.getId().index);

				((sf::LineShape*)spr.getShape())->init(r.src, r.anchor);
			}
		}
    }

	// check tile-rope collision/intersection point
	sf::Vector2f checkRectIntersection(Tile* t, RopeDetailsComponent& r){
		sf::Vector2f intersect;
		sf::Vector2f rectOrigin(t->getBody().getGlobalBounds().left,
				t->getBody().getGlobalBounds().top);
		sf::Vector2f rectMax(t->getBody().getGlobalBounds().left +
				t->getBody().getGlobalBounds().width,
				t->getBody().getGlobalBounds().top +
				t->getBody().getGlobalBounds().height);

		float dist1 = 9999;
		std::pair<sf::Vector2f, sf::Vector2f> side1;
		float dist2 = 9999;
		std::pair<sf::Vector2f, sf::Vector2f> side2;

		for(int i = 0; i < 4; ++i){
			switch(i){
			//	LEFT SIDE - O,O > O,M
			case 0:
			{
				sf::Vector2f s(rectOrigin.x, rectMax.y);
				sf::Vector2f mid((rectOrigin.x + rectOrigin.x) / 2,
						((rectOrigin.y + rectMax.y) / 2));
				float dist = std::abs(r.src.x - mid.x) +
						std::abs(r.src.y - mid.y);

				if(dist < dist1){
					if(dist1 >= 9999){
						dist2 = dist1;
						side2 = side1;
					}
					dist1 = dist;
					side1 = std::make_pair(rectOrigin, s);
				}else if(dist < dist2){
					dist2 = dist;
					side2 = std::make_pair(rectOrigin, s);
				}

			}
			break;
			//	BOTTOM SIDE - O,O > M,O
			case 1:
			{
				sf::Vector2f s(rectMax.x, rectOrigin.y);
				sf::Vector2f mid((rectOrigin.x + rectMax.x) / 2,
						((rectOrigin.y + rectOrigin.y) / 2));
				float dist = std::abs(r.src.x - mid.x) +
						std::abs(r.src.y - mid.y);

				if(dist < dist1){
					if(dist1 >= 9999){
						dist2 = dist1;
						side2 = side1;
					}
					dist1 = dist;
					side1 = std::make_pair(rectOrigin, s);
				}else if(dist < dist2){
					dist2 = dist;
					side2 = std::make_pair(rectOrigin, s);
				}

			}
			break;
			//	TOP SIDE - O,M > M,M
			case 2:
			{
				sf::Vector2f s(rectOrigin.x, rectMax.y);
				sf::Vector2f mid((rectOrigin.x + rectMax.x) / 2,
						((rectMax.y + rectMax.y) / 2));
				float dist = std::abs(r.src.x - mid.x) +
						std::abs(r.src.y - mid.y);

				if(dist < dist1){
					if(dist1 >= 9999){
						dist2 = dist1;
						side2 = side1;
					}
					dist1 = dist;
					side1 = std::make_pair(s, rectMax);
				}else if(dist < dist2){
					dist2 = dist;
					side2 = std::make_pair(s, rectMax);
				}
			}
			break;
			//	RIGHT SIDE - M,O > M,M
			case 3:
			{
				sf::Vector2f s(rectMax.x, rectOrigin.y);
				sf::Vector2f mid((rectMax.x + rectMax.x) / 2,
						((rectMax.y + rectOrigin.y) / 2));
				float dist = std::abs(r.src.x - mid.x) +
						std::abs(r.src.y - mid.y);

				if(dist < dist1){
					if(dist1 >= 9999){
						dist2 = dist1;
						side2 = side1;
					}
					dist1 = dist;
					side1 = std::make_pair(s, rectMax);
				}else if(dist < dist2){
					dist2 = dist;
					side2 = std::make_pair(s, rectMax);
				}
			}
			break;
			}
		}

		intersect = checkIntersection(r.src, r.extEnd,
				side1.first, side1.second);
		if(intersect.x == 0 && intersect.y == 0){
			intersect = checkIntersection(r.src, r.extEnd,
					side2.first, side2.second);
		}

		return intersect;
	}

	sf::Vector2f checkIntersection(sf::Vector2f ps1, sf::Vector2f pe1, sf::Vector2f ps2, sf::Vector2f pe2){
		// Store the values for fast access and easy
		// equations-to-code conversion
		float x1 = ps1.x, x2 = pe1.x, x3 = ps2.x, x4 = pe2.x;
		float y1 = ps1.y, y2 = pe1.y, y3 = ps2.y, y4 = pe2.y;

		float d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
		// If d is zero, there is no intersection
		if (d == 0) return sf::Vector2f(0, 0);

		// Get the x and y
		float pre = (x1*y2 - y1*x2), post = (x3*y4 - y3*x4);
		float x = ( pre * (x3 - x4) - (x1 - x2) * post ) / d;
		float y = ( pre * (y3 - y4) - (y1 - y2) * post ) / d;

		// Check if the x and y coordinates are within both lines
		if ( x < std::min(x1, x2) || x > std::max(x1, x2) ||
		x < std::min(x3, x4) || x > std::max(x3, x4) ) return sf::Vector2f(0, 0);
		if ( y < std::min(y1, y2) || y > std::max(y1, y2) ||
		y < std::min(y3, y4) || y > std::max(y3, y4) ) return sf::Vector2f(0, 0);

		// Return the point of intersection
		return sf::Vector2f(x, y);
	}
};

#endif /* LIBADO_ENTITY_SYSTEMS_NINJAROPESYSTEM_HPP_ */
