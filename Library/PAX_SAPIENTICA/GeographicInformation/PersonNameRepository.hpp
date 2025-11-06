/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_GEOGRAPHIC_INFORMATION_PERSON_NAME_REPOSITORY_HPP
#define PAX_SAPIENTICA_GEOGRAPHIC_INFORMATION_PERSON_NAME_REPOSITORY_HPP

#include <algorithm>
#include <cstdint>
#include <functional>
#include <string>
#include <vector>

#include <PAX_SAPIENTICA/AppConfig.hpp>
#include <PAX_SAPIENTICA/InputFile.hpp>
#include <PAX_SAPIENTICA/MapProjection.hpp>
#include <PAX_SAPIENTICA/MurMur3.hpp>
#include <PAX_SAPIENTICA/Type/UnorderedMap.hpp>

namespace paxs {
    // 人物の位置情報
    struct PersonLocationPoint {
        constexpr explicit PersonLocationPoint() = default;
        explicit PersonLocationPoint(
            const paxs::UnorderedMap<std::uint_least32_t, std::string>& place_name_,  // 地名
            const paxs::MercatorDeg& start_coordinate_,  // 経緯度
            const paxs::MercatorDeg& end_coordinate_,  // 経緯度
            const double overall_length_,  // 全長
            const double min_view_,  // 可視化する地図の最小範囲
            const double max_view_,  // 可視化する地図の最大範囲
            const double min_year_,  // 可視化する時代（古い年～）
            const double max_year_,  // 可視化する時代（～新しい年）
            const std::uint_least32_t lpe_,  // 対象となる地物の種別
            const std::uint_least32_t place_texture_ // 出典
        ) noexcept
            : place_name(place_name_), start_coordinate(start_coordinate_), end_coordinate(end_coordinate_), overall_length(overall_length_),
            min_view(min_view_), max_view(max_view_), min_year(min_year_), max_year(max_year_), lpe(lpe_), place_texture(place_texture_) {}

        paxs::UnorderedMap<std::uint_least32_t, std::string> place_name{}; // 地名
        paxs::MercatorDeg start_coordinate{}; // 経緯度
        paxs::MercatorDeg end_coordinate{}; // 経緯度
        double overall_length = 10; // 全長
        double min_view = 0.0, max_view = 9999.0; // 可視化する地図の拡大縮小範囲
        double min_year = -99999999, max_year = 99999999; // 可視化する時代（古い年～新しい年）
        std::uint_least32_t lpe = MurMur3::calcHash("place_name"); // 対象となる地物の種別
        std::uint_least32_t place_texture = 0; // 地物の画像
    };

    // 地物の一覧
    struct PersonLocationList {
        std::vector<PersonLocationPoint> person_location_list{}; // 地物の一覧

        paxs::MercatorDeg start_start_coordinate{}; // 経緯度
        paxs::MercatorDeg start_end_coordinate{}; // 経緯度
        paxs::MercatorDeg end_start_coordinate{}; // 経緯度
        paxs::MercatorDeg end_end_coordinate{}; // 経緯度
        double min_view = 0.0, max_view = 9999.0; // 可視化する地図の拡大縮小範囲
        double min_year = -99999999, max_year = 99999999; // 可視化する時代（古い年～新しい年）
        std::uint_least32_t lpe = MurMur3::calcHash("place_name"); // 対象となる地物の種別
        std::uint_least32_t place_texture = 0; // 地物の画像

        constexpr explicit PersonLocationList() = default;
        explicit PersonLocationList(
            const std::vector<PersonLocationPoint>& location_point_list_,  // 地物
            paxs::MercatorDeg start_start_coordinate_, // 経緯度
            paxs::MercatorDeg start_end_coordinate_, // 経緯度
            paxs::MercatorDeg end_start_coordinate_, // 経緯度
            paxs::MercatorDeg end_end_coordinate_, // 経緯度
            const double min_view_,  // 可視化する地図の最小範囲
            const double max_view_,  // 可視化する地図の最大範囲
            const double min_year_,  // 可視化する時代（古い年～）
            const double max_year_,  // 可視化する時代（～新しい年）
            const std::uint_least32_t lpe_,  // 対象となる地物の種別
            const std::uint_least32_t place_texture_ // 出典
        ) noexcept
            : person_location_list(location_point_list_),
            start_start_coordinate(start_start_coordinate_), start_end_coordinate(start_end_coordinate_),
            end_start_coordinate(end_start_coordinate_), end_end_coordinate(end_end_coordinate_),
            min_view(min_view_), max_view(max_view_), min_year(min_year_), max_year(max_year_), lpe(lpe_), place_texture(place_texture_) {}

    };

