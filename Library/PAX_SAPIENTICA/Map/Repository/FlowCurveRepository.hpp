/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_MAP_REPOSITORY_FLOW_CURVE_REPOSITORY_HPP
#define PAX_SAPIENTICA_MAP_REPOSITORY_FLOW_CURVE_REPOSITORY_HPP

#include <cstdint>
#include <functional>
#include <string>
#include <vector>

#include <PAX_SAPIENTICA/Core/Type/Range.hpp>
#include <PAX_SAPIENTICA/Core/Type/UnorderedMap.hpp>
#include <PAX_SAPIENTICA/Geography/Coordinate/Projection.hpp>
#include <PAX_SAPIENTICA/IO/Data/TsvTable.hpp>
#include <PAX_SAPIENTICA/Map/FlowCurveLocationPoint.hpp>
#include <PAX_SAPIENTICA/Map/Repository/FeatureListLoader.hpp>
#include <PAX_SAPIENTICA/Map/Repository/LocationDataLoader.hpp>
#include <PAX_SAPIENTICA/Utility/Logger.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>

namespace paxs {

    /// @brief フロー曲線データの読み込みを担当する構造体 (Infrastructure Layer)
    /// @brief Repository for loading flow curve data (Infrastructure Layer)
    struct FlowCurveRepository {
        /// @brief フロー曲線リストを読み込み、全てのFlowCurveLocationDataを返す
        /// @brief Load flow curve data file list and return all FlowCurveLocationData
        static std::vector<std::pair<FlowCurveLocationData, FlowCurveLocationGroup>> loadFlowCurveList() {
            std::vector<std::pair<FlowCurveLocationData, FlowCurveLocationGroup>> all_flow_arrows;

            FeatureListLoader::loadFeatureList("FlowCurves", [&all_flow_arrows](const FeatureListParams& params) {
                auto loaded = loadFlowCurveFromFile(params);
                for (const auto& flow_arrow_data : loaded.flow_arrow_list) {
                    all_flow_arrows.emplace_back(flow_arrow_data, loaded);
                }
            });

            return all_flow_arrows;
        }

        /// @brief 個別ファイルからフロー曲線データを読み込み
        /// @brief Load flow curve data from individual file
        static FlowCurveLocationGroup loadFlowCurveFromFile(const FeatureListParams& params) {
            std::vector<FlowCurveLocationData> flow_arrow_list{}; // フロー曲線の一覧 / List of flow curves
            const std::uint_least32_t feature_type_hash = MurMur3::calcHash(params.type.size(), params.type.c_str());

            const std::string flow_arrow_key = params.key;
            if (flow_arrow_key.empty()) {
                PAXS_WARNING("FlowCurves.tsv key is empty for file: " + params.file_path);
                return FlowCurveLocationGroup();
            }

            paxs::TsvTable table(params.file_path);
            if (!table.isSuccessfullyLoaded()) {
                PAXS_WARNING("Failed to load flow curve file: " + params.file_path);
                return FlowCurveLocationGroup();
            }

            // 必須カラムの検証
            if (!table.hasColumn("longitude")) {
                PAXS_ERROR("FlowCurveFile is missing required column: longitude");
                return FlowCurveLocationGroup();
            }
            if (!table.hasColumn("latitude")) {
                PAXS_ERROR("FlowCurveFile is missing required column: latitude");
                return FlowCurveLocationGroup();
            }

            // カラムハッシュの設定
            const std::uint_least32_t longitude_hash = MurMur3::calcHash("longitude");
            const std::uint_least32_t latitude_hash = MurMur3::calcHash("latitude");

            // 座標列を読み込み
            std::vector<paxs::WebMercatorDeg> coordinates;

            // 1 行ずつ読み込み
            table.forEachRow([&](std::size_t row_index, const std::vector<std::string>&) {
                const std::string& longitude_str = table.get(row_index, longitude_hash);
                const std::string& latitude_str = table.get(row_index, latitude_hash);

                if (longitude_str.empty() || latitude_str.empty()) {
                    PAXS_WARNING("Skipping row " + std::to_string(row_index) + " in " + params.file_path + ": missing coordinates");
                    return;
                }

                // 経度の変換
                auto lon_opt = StringUtils::toDouble(longitude_str);
                if (!lon_opt) {
                    PAXS_WARNING("Invalid longitude at row " + std::to_string(row_index) + " in " + params.file_path + ": \"" + longitude_str + "\"");
                    return;
                }
                double point_longitude = *lon_opt;

                // 緯度の変換
                auto lat_opt = StringUtils::toDouble(latitude_str);
                if (!lat_opt) {
                    PAXS_WARNING("Invalid latitude at row " + std::to_string(row_index) + " in " + params.file_path + ": \"" + latitude_str + "\"");
                    return;
                }
                double point_latitude = *lat_opt;

                // EPSG:3857(Webメルカトル) 座標に変換して格納
                coordinates.emplace_back(
                    paxs::EPSG4326_WGS84Deg(
                        paxs::Vector2<double>(point_longitude, point_latitude)
                    ).toWebMercatorDeg()
                );
            });

            // 座標を何も読み込んでいない場合は何もしないで終わる
            if (coordinates.empty()) {
                PAXS_WARNING("No valid coordinates loaded from file: " + params.file_path);
                return FlowCurveLocationGroup();
            }

            // FlowCurveLocationDataを作成
            flow_arrow_list.emplace_back(
                flow_arrow_key,
                std::move(coordinates),
                params.zoom_range,
                params.year_range,
                feature_type_hash
            );

            // FlowCurves.tsvからcolor_hashとline_widthを取得（paramsのtexture_hashを色として使用）
            // TODO: 将来的にはcolorとline_width専用のカラムを追加
            const std::uint_least32_t flow_arrow_color_hash = params.texture_hash;  // textureカラムを色として使用
            const float flow_arrow_line_width = static_cast<float>(params.zoom);    // zoomカラムを線幅として使用

            // 読み込んだファイルを格納する
            return FlowCurveLocationGroup(
                flow_arrow_list,
                params.zoom_range,
                params.year_range,
                feature_type_hash,
                flow_arrow_color_hash,
                flow_arrow_line_width
            );
        }
    };

}

#endif // !PAX_SAPIENTICA_MAP_REPOSITORY_FLOW_CURVE_REPOSITORY_HPP
