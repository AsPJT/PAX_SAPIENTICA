/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_LOCATION_RANGE_HPP
#define PAX_MAHOROBA_LOCATION_RANGE_HPP

#include <string>

#include <PAX_GRAPHICA/Texture.hpp>

#include <PAX_MAHOROBA/Core/Init.hpp>

#include <PAX_SAPIENTICA/MapProjection.hpp>

namespace paxs {

    // 地図上に描画する画像とその画像の範囲（メルカトル座標）
    struct LocationRange {
        paxg::Texture texture{}; // 地図上に描画する画像
        double msx, msy; // 始点座標
        double mex, mey; // 終点座標
        double mcx, mcy; // 中間座標
        double mx, my; // 範囲の大きさ（終点座標 - 始点座標）

        explicit LocationRange() = default;
        explicit LocationRange(const std::string& str_, const double msx_, const double msy_, const double mex_, const double mey_)
            :texture(str_)
            , msx(msx_), msy(msy_), mex(mex_), mey(mey_)
            , mcx((mex_ - msx_) / 2 + msx_), mcy((mey_ - msy_) / 2 + msy_)
            , mx(mex_ - msx_), my(mey_ - msy_) {}
    };
}

#endif // !PAX_MAHOROBA_LOCATION_RANGE_HPP
