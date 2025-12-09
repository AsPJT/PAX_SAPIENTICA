/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_SIV3D_CIRCLE_HPP
#define PAX_GRAPHICA_SIV3D_CIRCLE_HPP

#ifdef PAXS_USING_SIV3D
#include <Siv3D.hpp>

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
		s3d::Circle{ x_, y_, r_ }.draw();
	}

	inline void Circle::draw(const Color& color) const {
		s3d::Circle{ x_, y_, r_ }.draw(s3d::ColorF{ color });
	}

	inline constexpr paxs::Vector2<float> Circle::getPosition() const {
		return paxs::Vector2<float>(x_, y_);
	}

	inline constexpr float Circle::getRadius() const {
		return r_;
	}

} // namespace paxg

#endif // PAXS_USING_SIV3D

#endif // !PAX_GRAPHICA_SIV3D_CIRCLE_HPP
