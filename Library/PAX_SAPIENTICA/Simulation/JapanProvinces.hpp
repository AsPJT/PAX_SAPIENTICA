/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_PROVINCES_JAPAN_HPP
#define PAX_SAPIENTICA_PROVINCES_JAPAN_HPP

/*##########################################################################################

##########################################################################################*/

#include <random>
#include <unordered_map>
#include <vector>

#include <PAX_SAPIENTICA/AppConfig.hpp>
#include <PAX_SAPIENTICA/File.hpp>
#include <PAX_SAPIENTICA/InputFile.hpp>
#include <PAX_SAPIENTICA/Logger.hpp>
#include <PAX_SAPIENTICA/Simulation/SimulationConst.hpp>

namespace paxs {

        /// @brief A struct that represents a region in Japan.
        /// @brief 日本の地方区分を表す構造体
        struct JapanRegion {
            std::uint_least8_t id = 0;
            std::string name = "";
            std::uint_least32_t population = 0; // 人口
        };

        /// @brief 
        /// @brief mtDNA の地方区分を表す構造体
        struct mtDNA_Region {
            std::vector<std::uint_least8_t> id{};
            std::vector<double> weight{};
            std::discrete_distribution<> dist{};
        };

        /// @brief 
        /// @brief 生命表
        struct lifeSpan {
            std::vector<double> weight_farming_female{}; // 農耕民の女性の重み
            std::vector<double> weight_farming_male{}; // 農耕民の男性の重み
            std::vector<double> weight_hunter_gatherer_female{}; // 狩猟採集民の女性の重み
            std::vector<double> weight_hunter_gatherer_male{}; // 狩猟採集民の男性の重み
            std::discrete_distribution<> dist_farming_female{};
            std::discrete_distribution<> dist_farming_male{};
            std::discrete_distribution<> dist_hunter_gatherer_female{};
            std::discrete_distribution<> dist_hunter_gatherer_male{};
        };

        /// @brief A struct that represents a prefecture in Japan.
        /// @brief 日本の地区を表す構造体
        struct District {
            std::uint_least8_t id = 0;
            std::string name = "";
            std::uint_least8_t region_id = 0; // 対応する地方区分ID
            std::uint_least32_t settlement_population_min_ad200 = 0;
            std::uint_least32_t settlement_population_max_ad200 = 0;
            std::uint_least32_t init_pop = 0;
            std::uint_least32_t immigrant = 0;
            double immigrant_f64 = 0;
            double increased_immigration = 0;
            std::uint_least32_t mtdna_region_hash = 0;

            std::uint_least32_t direction_min_distance = 100;
            // std::array<double, 8> direction_weight{};
            std::vector<double> direction_weight{};
            std::discrete_distribution<> direction_dist{};
        };

    /// @brief A class that represents a prefecture in Japan.
    /// @brief 日本の州を表すクラス
    class JapanProvinces {
    private:

        // 項目の ID を返す
        std::size_t getMenuIndex(const std::unordered_map<std::uint_least32_t, std::size_t>& menu, const std::uint_least32_t& str_) const {
            return  (menu.find(str_) != menu.end()) ? menu.at(str_) : SIZE_MAX;
        }

        void inputMtDNA_List(const std::string& japan_provinces_path) noexcept {

            const std::string path = japan_provinces_path + "/mtDNA_List.tsv";

            paxs::InputFile mtdna_tsv(AppConfig::getInstance()->getRootPath() + path);
            if (mtdna_tsv.fail()) {
                PAXS_WARNING("Failed to read MtDNA_List TSV file: " + path);
                return;
            }
            // 1 行目を読み込む
            if (!(mtdna_tsv.getLine())) {
                return; // 何もない場合
            }
            // BOM を削除
            mtdna_tsv.deleteBOM();
            // 1 行目を分割する
            std::unordered_map<std::uint_least32_t, std::size_t> menu = mtdna_tsv.splitHashMapMurMur3('\t');
            std::size_t i = 1;

            // 1 行ずつ読み込み（区切りはタブ）
            while (mtdna_tsv.getLine()) {
                std::vector<std::string> sub_menu_v = mtdna_tsv.split('\t');
                if (
                    sub_menu_v.size() <= getMenuIndex(menu, MurMur3::calcHash("mtdna"))
                    ) {
                    PAXS_WARNING("Failed to read Japan MtDNA_List TSV file: " + path + " at line " + std::to_string(i));
                    continue;
                }
                mtdna_list.emplace_back(sub_menu_v[menu[MurMur3::calcHash("mtdna")]]);
                ++i;
            }

        }

