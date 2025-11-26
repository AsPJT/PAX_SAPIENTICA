/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_GEOGRAPHY_COORDINATE_PROJECTION_HPP
#define PAX_SAPIENTICA_GEOGRAPHY_COORDINATE_PROJECTION_HPP

#include <cmath>

#include <PAX_SAPIENTICA/Core/Math/Math.hpp>
#include <PAX_SAPIENTICA/Core/Type/Vector2.hpp>

namespace paxs {

    // |	X min 	|	X max 	|	Y min 	|	Y max 	|	MapProjection 地図投影法
    // |	----	|	----	|	----	|	----	|	------------------
    // |	-180  	|	180  	|	 -90  	|	  90  	|	EPSG:4326(WGS84)
    // |	----	|	----	|	----	|	----	|	------------------
    // |	-180  	|	180  	|	 -∞  	|	  ∞  	|	MercatorDeg メルカトル図法（度）
    // |	----	|	----	|	----	|	----	|	------------------
    // |	-180  	|	180  	|	-180  	|	  180  	|	XYZTiles	XYZ タイル（度）

    // 前方宣言
    // forward declaration
    struct MercatorDeg; // メルカトル図法（度）
    struct EPSG4326_WGS84Deg; // EPSG:4326(WGS84)

    // メルカトル図法（度）
    struct MercatorDeg : paxs::Vector2<double> {
        constexpr explicit MercatorDeg() : Vector2() {}
        constexpr explicit MercatorDeg(const paxs::Vector2<double>& v) noexcept : paxs::Vector2<double>(v) {}
        // Ｙ軸を EPSG:4326(WGS84) （ラジアン）へ変換した値を返す
        double toEPSG4326_WGS84RadY() const noexcept {
            return std::asin(std::tanh(paxs::Math<double>::degToRad(this->y)));
        }
        // Ｙ軸を EPSG:4326(WGS84) （度）へ変換した値を返す
        double toEPSG4326_WGS84DegY() const noexcept {
            return paxs::Math<double>::radToDeg(toEPSG4326_WGS84RadY());
        }
        // EPSG:4326(WGS84) （度）へ変換した値を返す
        operator EPSG4326_WGS84Deg() const noexcept;
    };

    // EPSG:4326(WGS84) （度）
    struct EPSG4326_WGS84Deg : paxs::Vector2<double> {
        constexpr explicit EPSG4326_WGS84Deg(const paxs::Vector2<double>& v) noexcept : paxs::Vector2<double>(v) {}
        // Ｙ軸をメルカトル図法（ラジアン）へ変換した値を返す
        double toMercatorRadY() const noexcept {
            return static_cast<double>((y >= 0 ? 1 : -1) * std::abs(std::log(std::abs(std::tan(paxs::Math<double>::pi() / 4.0 - paxs::Math<double>::degToRad(y) / 2.0)))));
        }
        // Ｙ軸をメルカトル図法（度）へ変換した値を返す
        double toMercatorDegY() const noexcept {
            return paxs::Math<double>::radToDeg(toMercatorRadY());
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
    // メルカトル図法（度）を EPSG:4326(WGS84) （度）へ変換した値を返す
    MercatorDeg::operator EPSG4326_WGS84Deg() const noexcept {
        return EPSG4326_WGS84Deg(paxs::Vector2<double>(x, toEPSG4326_WGS84DegY()));
    }

    // 個別の変換
    template<typename T>
    struct MapProjection {
        // EPSG:4326(WGS84) （ラジアン）からメルカトル図法へ変換（ラジアン）
        constexpr static T EPSG4326_WGS84RadYToMercatorRadY(const T value) noexcept {
            return static_cast<T>((value >= 0 ? 1 : -1) * std::abs(std::log(std::abs(std::tan(paxs::Math<double>::pi() / 4.0 - double(value) / 2.0)))));
        }
        // EPSG:4326(WGS84) （度）からメルカトル図法へ変換（ラジアン）
        constexpr static T EPSG4326_WGS84DegYToMercatorRadY(const T value) noexcept {
            return static_cast<T>(EPSG4326_WGS84RadYToMercatorRadY(paxs::Math<T>::degToRad(value)));
        }
        // EPSG:4326(WGS84) （ラジアン）からメルカトル図法へ変換（度）
        constexpr static T EPSG4326_WGS84RadYToMercatorDegY(const T value) noexcept {
            return static_cast<T>(paxs::Math<double>::radToDeg(EPSG4326_WGS84RadYToMercatorRadY(value)));
        }
        // EPSG:4326(WGS84) （度）からメルカトル図法へ変換（度）
        constexpr static T EPSG4326_WGS84DegYToMercatorDegY(const T value) noexcept {
            return static_cast<T>(paxs::Math<double>::radToDeg(EPSG4326_WGS84RadYToMercatorRadY(paxs::Math<T>::degToRad(value))));
        }
    };
}

#endif // !PAX_SAPIENTICA_GEOGRAPHY_COORDINATE_PROJECTION_HPP
