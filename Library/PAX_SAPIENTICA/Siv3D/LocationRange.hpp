/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIV3D_LOCATION_RANGE_HPP
#define PAX_SAPIENTICA_SIV3D_LOCATION_RANGE_HPP

/*##########################################################################################

##########################################################################################*/

#include <string>

#include <PAX_SAPIENTICA/Siv3D/Init.hpp>
#include <PAX_SAPIENTICA/MapProjection.hpp> // 地図投影法

namespace paxs {

    // 地図上に描画する画像とその画像の範囲（メルカトル座標）
    struct LocationRange {
        s3d::Texture texture; // 地図上に描画する画像
        double msx, msy; // 始点座標
        double mex, mey; // 終点座標
        double mcx, mcy; // 中間座標
        double mx, my; // 範囲の大きさ（終点座標 - 始点座標）

        explicit LocationRange() = default;
        explicit LocationRange(const std::string& str_, const double msx_, const double msy_, const double mex_, const double mey_)
            :texture(s3d::Unicode::FromUTF8(str_))
            , msx(msx_), msy(msy_), mex(mex_), mey(mey_)
            , mcx((mex_ - msx_) / 2 + msx_), mcy((mey_ - msy_) / 2 + msy_)
            , mx(mex_ - msx_), my(mey_ - msy_) {}
    };

    class TextureLocation {
    public:
        TextureLocation() {
            // ここに実験用のテクスチャを定義する

            //location_range_list.emplace_back("./../../../../../Data/Map/TestMap/nara.png",
            //	135.7104, 
            //	MapProjectionF64::equirectangularDegYToMercatorDegY(34.59451),
            //	135.84725, 
            //	MapProjectionF64::equirectangularDegYToMercatorDegY(34.7072));

            //location_range_list.emplace_back("./../../../../../Data/Map/XYZTile/Union/ColorCodingByProvincesOfJapan/ColorCodingByProvincesOfJapanLineAlpha_8_218_90_16_16.png",
            ////location_range_list.emplace_back("./../../../../../Data/Map/XYZTile/Union/Ryoseikoku_8_218_90_16_16.png",
            //	(218.0 / 256.0 * 360.0) - 180.0, // 126.5625
            //	((1.0 - ((90.0 + 16.0) / 256.0)) * 360.0) - 180.0,
            //	((218.0 + 16.0) / 256.0 * 360.0) - 180.0, // 149.0625
            //	((1.0 - (90.0 / 256.0)) * 360.0) - 180.0);

            //location_range_list.emplace_back("./../../../../../Data/Map/XYZTile/Union/Gaya_8_218_90_16_16.png",
            //	(218.0 / 256.0 * 360.0) - 180.0, // 126.5625
            //	((1.0 - ((90.0 + 16.0) / 256.0)) * 360.0) - 180.0,
            //	((218.0 + 16.0) / 256.0 * 360.0) - 180.0, // 149.0625
            //	((1.0 - (90.0 / 256.0)) * 360.0) - 180.0);
        }
        void update(const double  map_view_center_x, const double map_view_center_y, const double map_view_width, const double map_view_height) {
            for (std::size_t i = 0; i < location_range_list.size(); ++i) {
                auto& jj = location_range_list[i];
                if (jj.mex > map_view_center_x - map_view_width / 2 &&
                    jj.msx < map_view_center_x + map_view_width / 2 &&
                    jj.mey > map_view_center_y - map_view_height / 2 &&
                    jj.msy < map_view_center_y + map_view_height / 2) {
                    // テクスチャを描画する
                    jj.texture.resized( // 描画時に指定した画像サイズへ変換
                        jj.mx / map_view_width * double(s3d::Scene::Width())
                        , jj.my / map_view_height * double(s3d::Scene::Height())// * 1.3
                    ).drawAt( // 指定位置を画像の中心として描画
                        (jj.mcx - (map_view_center_x - map_view_width / 2)) / map_view_width * double(s3d::Scene::Width()) - 0,
                        double(s3d::Scene::Height()) - ((jj.mcy - (map_view_center_y - map_view_height / 2)) / map_view_height * double(s3d::Scene::Height()))// + 270
                    );
                }
            }
        }
    private:
        std::vector<LocationRange> location_range_list; // 地物の範囲の一覧
    };

}

#endif // !PAX_SAPIENTICA_SIV3D_LOCATION_RANGE_HPP