        void inputLifeSpan(const std::string& japan_provinces_path) noexcept {

            const std::string path = japan_provinces_path + "/LifeSpan.tsv";

            paxs::InputFile life_span_tsv(AppConfig::getInstance()->getRootPath() + path);
            if (life_span_tsv.fail()) {
                PAXS_WARNING("Failed to read LifeSpan TSV file: " + path);
                return;
            }
            // 1 行目を読み込む
            if (!(life_span_tsv.getLine())) {
                return; // 何もない場合
            }
            // BOM を削除
            life_span_tsv.deleteBOM();
            // 1 行目を分割する
            std::unordered_map<std::uint_least32_t, std::size_t> menu = life_span_tsv.splitHashMapMurMur3('\t');
            std::size_t i = 1;

            // 1 行ずつ読み込み（区切りはタブ）
            while (life_span_tsv.getLine()) {
                std::vector<std::string> sub_menu_v = life_span_tsv.split('\t');
                if (
                    sub_menu_v.size() <= getMenuIndex(menu, MurMur3::calcHash("jomon_male_ndx")) ||
                    sub_menu_v.size() <= getMenuIndex(menu, MurMur3::calcHash("jomon_female_ndx")) ||
                    sub_menu_v.size() <= getMenuIndex(menu, MurMur3::calcHash("yayoi_male_ndx")) ||
                    sub_menu_v.size() <= getMenuIndex(menu, MurMur3::calcHash("yayoi_female_ndx"))
                    ) {
                    PAXS_WARNING("Failed to read Japan LifeSpan TSV file: " + path + " at line " + std::to_string(i));

                    life_span.weight_farming_female.emplace_back(0.0);
                    life_span.weight_farming_male.emplace_back(0.0);
                    life_span.weight_hunter_gatherer_female.emplace_back(0.0);
                    life_span.weight_hunter_gatherer_male.emplace_back(0.0);
                    continue;
                }
                life_span.weight_farming_female.emplace_back(std::stod(sub_menu_v[menu[MurMur3::calcHash("yayoi_female_ndx")]]));
                life_span.weight_farming_male.emplace_back(std::stod(sub_menu_v[menu[MurMur3::calcHash("yayoi_male_ndx")]]));
                life_span.weight_hunter_gatherer_female.emplace_back(std::stod(sub_menu_v[menu[MurMur3::calcHash("jomon_female_ndx")]]));
                life_span.weight_hunter_gatherer_male.emplace_back(std::stod(sub_menu_v[menu[MurMur3::calcHash("jomon_male_ndx")]]));
                ;
                // 確率分布を生成
                life_span.dist_farming_female = std::discrete_distribution<>(life_span.weight_farming_female.begin(), life_span.weight_farming_female.end());
                life_span.dist_farming_male = std::discrete_distribution<>(life_span.weight_farming_male.begin(), life_span.weight_farming_male.end());
                life_span.dist_hunter_gatherer_female = std::discrete_distribution<>(life_span.weight_hunter_gatherer_female.begin(), life_span.weight_hunter_gatherer_female.end());
                life_span.dist_hunter_gatherer_male = std::discrete_distribution<>(life_span.weight_hunter_gatherer_male.begin(), life_span.weight_hunter_gatherer_male.end());
                ++i;
            }
        }

