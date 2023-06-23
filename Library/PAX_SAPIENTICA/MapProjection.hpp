/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_TYPE_MAP_PROJECTION_HPP
#define PAX_SAPIENTICA_TYPE_MAP_PROJECTION_HPP

/*##########################################################################################

##########################################################################################*/

#include <PAX_SAPIENTICA/Math.hpp>

namespace paxs {

	struct MercatorDeg;
	struct EquirectangularDeg;

	struct MercatorDeg : paxs::Vector2<double> {
	public:
		double toEquirectangularRadY() const {
			return static_cast<double>(std::asin(std::tanh(paxs::MathF64::degToRad(this->y))));
		}
		double toEquirectangularDegY() const {
			return static_cast<double>(paxs::MathF64::radToDeg(toEquirectangularRadY()));
		}
		operator EquirectangularDeg() const;
	};

	struct EquirectangularDeg : paxs::Vector2<double> {
	public:
		double toMercatorRadY() const {
			return static_cast<double>((y >= 0 ? 1 : -1) * std::abs(std::log(std::abs(std::tan(paxs::MathF64::pi() / 4.0 - paxs::MathF64::degToRad(y) / 2.0)))));
		}
		double toMercatorDegY() const {
			return static_cast<double>(paxs::MathF64::radToDeg(toMercatorRadY()));
		}
		operator MercatorDeg() const {
			return MercatorDeg(paxs::Vector2<double>(x, toMercatorDegY()));
		}
	};
	MercatorDeg::operator EquirectangularDeg() const {
		return EquirectangularDeg(paxs::Vector2<double>(x, toEquirectangularDegY()));
	}

	// 仮実装
	template<typename T>
	class MapProjection {
	public:

		// 正距円筒図法（ラジアン）からメルカトル図法へ変換（ラジアン）
		constexpr inline static T equirectangularRadYToMercatorRadY(const T value) {
			return static_cast<T>(std::asin(std::tanh(value)));
		}
		// 正距円筒図法（度）からメルカトル図法へ変換（ラジアン）
		constexpr inline static T equirectangularDegYToMercatorRadY(const T value) {
			return static_cast<T>(std::asin(std::tanh(paxs::Math<T>::degToRad(value))));
		}
		// 正距円筒図法（ラジアン）からメルカトル図法へ変換（度）
		constexpr inline static T equirectangularRadYToMercatorDegY(const T value) {
			return static_cast<T>(paxs::Math<T>::radToDeg(std::asin(std::tanh(value))));
		}
		// 正距円筒図法（度）からメルカトル図法へ変換（度）
		constexpr inline static T equirectangularDegYToMercatorDegY(const T value) {
			return static_cast<T>(paxs::Math<T>::radToDeg(std::asin(std::tanh(paxs::Math<T>::degToRad(value)))));
		}

	};

	using MapProjectionF32 = MapProjection<float>;
	using MapProjectionF64 = MapProjection<double>;

}

#endif // !PAX_SAPIENTICA_TYPE_MAP_PROJECTION_HPP