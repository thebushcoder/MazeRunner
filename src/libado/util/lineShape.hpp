#ifndef SFML_LINESHAPE_HPP
#define SFML_LINESHAPE_HPP

#include <SFML/Graphics/Export.hpp>
#include <SFML/Graphics/Shape.hpp>

namespace sf
{
	class SFML_GRAPHICS_API LineShape : public Shape
	{
		public :

		LineShape(){}
		LineShape(const Vector2f& point1, const Vector2f& point2);

		void init(Vector2f& point1, Vector2f& point2);

		void setThickness(float thickness);

		float getThickness() const;

		float getLength() const;

		Vector2f getPointA();
		Vector2f getPointB();
		void setPointA(float x, float y);
		void setPointB(float x, float y);

		virtual std::size_t getPointCount() const;

		virtual Vector2f getPoint(std::size_t index) const;

		static float calcLineMag(sf::Vector2f p1, sf::Vector2f p2);
		private :

		Vector2f pointA, pointB;
		Vector2f m_direction; ///< Direction of the line
		float m_thickness;    ///< Thickness of the line
};

} // namespace sf


#endif // SFML_LINESHAPE_HPP
