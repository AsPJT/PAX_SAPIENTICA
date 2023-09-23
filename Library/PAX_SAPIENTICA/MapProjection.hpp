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

#include <cmath>

#include <PAX_SAPIENTICA/Math.hpp>
#include <PAX_SAPIENTICA/Type/Vector2.hpp>

namespace paxs {

    // |	X min 	|	X max 	|	Y min 	|	Y max 	|	MapProjection 地図投影法
    // |	----	|	----	|	----	|	----	|	------------------
    // |	-180  	|	180  	|	 -90  	|	  90  	|	EquirectangularDeg 正距円筒図法（度）
    // |	----	|	----	|	----	|	----	|	------------------
    // |	-180  	|	180  	|	 -∞  	|	  ∞  	|	MercatorDeg メルカトル図法（度）
    // |	----	|	----	|	----	|	----	|	------------------
    // |	-180  	|	180  	|	-180  	|	  180  	|	XYZTiles	XYZ タイル（度）

    // 前方宣言
    // forward declaration
    struct MercatorDeg; // メルカトル図法（度）
    //struct MercatorRad; // メルカトル図法（ラジアン）
    struct EquirectangularDeg; // 正距円筒図法（度）
    //struct EquirectangularRad; // 正距円筒図法（ラジアン）

    // メルカトル図法（度）
    struct MercatorDeg : paxs::Vector2<double> {
    public:
        explicit MercatorDeg() {}
        // C++20 より前は constexpr にならない
        explicit MercatorDeg(const paxs::Vector2<double>& v) noexcept : paxs::Vector2<double>(v) {}
        // Ｙ軸を正距円筒図法（ラジアン）へ変換した値を返す
        double toEquirectangularRadY() const noexcept {
            return static_cast<double>(std::asin(std::tanh(paxs::MathF64::degToRad(this->y))));
        }
        // Ｙ軸を正距円筒図法（度）へ変換した値を返す
        double toEquirectangularDegY() const noexcept {
            return static_cast<double>(paxs::MathF64::radToDeg(toEquirectangularRadY()));
        }
        // 正距円筒図法（度）へ変換した値を返す
        operator EquirectangularDeg() const noexcept;
    };

    // 正距円筒図法（度）
    struct EquirectangularDeg : paxs::Vector2<double> {
    public:
        constexpr explicit EquirectangularDeg(const paxs::Vector2<double>& v) noexcept : paxs::Vector2<double>(v) {}
        // Ｙ軸をメルカトル図法（ラジアン）へ変換した値を返す
        double toMercatorRadY() const noexcept {
            return static_cast<double>((y >= 0 ? 1 : -1) * std::abs(std::log(std::abs(std::tan(paxs::MathF64::pi() / 4.0 - paxs::MathF64::degToRad(y) / 2.0)))));
        }
        // Ｙ軸をメルカトル図法（度）へ変換した値を返す
        double toMercatorDegY() const noexcept {
            return static_cast<double>(paxs::MathF64::radToDeg(toMercatorRadY()));
        }
        // メルカトル図法（度）へ変換した値を返す
        operator MercatorDeg() const noexcept {
            return MercatorDeg(paxs::Vector2<double>(x, toMercatorDegY()));
        }
        // メルカトル図法（度）へ変換した値を返す
        MercatorDeg toMercatorDeg() const noexcept {
            return MercatorDeg(paxs::Vector2<double>(x, toMercatorDegY()));
        }
    };
    // メルカトル図法（度）を正距円筒図法（度）へ変換した値を返す
    MercatorDeg::operator EquirectangularDeg() const noexcept {
        return EquirectangularDeg(paxs::Vector2<double>(x, toEquirectangularDegY()));
    }

    // 個別の変換
    template<typename T>
    class MapProjection {
    public:
        // 正距円筒図法（ラジアン）からメルカトル図法へ変換（ラジアン）
        constexpr inline static T equirectangularRadYToMercatorRadY(const T value) noexcept {
            return static_cast<T>((value >= 0 ? 1 : -1) * std::abs(std::log(std::abs(std::tan(paxs::MathF64::pi() / 4.0 - double(value) / 2.0)))));
        }
        // 正距円筒図法（度）からメルカトル図法へ変換（ラジアン）
        constexpr inline static T equirectangularDegYToMercatorRadY(const T value) noexcept {
            return static_cast<T>(equirectangularRadYToMercatorRadY(paxs::Math<T>::degToRad(value)));
        }
        // 正距円筒図法（ラジアン）からメルカトル図法へ変換（度）
        constexpr inline static T equirectangularRadYToMercatorDegY(const T value) noexcept {
            return static_cast<T>(paxs::MathF64::radToDeg(equirectangularRadYToMercatorRadY(value)));
        }
        // 正距円筒図法（度）からメルカトル図法へ変換（度）
        constexpr inline static T equirectangularDegYToMercatorDegY(const T value) noexcept {
            return static_cast<T>(paxs::MathF64::radToDeg(equirectangularRadYToMercatorRadY(paxs::Math<T>::degToRad(value))));
        }

    };

    using MapProjectionF32 = MapProjection<float>;
    using MapProjectionF64 = MapProjection<double>;

}

#endif // !PAX_SAPIENTICA_TYPE_MAP_PROJECTION_HPP