        void inputMtDNA_Region(const std::string& japan_provinces_path) noexcept {

            const std::string path = japan_provinces_path + "/mtDNA.tsv";

            paxs::InputFile mtdna_tsv(AppConfig::getInstance()->getRootPath() + path);
            if (mtdna_tsv.fail()) {
                PAXS_WARNING("Failed to read MtDNA TSV file: " + path);
                return;
            }
            // 1 行目を読み込む
            if (!(mtdna_tsv.getLine())) {
                return; // 何もない場合
            }
            // BOM を削除
            mtdna_tsv.deleteBOM();
            // 1 行目を分割する
            std::unordered_map<std::uint_least32_t, std::size_t> menu = mtdna_tsv.splitHashMapMurMur3('\t');
            std::size_t i = 1;

            // 1 行ずつ読み込み（区切りはタブ）
            while (mtdna_tsv.getLine()) {
                std::vector<std::string> sub_menu_v = mtdna_tsv.split('\t');
                if (
                    sub_menu_v.size() <= getMenuIndex(menu, MurMur3::calcHash("haplo_group_region")) ||
                    sub_menu_v.size() <= getMenuIndex(menu, MurMur3::calcHash("haplo_dist"))
                    ) {
                    PAXS_WARNING("Failed to read Japan MtDNA TSV file: " + path + " at line " + std::to_string(i));
                    continue;
                }

                mtDNA_Region mtdna_region;
                std::vector<std::string> dist = paxs::StringExtensions::split(sub_menu_v[menu[MurMur3::calcHash("haplo_dist")]], '/');

                if (dist.size() % 2 == 1) {
                    continue;
                }
                if (dist.size() <= 1) {
                    continue;
                }
                for (int j = 0; j < dist.size(); j += 2) {
                    for (int k = 0; k < mtdna_list.size();++k) {
                        // mtDNA の名称の index を取得し、確率分布と一緒に管理
                        if (mtdna_list[k] == dist[j]) {
                            mtdna_region.id.emplace_back(k);
                            mtdna_region.weight.emplace_back(std::stod(dist[j + 1]));
                            break;
                        }
                    }
                }
                // 確率分布を生成
                mtdna_region.dist = std::discrete_distribution<>(mtdna_region.weight.begin(), mtdna_region.weight.end());

                // mtDNA 地方区分のハッシュ
                const std::string& mtdna_region_str = sub_menu_v[menu[MurMur3::calcHash("haplo_group_region")]];
                mtdna_region_list.emplace(MurMur3::calcHash(mtdna_region_str.size(), mtdna_region_str.c_str()), mtdna_region);

                ++i;
            }
        }

