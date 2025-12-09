/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_SFML_CIRCLE_HPP
#define PAX_GRAPHICA_SFML_CIRCLE_HPP

#ifdef PAXS_USING_SFML
#include <SFML/Graphics.hpp>

#include <PAX_GRAPHICA/Interface/Circle.hpp>
#include <PAX_GRAPHICA/Window.hpp>

namespace paxg {

	// Circle Implementation
	inline constexpr Circle::Circle(float x, float y, float r)
		: x_(x), y_(y), r_(r) {}

	inline constexpr Circle::Circle(const paxs::Vector2<int>& pos, const float r)
		: x_(static_cast<float>(pos.x)), y_(static_cast<float>(pos.y)), r_(r) {}

	inline constexpr Circle::Circle(const paxs::Vector2<float>& pos, const float r)
		: x_(pos.x), y_(pos.y), r_(r) {}

	inline constexpr Circle::Circle(const paxs::Vector2<double>& pos, const float r)
		: x_(static_cast<float>(pos.x)), y_(static_cast<float>(pos.y)), r_(r) {}

	inline void Circle::draw() const {
		sf::CircleShape circleShape(r_);
		circleShape.setPosition({ x_, y_ });
		Window::window().draw(circleShape);
	}

	inline void Circle::draw(const Color& color) const {
		sf::CircleShape circleShape(r_);
		circleShape.setPosition({ x_, y_ });
		circleShape.setFillColor(color);
		Window::window().draw(circleShape);
	}

	inline constexpr paxs::Vector2<float> Circle::getPosition() const {
		return paxs::Vector2<float>(x_, y_);
	}

	inline constexpr float Circle::getRadius() const {
		return r_;
	}

} // namespace paxg

#endif // PAXS_USING_SFML

#endif // !PAX_GRAPHICA_SFML_CIRCLE_HPP
