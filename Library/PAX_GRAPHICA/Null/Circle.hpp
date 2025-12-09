/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_NULL_CIRCLE_HPP
#define PAX_GRAPHICA_NULL_CIRCLE_HPP

#include <PAX_GRAPHICA/Interface/Circle.hpp>

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
		// No operation
	}

	inline void Circle::draw(const Color&) const {
		// No operation
	}

	inline constexpr paxs::Vector2<float> Circle::getPosition() const {
		return paxs::Vector2<float>(x_, y_);
	}

	inline constexpr float Circle::getRadius() const {
		return r_;
	}

} // namespace paxg

#endif // !PAX_GRAPHICA_NULL_CIRCLE_HPP
