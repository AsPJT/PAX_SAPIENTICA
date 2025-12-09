/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_INTERFACE_CIRCLE_HPP
#define PAX_GRAPHICA_INTERFACE_CIRCLE_HPP

#include <PAX_GRAPHICA/Color.hpp>
#include <PAX_SAPIENTICA/Core/Type/Vector2.hpp>

namespace paxg {

	/// @brief Circle class for graphics library abstraction
	/// @note Implementation is provided in each graphics library folder
	struct Circle {
	private:
		float x_{};
		float y_{};
		float r_{};

	public:
		/// @brief Constructor with separate x, y coordinates and radius
		/// @param x X-coordinate of the circle center
		/// @param y Y-coordinate of the circle center
		/// @param r Radius of the circle
		constexpr Circle(float x, float y, float r);

		/// @brief Constructor with Vector2<int> position and radius
		/// @param pos Position of the circle center
		/// @param r Radius of the circle
		constexpr Circle(const paxs::Vector2<int>& pos, const float r);

		/// @brief Constructor with Vector2<float> position and radius
		/// @param pos Position of the circle center
		/// @param r Radius of the circle
		constexpr Circle(const paxs::Vector2<float>& pos, const float r);

		/// @brief Constructor with Vector2<double> position and radius
		/// @param pos Position of the circle center
		/// @param r Radius of the circle
		constexpr Circle(const paxs::Vector2<double>& pos, const float r);

		/// @brief Draw the circle without color
		void draw() const;

		/// @brief Draw the circle with specified color
		/// @param color The color to draw the circle
		void draw(const Color& color) const;

		/// @brief Get the position of the circle center
		/// @return Position as Vector2<float>
		constexpr paxs::Vector2<float> getPosition() const;

		/// @brief Get the radius of the circle
		constexpr float getRadius() const;
	};

} // namespace paxg

#endif // !PAX_GRAPHICA_INTERFACE_CIRCLE_HPP
