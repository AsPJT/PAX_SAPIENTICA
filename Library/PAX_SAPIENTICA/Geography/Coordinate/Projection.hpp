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
    // |	-180  	|	180  	|-85.051129 | 85.051129 |	WebMercatorDeg EPSG:3857(Webメルカトル) （度）
    // |	----	|	----	|	----	|	----	|	------------------
    // |	-180  	|	180  	|	-180  	|	  180  	|	この範囲を「EPSG:3857(Webメルカトル) 座標」と定義 （正方形、 EPSG:3857(Webメルカトル) の経度（度）基準の大きさ）

    //メルカトル図法において、緯度 $\phi$（ファイ）から地図上の縦方向の座標 $y$ を求める計算式、および「なぜ 85.051129度 なのか」という導出過程は以下の通りです。
    //この数値は、**Webメルカトルにおいて、地図全体を正方形にするために定義された上限**です。
    //### 1. メルカトル図法の計算式
    //メルカトル図法における緯度 $\phi$（ラジアン）の $y$ 座標は、以下の式で表されます。
    //$$
    //y = \ln\left(\tan\left(\frac{\pi}{4} + \frac{\phi}{2}\right)\right)
    //$$
    //ここで、
    //* $\ln$ は自然対数（ネイピア数 $e$ を底とする対数）
    //* $\tan$ は正接関数
    //* $\phi$ は緯度（ラジアン表記）
    //* $\pi$ は円周率
    //この式は、地球を半径1の球体とした場合の座標です。
    //### 2. なぜ「180」になるのか（導出）
    //「経度の半分の大きさ（180）と同じになる」という条件は、**「赤道から極限までの描画距離（$y$）を、赤道から経度180度までの距離（$x=\pi$）と等しくする」** という意味です。
    //つまり、上記公式の $y$ が $\pi$ （度数法での180度に相当するラジアン値）になる時の $\phi$ を逆算することで求められます。
    //#### 計算ステップ
    //**① 式を立てる**
    //$y$ に $\pi$ を代入します。
    //$$
    //\pi = \ln\left(\tan\left(\frac{\pi}{4} + \frac{\phi}{2}\right)\right)
    //$$
    //**② 対数を外す**
    //両辺の指数をとります（$\ln$ の逆関数は $e^x$）。
    //$$
    //e^\pi = \tan\left(\frac{\pi}{4} + \frac{\phi}{2}\right)
    //$$
    //**③ $\tan$ を外す**
    //逆正接（アークタンジェント）をとります。
    //$$
    //\arctan(e^\pi) = \frac{\pi}{4} + \frac{\phi}{2}
    //$$
    //**④ $\phi$ について解く**
    //$$
    //\frac{\phi}{2} = \arctan(e^\pi) - \frac{\pi}{4}
    //$$
    //$$
    //\phi = 2 \arctan(e^\pi) - \frac{\pi}{2}
    //$$
    //**⑤ 値を計算する**
    //* $e^\pi \approx 23.14069$
    //* $\arctan(23.14069)$ を計算すると、約 $1.5276$ ラジアン（約 $87.52557^\circ$）
    //* ここから $\frac{\pi}{2}$ （90度）を引いて2倍する計算になりますが、度数法で計算したほうが直感的です。
    //$$
    //\phi_{\text{deg}} = 2 \times (87.52557^\circ) - 90^\circ
    //$$
    //$$
    //\phi_{\text{deg}} = 175.05114^\circ - 90^\circ
    //$$
    //$$
    //\phi_{\text{deg}} \approx 85.05114^\circ
    //$$
    //より精密な値が、**85.05112878...度** となります。
    //### まとめ
    //計算式は以下の等式です。
    //$$
    //\phi = 2 \arctan(e^\pi) - \frac{\pi}{2}
    //$$
    //この緯度で地図を切り取ることにより、メルカトル図法で描画される世界地図は**横幅（経度360度分）と縦幅（北緯約85度〜南緯約85度）が1:1の正方形**になります。これにより、Web上のタイル画像処理（256x256ピクセルの正方形タイルの組み合わせ）が非常に効率的になるため、多くのWeb地図でこの緯度が「世界の果て」として採用されています。

    // 前方宣言
    // forward declaration
    struct WebMercatorDeg; // EPSG:3857(Webメルカトル) （度）
    struct EPSG4326_WGS84Deg; // EPSG:4326(WGS84)

    // EPSG:3857(Webメルカトル) （度）
    struct WebMercatorDeg : paxs::Vector2<double> {
        constexpr explicit WebMercatorDeg() : Vector2() {}
        constexpr explicit WebMercatorDeg(const paxs::Vector2<double>& v) noexcept : paxs::Vector2<double>(v) {}
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
        // Ｙ軸をEPSG:3857(Webメルカトル) （ラジアン）へ変換した値を返す
        double toMercatorRadY() const noexcept {
            return static_cast<double>((y >= 0 ? 1 : -1) * std::abs(std::log(std::abs(std::tan(paxs::Math<double>::pi() / 4.0 - paxs::Math<double>::degToRad(y) / 2.0)))));
        }
        // Ｙ軸をEPSG:3857(Webメルカトル) （度）へ変換した値を返す
        double toWebMercatorDegY() const noexcept {
            return paxs::Math<double>::radToDeg(toMercatorRadY());
        }
        // EPSG:3857(Webメルカトル) （度）へ変換した値を返す
        operator WebMercatorDeg() const noexcept {
            return WebMercatorDeg(paxs::Vector2<double>(x, toWebMercatorDegY()));
        }
        // EPSG:3857(Webメルカトル) （度）へ変換した値を返す
        WebMercatorDeg toWebMercatorDeg() const noexcept {
            return WebMercatorDeg(paxs::Vector2<double>(x, toWebMercatorDegY()));
        }
    };
    // EPSG:3857(Webメルカトル) （度）を EPSG:4326(WGS84) （度）へ変換した値を返す
    WebMercatorDeg::operator EPSG4326_WGS84Deg() const noexcept {
        return EPSG4326_WGS84Deg(paxs::Vector2<double>(x, toEPSG4326_WGS84DegY()));
    }

    // 個別の変換
    template<typename T>
    struct MapProjection {
        // EPSG:4326(WGS84) （ラジアン）から EPSG:3857(Webメルカトル) へ変換（ラジアン）
        constexpr static T EPSG4326_WGS84RadYToMercatorRadY(const T value) noexcept {
            return static_cast<T>((value >= 0 ? 1 : -1) * std::abs(std::log(std::abs(std::tan(paxs::Math<double>::pi() / 4.0 - double(value) / 2.0)))));
        }
        // EPSG:4326(WGS84) （度）から EPSG:3857(Webメルカトル) へ変換（ラジアン）
        constexpr static T EPSG4326_WGS84DegYToMercatorRadY(const T value) noexcept {
            return static_cast<T>(EPSG4326_WGS84RadYToMercatorRadY(paxs::Math<T>::degToRad(value)));
        }
        // EPSG:4326(WGS84) （ラジアン）から EPSG:3857(Webメルカトル) へ変換（度）
        constexpr static T EPSG4326_WGS84RadYToWebMercatorDegY(const T value) noexcept {
            return static_cast<T>(paxs::Math<double>::radToDeg(EPSG4326_WGS84RadYToMercatorRadY(value)));
        }
        // EPSG:4326(WGS84) （度）から EPSG:3857(Webメルカトル) へ変換（度）
        constexpr static T EPSG4326_WGS84DegYToWebMercatorDegY(const T value) noexcept {
            return static_cast<T>(paxs::Math<double>::radToDeg(EPSG4326_WGS84RadYToMercatorRadY(paxs::Math<T>::degToRad(value))));
        }
    };
}

#endif // !PAX_SAPIENTICA_GEOGRAPHY_COORDINATE_PROJECTION_HPP