    /// @brief 人物名のデータ読み込みを担当するクラス (Infrastructure Layer)
    class PersonNameRepository {
    public:
        PersonNameRepository() = default;

        /// @brief 人物名リストを読み込み
        void loadPersonNameList(
            const std::function<void(const std::string&, double, double, int, int,
                                     std::uint_least32_t, std::uint_least32_t)>& inputPlaceFunc
        ) const {
            std::string str = "";
            AppConfig::getInstance()->calcDataSettings(MurMur3::calcHash("PersonNames"),
                [&](const std::string& path_) {str = path_; });
            if (str.size() == 0) return;

            paxs::InputFile pifs(str);
            if (pifs.fail()) return;
            // 1 行目を読み込む
            if (!(pifs.getLine())) {
                return; // 何もない場合
            }
            // BOM を削除
            pifs.deleteBOM();
            // 1 行目を分割する
            paxs::UnorderedMap<std::uint_least32_t, std::size_t> menu = pifs.splitHashMapMurMur3('\t');

            const std::size_t file_path = getMenuIndex(menu, MurMur3::calcHash("file_path"));
            if (file_path == SIZE_MAX) return; // パスがないのはデータにならない
            const std::size_t place_type = getMenuIndex(menu, MurMur3::calcHash("place_type"));

            const std::size_t minimum_size = getMenuIndex(menu, MurMur3::calcHash("min_size"));
            const std::size_t maximum_size = getMenuIndex(menu, MurMur3::calcHash("max_size"));
            const std::size_t first_julian_day = getMenuIndex(menu, MurMur3::calcHash("first_julian_day"));
            const std::size_t last_julian_day = getMenuIndex(menu, MurMur3::calcHash("last_julian_day"));
            const std::size_t place_texture = getMenuIndex(menu, MurMur3::calcHash("texture"));

            // 1 行ずつ読み込み（区切りはタブ）
            while (pifs.getLine()) {
                std::vector<std::string> strvec = pifs.split('\t');

                // パスが空の場合は読み込まない
                if (strvec[file_path].size() == 0) continue;

                // 対象となる地物の種別
                const std::uint_least32_t type = (place_type == SIZE_MAX) ?
                    MurMur3::calcHash("place_name") :
                    ((strvec[place_type].size() == 0) ?
                        MurMur3::calcHash("place_name") :
                        MurMur3::calcHash(strvec[place_type].size(), strvec[place_type].c_str()));

                // 可視化する地図の最小範囲
                const double min_view = (minimum_size >= strvec.size()) ?
                    0 : ((strvec[minimum_size].size() == 0) ?
                        0 : std::stod(strvec[minimum_size]));
                // 可視化する地図の最大範囲
                const double max_view = (maximum_size >= strvec.size()) ?
                    99999999.0 : ((strvec[maximum_size].size() == 0) ?
                        99999999.0 : std::stod(strvec[maximum_size]));
                // 可視化する時代（古い年～）
                const int min_year = (first_julian_day >= strvec.size()) ?
                    -99999999 : ((strvec[first_julian_day].size() == 0) ?
                        -99999999 : std::stoi(strvec[first_julian_day]));
                // 可視化する時代（～新しい年）
                const int max_year = (last_julian_day >= strvec.size()) ?
                    99999999 : ((strvec[last_julian_day].size() == 0) ?
                        99999999 : std::stoi(strvec[last_julian_day]));
                // 画像
                const std::uint_least32_t place_texture_hash = (place_texture >= strvec.size()) ?
                    0 : ((strvec[place_texture].size() == 0) ?
                        0 : MurMur3::calcHash(strvec[place_texture].size(), strvec[place_texture].c_str()));

                // 地物を追加 (callback)
                inputPlaceFunc(strvec[file_path], min_view, max_view, min_year, max_year, type, place_texture_hash);
            }
        }

