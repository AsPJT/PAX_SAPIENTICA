/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_TYPE_MATH_HPP
#define PAX_SAPIENTICA_TYPE_MATH_HPP

/*##########################################################################################

##########################################################################################*/

// π
#define PAX_SAPIENTICA_PI (3.1415926535897932384626433832795028841971)

namespace paxs {

	template<typename T>
	class Math {
	public:
		constexpr inline static T pi() noexcept { return static_cast<T>(PAX_SAPIENTICA_PI); } // π
		constexpr inline static T pi2() noexcept { return static_cast<T>(PAX_SAPIENTICA_PI * 2.0); } // ２π
		constexpr inline static T pi3() noexcept { return static_cast<T>(PAX_SAPIENTICA_PI * 3.0); } // ３π
		constexpr inline static T pi4() noexcept { return static_cast<T>(PAX_SAPIENTICA_PI * 4.0); } // ４π
		constexpr inline static T piHalf() noexcept { return static_cast<T>(PAX_SAPIENTICA_PI * 0.5); } // π／２

		constexpr inline static T radToDeg() noexcept { return static_cast<T>(180.0 / PAX_SAPIENTICA_PI); } // ラジアンから度
		constexpr inline static T degToRad() noexcept { return static_cast<T>(PAX_SAPIENTICA_PI / 180.0); } // 度からラジアン
		constexpr inline static T radToDeg(const T value) noexcept { return static_cast<T>(value * radToDeg()); } // ラジアンから度
		constexpr inline static T degToRad(const T value) noexcept { return static_cast<T>(value * degToRad()); } // 度からラジアン

	};

	using MathF32 = Math<float>;
	using MathF64 = Math<double>;

}

#endif // !PAX_SAPIENTICA_TYPE_MATH_HPP