        void inputDistrict(const std::string& japan_provinces_path) noexcept {

            const std::string district_tsv_path = japan_provinces_path + "/District.tsv";

            paxs::InputFile district_tsv(AppConfig::getInstance()->getRootPath() + district_tsv_path);
            if (district_tsv.fail()) {
                PAXS_WARNING("Failed to read District TSV file: " + district_tsv_path);
                return;
            }
            // 1 行目を読み込む
            if (!(district_tsv.getLine())) {
                return; // 何もない場合
            }
            // BOM を削除
            district_tsv.deleteBOM();
            // 1 行目を分割する
            std::unordered_map<std::uint_least32_t, std::size_t> menu = district_tsv.splitHashMapMurMur3('\t');
            std::size_t i = 1;

            // 1 行ずつ読み込み（区切りはタブ）
            while (district_tsv.getLine()) {
                std::vector<std::string> sub_menu_v = district_tsv.split('\t');

                if (
                    sub_menu_v.size() <= getMenuIndex(menu, MurMur3::calcHash("id")) ||
                    sub_menu_v.size() <= getMenuIndex(menu, MurMur3::calcHash("name")) ||
                    sub_menu_v.size() <= getMenuIndex(menu, MurMur3::calcHash("region")) ||
                    sub_menu_v.size() <= getMenuIndex(menu, MurMur3::calcHash("min_pop_placed_per_cell")) ||
                    sub_menu_v.size() <= getMenuIndex(menu, MurMur3::calcHash("max_pop_placed_per_cell")) ||
                    sub_menu_v.size() <= getMenuIndex(menu, MurMur3::calcHash("init_pop")) ||
                    sub_menu_v.size() <= getMenuIndex(menu, MurMur3::calcHash("immigrant")) ||
                    sub_menu_v.size() <= getMenuIndex(menu, MurMur3::calcHash("mtdna_region")) ||
                    sub_menu_v.size() <= getMenuIndex(menu, MurMur3::calcHash("direction_min_distance")) ||
                    sub_menu_v.size() <= getMenuIndex(menu, MurMur3::calcHash("directions"))
                    ) {
                    PAXS_WARNING("Failed to read Japan region TSV file: " + district_tsv_path + " at line " + std::to_string(i));
                    continue;
                }
                District district;
                district.id = static_cast<std::uint_least8_t>(std::stoul(sub_menu_v[menu[MurMur3::calcHash("id")]]));
                district.name = sub_menu_v[menu[MurMur3::calcHash("name")]];
                district.region_id = static_cast<std::uint_least8_t>(std::stoul(sub_menu_v[menu[MurMur3::calcHash("region")]]));
                district.settlement_population_min_ad200 = static_cast<std::uint_least32_t>(std::stoul(sub_menu_v[menu[MurMur3::calcHash("min_pop_placed_per_cell")]]));
                district.settlement_population_max_ad200 = static_cast<std::uint_least32_t>(std::stoul(sub_menu_v[menu[MurMur3::calcHash("max_pop_placed_per_cell")]]));
                district.init_pop = static_cast<std::uint_least32_t>(std::stoul(sub_menu_v[menu[MurMur3::calcHash("init_pop")]]));
                district.immigrant = static_cast<std::uint_least32_t>(std::stoul(sub_menu_v[menu[MurMur3::calcHash("immigrant")]]));
                district.immigrant_f64 = static_cast<double>(district.immigrant);
                district.increased_immigration = std::stod(sub_menu_v[menu[MurMur3::calcHash("increased_immigration")]]);

                district.direction_min_distance = static_cast<std::uint_least32_t>(std::stoul(sub_menu_v[menu[MurMur3::calcHash("direction_min_distance")]]));

                std::vector<std::string> direction_split = paxs::StringExtensions::split(sub_menu_v[menu[MurMur3::calcHash("directions")]], '/');
                for (std::size_t di = 0; di < direction_split.size() && di < 8; ++di) {
                    district.direction_weight.emplace_back(std::stod(direction_split[di]));
                }
                if(district.direction_weight.size() == 0) district.direction_weight.emplace_back(0.0);
                // 方向の確率分布を生成
                district.direction_dist = std::discrete_distribution<>(district.direction_weight.begin(), district.direction_weight.end());

                const std::string& mtdna_region_str = sub_menu_v[menu[MurMur3::calcHash("mtdna_region")]];
                district.mtdna_region_hash = MurMur3::calcHash(mtdna_region_str.size(), mtdna_region_str.c_str());
                district_list.emplace_back(district);
                ++i;
            }

        }

    public:
        explicit JapanProvinces(const std::string& japan_provinces_path) noexcept {

            //const std::string japan_region_tsv_path = japan_provinces_path + "/JapanRegion.tsv";

            inputMtDNA_List(japan_provinces_path);
            inputMtDNA_Region(japan_provinces_path);
            inputDistrict(japan_provinces_path);
            inputLifeSpan(japan_provinces_path);

        }

        /// @brief
        /// @brief 更新処理
        void update() noexcept {
            for (auto& district : district_list) {
                // 渡来人増加が０の場合は処理しない
                if (district.increased_immigration == 0.0) continue;
                // 渡来人の増加
                district.immigrant_f64 += district.increased_immigration;
                district.immigrant = static_cast<std::uint_least32_t>(district.immigrant_f64);
            }
        }