        /// @brief 個別ファイルから人物データを読み込み
        PersonLocationList loadPersonFromFile(
            const std::string& str_,
            const double min_view_,
            const double max_view_,
            const int min_year_,
            const int max_year_,
            const std::uint_least32_t lpe_,
            const std::uint_least32_t place_texture_
        ) const {
            std::vector<PersonLocationPoint> person_location_list{}; // 地物の一覧

            paxs::InputFile pifs(str_, AppConfig::getInstance()->getRootPath());
            if (pifs.fail()) return PersonLocationList();
            // 1 行目を読み込む
            if (!(pifs.getLine())) {
                return PersonLocationList(); // 何もない場合
            }
            // BOM を削除
            pifs.deleteBOM();
            // 1 行目を分割する
            paxs::UnorderedMap<std::uint_least32_t, std::size_t> menu = pifs.splitHashMapMurMur3('\t');

            const std::size_t start_longitude = getMenuIndex(menu, MurMur3::calcHash("start_longitude"));
            if (start_longitude == SIZE_MAX) return PersonLocationList(); // 経度がないのはデータにならない
            const std::size_t start_latitude = getMenuIndex(menu, MurMur3::calcHash("start_latitude"));
            if (start_latitude == SIZE_MAX) return PersonLocationList(); // 緯度がないのはデータにならない

            const std::size_t end_longitude = getMenuIndex(menu, MurMur3::calcHash("end_longitude"));
            if (end_longitude == SIZE_MAX) return PersonLocationList(); // 経度がないのはデータにならない
            const std::size_t end_latitude = getMenuIndex(menu, MurMur3::calcHash("end_latitude"));
            if (end_latitude == SIZE_MAX) return PersonLocationList(); // 緯度がないのはデータにならない

            double start_start_longitude = 180.0; // 始点の経度
            double start_end_longitude = -180.0; // 終点の経度
            double start_start_latitude = 90.0; // 始点の緯度
            double start_end_latitude = -90.0; // 終点の緯度

            double end_start_longitude = 180.0; // 始点の経度
            double end_end_longitude = -180.0; // 終点の経度
            double end_start_latitude = 90.0; // 始点の緯度
            double end_end_latitude = -90.0; // 終点の緯度

            // 配列の添え字番号
            const std::size_t overall_length = getMenuIndex(menu, MurMur3::calcHash("overall_length"));
            const std::size_t minimum_size = getMenuIndex(menu, MurMur3::calcHash("min_size"));
            const std::size_t maximum_size = getMenuIndex(menu, MurMur3::calcHash("max_size"));
            const std::size_t first_julian_day = getMenuIndex(menu, MurMur3::calcHash("first_julian_day"));
            const std::size_t last_julian_day = getMenuIndex(menu, MurMur3::calcHash("last_julian_day"));
            const std::size_t place_texture = getMenuIndex(menu, MurMur3::calcHash("texture"));

            // 地名
            const std::size_t language_ja_jp = getMenuIndex(menu, MurMur3::calcHash("ja-JP"));
            const std::size_t language_en_us = getMenuIndex(menu, MurMur3::calcHash("en-US"));

            // 1 行ずつ読み込み（区切りはタブ）
            while (pifs.getLine()) {
                std::vector<std::string> strvec = pifs.split('\t');

                // 経度の文字列が空の場合は読み込まない
                if (strvec[start_longitude].size() == 0) continue;
                // 経度の文字列が空の場合は読み込まない
                if (strvec[start_latitude].size() == 0) continue;
                // 経度の文字列が空の場合は読み込まない
                if (strvec[end_longitude].size() == 0) continue;
                // 経度の文字列が空の場合は読み込まない
                if (strvec[end_latitude].size() == 0) continue;

                // 地名
                paxs::UnorderedMap<std::uint_least32_t, std::string> place_name{};
                if (language_ja_jp < strvec.size() && strvec[language_ja_jp].size() != 0) {
                    place_name.emplace(MurMur3::calcHash("ja-JP"), strvec[language_ja_jp]);
                }
                if (language_en_us < strvec.size() && strvec[language_en_us].size() != 0) {
                    place_name.emplace(MurMur3::calcHash("en-US"), strvec[language_en_us]);
                }
                bool is_overall_length = false;
                if (overall_length < strvec.size()) {
                    if (strvec[overall_length].size() != 0) is_overall_length = true;
                }

                double start_point_longitude = std::stod(strvec[start_longitude]); // 経度
                double start_point_latitude = std::stod(strvec[start_latitude]); // 緯度
                // 経緯度の範囲を求める
                start_start_longitude = (std::min)(start_start_longitude, start_point_longitude);
                start_start_latitude = (std::min)(start_start_latitude, start_point_latitude);
                start_end_longitude = (std::max)(start_end_longitude, start_point_longitude);
                start_end_latitude = (std::max)(start_end_latitude, start_point_latitude);

                double end_point_longitude = std::stod(strvec[end_longitude]); // 経度
                double end_point_latitude = std::stod(strvec[end_latitude]); // 緯度
                // 経緯度の範囲を求める
                end_start_longitude = (std::min)(end_start_longitude, end_point_longitude);
                end_start_latitude = (std::min)(end_start_latitude, end_point_latitude);
                end_end_longitude = (std::max)(end_end_longitude, end_point_longitude);
                end_end_latitude = (std::max)(end_end_latitude, end_point_latitude);

                // 格納
                person_location_list.emplace_back(
                    place_name,
                    paxs::EquirectangularDeg(
                        paxs::Vector2<double>(
                            start_point_longitude, // 経度
                            start_point_latitude)).toMercatorDeg(), // 緯度
                    paxs::EquirectangularDeg(
                    paxs::Vector2<double>(
                        end_point_longitude, // 経度
                        end_point_latitude)).toMercatorDeg(), // 緯度
                    (!is_overall_length) ? 1000.0 : std::stod(strvec[overall_length]), // 全長
                    (minimum_size >= strvec.size()) ? min_view_ : std::stod(strvec[minimum_size]), // 最小サイズ
                    (maximum_size >= strvec.size()) ? max_view_ : std::stod(strvec[maximum_size]), // 最大サイズ
                    (first_julian_day >= strvec.size()) ? min_year_ : std::stod(strvec[first_julian_day]), // 最小時代
                    (last_julian_day >= strvec.size()) ? max_year_ : std::stod(strvec[last_julian_day]), // 最大時代
                    lpe_,
                    (place_texture >= strvec.size()) ? place_texture_ : MurMur3::calcHash(strvec[place_texture].size(), strvec[place_texture].c_str()) // テクスチャの Key
                );
            }
            // 地物を何も読み込んでいない場合は何もしないで終わる
            if (person_location_list.size() == 0) return PersonLocationList();

            // 読み込んだファイルを格納する
            return PersonLocationList(person_location_list,
                paxs::EquirectangularDeg(
                    paxs::Vector2<double>(start_start_longitude/* 経度 */, start_start_latitude/* 緯度 */)).toMercatorDeg(),
                paxs::EquirectangularDeg(
                    paxs::Vector2<double>(start_end_longitude/* 経度 */, start_end_latitude/* 緯度 */)).toMercatorDeg(),
                paxs::EquirectangularDeg(
                    paxs::Vector2<double>(end_start_longitude/* 経度 */, end_start_latitude/* 緯度 */)).toMercatorDeg(),
                paxs::EquirectangularDeg(
                    paxs::Vector2<double>(end_end_longitude/* 経度 */, end_end_latitude/* 緯度 */)).toMercatorDeg(),
                min_view_, max_view_, min_year_, max_year_
                , lpe_, place_texture_);
        }

    private:
        // 項目の ID を返す
        static std::size_t getMenuIndex(const paxs::UnorderedMap<std::uint_least32_t, std::size_t>& menu, const std::uint_least32_t& str_) {
            return  (menu.find(str_) != menu.end()) ? menu.at(str_) : SIZE_MAX;
        }
    };

}

#endif // !PAX_SAPIENTICA_GEOGRAPHIC_INFORMATION_PERSON_NAME_REPOSITORY_HPP