        /// @brief 日本の地方区分のIDから人口を取得する
        /// @param id 日本の地方区分のID
        /// @return 人口
        /// @note IDが不正な場合は0を返す
        std::uint_least32_t getJapanRegionPopulation(const std::uint_least8_t id) const noexcept {
            for (const auto& japan_region : japan_regions) {
                if (japan_region.id == id) {
                    return japan_region.population;
                }
            }
            PAXS_WARNING("Failed to get Japan region population: " + std::to_string(id));

            return 0;
        }

        /// @brief Get a district from the ID of the district in Japan.
        /// @brief 日本の地区のIDから地区を取得する
        District& getDistrict(const std::uint_least8_t id) noexcept {
            for (auto& district : district_list) {
                if (district.id == id) {
                    return district;
                }
            }
            PAXS_WARNING("Failed to get District: " + std::to_string(id));

            return district_list[0];
        }
        const District& cgetDistrict(const std::uint_least8_t id) const noexcept {
            for (const auto& district : district_list) {
                if (district.id == id) {
                    return district;
                }
            }
            PAXS_WARNING("Failed to get District: " + std::to_string(id));

            return district_list[0];
        }
        std::uint_least8_t getMtDNA(const std::uint_least8_t id, std::mt19937& gen) noexcept {
            for (const auto& district : district_list) {
                if (district.id == id) {
                    auto& weight_list = mtdna_region_list.at(district.mtdna_region_hash);
                    return weight_list.id[weight_list.dist(gen)];
                }
            }
            PAXS_WARNING("Failed to get District: " + std::to_string(id));

            auto& weight_list = mtdna_region_list.at(district_list[0].mtdna_region_hash);
            return weight_list.id[weight_list.dist(gen)];
        }
        const std::string& getMtDNA_Name(const std::uint_least8_t id) const noexcept {
            return mtdna_list[id];
        }
        std::size_t getSizeMtDNA() const noexcept {
            return mtdna_list.size();
        }

        AgeType getLifeSpan(const bool is_farming, const bool is_female, std::mt19937& gen) noexcept {
            return static_cast<AgeType>((is_farming)?
                ((is_female) ? life_span.dist_farming_female(gen):
                    life_span.dist_farming_male(gen)) :
                ((is_female) ? life_span.dist_hunter_gatherer_female(gen) :
                    life_span.dist_hunter_gatherer_male(gen)));
        }


        /// @brief 日本の地区のIDから人口を取得する
        /// @param id 日本の地区のID
        /// @return 人口
        /// @note IDが不正な場合は0を返す
        std::uint_least32_t getDistrictPopulationAd200(const std::uint_least8_t id) const noexcept {
            for (const auto& district : district_list) {
                if (district.id == id) {
                    return district.init_pop;
                }
            }
            PAXS_WARNING("Failed to get District population: " + std::to_string(id));

            return 0;
        }

        /// @brief 日本の地区のIDから地方区分のIDを取得する
        /// @param id 日本の地区のID
        /// @return 地方区分のID
        /// @note IDが不正な場合は0を返す
        std::uint_least8_t getJapanRegionId(const std::uint_least8_t id) const noexcept {
            for (const auto& district : district_list) {
                if (district.id == id) {
                    return district.region_id;
                }
            }
            PAXS_WARNING("Failed to get Japan region ID: " + std::to_string(id));

            return 0;
        }

        /// @brief Get a list of District
        /// @brief 地区のリストを取得する
        std::vector<District>& getDistrictList() noexcept {
            return district_list;
        }
        const std::vector<District>& cgetDistrictList() const noexcept {
            return district_list;
        }
    private:
        std::vector<JapanRegion> japan_regions; // 日本の地方区分
        std::vector<District> district_list; // 日本の地区
        std::unordered_map<std::uint_least32_t, mtDNA_Region> mtdna_region_list; // mtDNA 地方区分
        //std::vector<std::uint_least32_t> mtdna_region_hash_list; // mtDNA ハッシュ計算用
        std::vector<std::string> mtdna_list; // mtDNA
        lifeSpan life_span;

    };

}

#endif // !PAX_SAPIENTICA_PROVINCES_JAPAN_HPP
