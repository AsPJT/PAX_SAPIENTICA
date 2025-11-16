/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIMULATION_CONFIG_PROVINCES_JAPAN_HPP
#define PAX_SAPIENTICA_SIMULATION_CONFIG_PROVINCES_JAPAN_HPP

#include <random>
#include <vector>

#include <PAX_SAPIENTICA/Core/Utility/StringUtils.hpp>
#include <PAX_SAPIENTICA/IO/File/FileSystem.hpp>
#include <PAX_SAPIENTICA/System/AppConfig.hpp>
#include <PAX_SAPIENTICA/System/InputFile.hpp>
#include <PAX_SAPIENTICA/Utility/Logger.hpp>

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

        /// @brief A struct that represents a prefecture in Japan.
        /// @brief 日本の地区を表す構造体
        struct District {
            std::uint_least8_t id = 0;
            std::string name = "";
            std::uint_least8_t region_id = 0; // 対応する地方区分ID
            std::uint_least8_t language = 0; // 言語
            std::uint_least8_t hunter_gatherer = 0; // 狩猟採集
            std::uint_least8_t farming = 0; // 農耕文化
            std::uint_least8_t snp = 0; // SNP
            std::uint_least32_t settlement_pop_min = 0;
            std::uint_least32_t settlement_pop_max = 0;
            std::uint_least32_t init_pop = 0;
            std::uint_least32_t immigrant = 0;
            double immigrant_f64 = 0;
            double increased_immigration = 0;
            std::uint_least32_t mtdna_region_hash = 0;
            std::uint_least32_t language_region_hash = 0;

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
        std::size_t getMenuIndex(const paxs::UnorderedMap<std::uint_least32_t, std::size_t>& menu, const std::uint_least32_t& str_) const {
            return  (menu.find(str_) != menu.end()) ? menu.at(str_) : SIZE_MAX;
        }

        void inputLanguage_List(const std::string& japan_provinces_path) noexcept {

            const std::string path = japan_provinces_path + "/Language_List.tsv";

            paxs::InputFile language_tsv(path);
            if (language_tsv.fail()) {
                PAXS_WARNING("Failed to read Language_List TSV file: " + path);
                return;
            }
            // 1 行目を読み込む
            if (!(language_tsv.getLine())) {
                PAXS_WARNING("Language_List TSV file is empty: " + path);
                return; // 何もない場合
            }
            // BOM を削除
            language_tsv.deleteBOM();
            // 1 行目を分割する
            paxs::UnorderedMap<std::uint_least32_t, std::size_t> menu = language_tsv.splitHashMapMurMur3('\t');
            std::size_t i = 1;

            // 1 行ずつ読み込み（区切りはタブ）
            while (language_tsv.getLine()) {
                std::vector<std::string> sub_menu_v = language_tsv.split('\t');
                if (
                    sub_menu_v.size() <= getMenuIndex(menu, MurMur3::calcHash("language"))
                    ) {
                    PAXS_WARNING("Failed to read Japan Language_List TSV file: " + path + " at line " + std::to_string(i));
                    ++i;
                    continue;
                }
                language_list.emplace_back(sub_menu_v[menu[MurMur3::calcHash("language")]]);
                ++i;
            }

        }

        void inputMtDNA_List(const std::string& japan_provinces_path) noexcept {

            const std::string path = japan_provinces_path + "/mtDNA_List.tsv";

            paxs::InputFile mtdna_tsv(path);
            if (mtdna_tsv.fail()) {
                PAXS_WARNING("Failed to read MtDNA_List TSV file: " + path);
                return;
            }
            // 1 行目を読み込む
            if (!(mtdna_tsv.getLine())) {
                PAXS_WARNING("MtDNA_List TSV file is empty: " + path);
                return; // 何もない場合
            }
            // BOM を削除
            mtdna_tsv.deleteBOM();
            // 1 行目を分割する
            paxs::UnorderedMap<std::uint_least32_t, std::size_t> menu = mtdna_tsv.splitHashMapMurMur3('\t');
            std::size_t i = 1;

            // 1 行ずつ読み込み（区切りはタブ）
            while (mtdna_tsv.getLine()) {
                std::vector<std::string> sub_menu_v = mtdna_tsv.split('\t');
                if (
                    sub_menu_v.size() <= getMenuIndex(menu, MurMur3::calcHash("mtdna"))
                    ) {
                    PAXS_WARNING("Failed to read Japan MtDNA_List TSV file: " + path + " at line " + std::to_string(i));
                    ++i;
                    continue;
                }
                mtdna_list.emplace_back(sub_menu_v[menu[MurMur3::calcHash("mtdna")]]);
                ++i;
            }

        }

        void inputLanguage_Region(const std::string& japan_provinces_path) noexcept {

            const std::string path = japan_provinces_path + "/Language.tsv";

            paxs::InputFile language_tsv(path);
            if (language_tsv.fail()) {
                PAXS_WARNING("Failed to read Language TSV file: " + path);
                return;
            }
            // 1 行目を読み込む
            if (!(language_tsv.getLine())) {
                PAXS_WARNING("Language TSV file is empty: " + path);
                return; // 何もない場合
            }
            // BOM を削除
            language_tsv.deleteBOM();
            // 1 行目を分割する
            paxs::UnorderedMap<std::uint_least32_t, std::size_t> menu = language_tsv.splitHashMapMurMur3('\t');
#ifdef PAXS_DEVELOPMENT
            std::size_t i = 1;
#endif

            // 1 行ずつ読み込み（区切りはタブ）
            while (language_tsv.getLine()) {
                std::vector<std::string> sub_menu_v = language_tsv.split('\t');
                if (
                    sub_menu_v.size() <= getMenuIndex(menu, MurMur3::calcHash("language_region")) ||
                    sub_menu_v.size() <= getMenuIndex(menu, MurMur3::calcHash("language_dist"))
                    ) {
#ifdef PAXS_DEVELOPMENT
                    PAXS_WARNING("Failed to read Japan Language TSV file: " + path + " at line " + std::to_string(i));
#endif
                    continue;
                }

                mtDNA_Region language_region;
                std::vector<std::string> dist = paxs::StringUtils::split(sub_menu_v[menu[MurMur3::calcHash("language_dist")]], '/');

                if (dist.size() % 2 == 1) {
                    continue;
                }
                if (dist.size() <= 1) {
                    continue;
                }
                for (int j = 0; j < dist.size(); j += 2) {
                    for (int k = 0; k < language_list.size(); ++k) {
                        // 言語 の名称の index を取得し、確率分布と一緒に管理
                        if (language_list[k] == dist[j]) {
                            language_region.id.emplace_back(k);
                            language_region.weight.emplace_back(StringUtils::safeStod(dist[j + 1], 0.0, true));
                            break;
                        }
                    }
                }
                // 確率分布を生成
                language_region.dist = std::discrete_distribution<>(language_region.weight.begin(), language_region.weight.end());

                // mtDNA 地方区分のハッシュ
                const std::string& mtdna_region_str = sub_menu_v[menu[MurMur3::calcHash("haplo_group_region")]];
                language_region_list.emplace(MurMur3::calcHash(mtdna_region_str.size(), mtdna_region_str.c_str()), language_region);

#ifdef PAXS_DEVELOPMENT
                ++i;
#endif
            }
        }

        void inputMtDNA_Region(const std::string& japan_provinces_path) noexcept {

            const std::string path = japan_provinces_path + "/mtDNA.tsv";

            paxs::InputFile mtdna_tsv(path);
            if (mtdna_tsv.fail()) {
                PAXS_WARNING("Failed to read MtDNA TSV file: " + path);
                return;
            }
            // 1 行目を読み込む
            if (!(mtdna_tsv.getLine())) {
                PAXS_WARNING("MtDNA TSV file is empty: " + path);
                return; // 何もない場合
            }
            // BOM を削除
            mtdna_tsv.deleteBOM();
            // 1 行目を分割する
            paxs::UnorderedMap<std::uint_least32_t, std::size_t> menu = mtdna_tsv.splitHashMapMurMur3('\t');
#ifdef PAXS_DEVELOPMENT
            std::size_t i = 1;
#endif

            // 1 行ずつ読み込み（区切りはタブ）
            while (mtdna_tsv.getLine()) {
                std::vector<std::string> sub_menu_v = mtdna_tsv.split('\t');
                if (
                    sub_menu_v.size() <= getMenuIndex(menu, MurMur3::calcHash("haplo_group_region")) ||
                    sub_menu_v.size() <= getMenuIndex(menu, MurMur3::calcHash("haplo_dist"))
                    ) {
#ifdef PAXS_DEVELOPMENT
                    PAXS_WARNING("Failed to read Japan MtDNA TSV file: " + path + " at line " + std::to_string(i));
#endif
                    continue;
                }

                mtDNA_Region mtdna_region;
                std::vector<std::string> dist = paxs::StringUtils::split(sub_menu_v[menu[MurMur3::calcHash("haplo_dist")]], '/');

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
                            mtdna_region.weight.emplace_back(StringUtils::safeStod(dist[j + 1], 0.0, true));
                            break;
                        }
                    }
                }
                // 確率分布を生成
                mtdna_region.dist = std::discrete_distribution<>(mtdna_region.weight.begin(), mtdna_region.weight.end());

                // mtDNA 地方区分のハッシュ
                const std::string& mtdna_region_str = sub_menu_v[menu[MurMur3::calcHash("haplo_group_region")]];
                mtdna_region_list.emplace(MurMur3::calcHash(mtdna_region_str.size(), mtdna_region_str.c_str()), mtdna_region);

#ifdef PAXS_DEVELOPMENT
                ++i;
#endif
            }
        }

        void inputDistrict(const std::string& japan_provinces_path) noexcept {

            const std::string district_tsv_path = japan_provinces_path + "/District.tsv";

            paxs::InputFile district_tsv(district_tsv_path);
            if (district_tsv.fail()) {
                PAXS_WARNING("Failed to read District TSV file: " + district_tsv_path);
                return;
            }
            // 1 行目を読み込む
            if (!(district_tsv.getLine())) {
                PAXS_WARNING("District TSV file is empty: " + district_tsv_path);
                return; // 何もない場合
            }
            // BOM を削除
            district_tsv.deleteBOM();
            // 1 行目を分割する
            paxs::UnorderedMap<std::uint_least32_t, std::size_t> menu = district_tsv.splitHashMapMurMur3('\t');
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
                district.language = static_cast<std::uint_least8_t>(std::stoul(sub_menu_v[menu[MurMur3::calcHash("language")]]));
                district.hunter_gatherer = static_cast<std::uint_least8_t>(std::stoul(sub_menu_v[menu[MurMur3::calcHash("hunter_gatherer")]]));
                district.farming = static_cast<std::uint_least8_t>(std::stoul(sub_menu_v[menu[MurMur3::calcHash("farming")]]));
                district.snp = static_cast<std::uint_least8_t>(std::stoul(sub_menu_v[menu[MurMur3::calcHash("snp")]]));
                district.settlement_pop_min = static_cast<std::uint_least32_t>(std::stoul(sub_menu_v[menu[MurMur3::calcHash("min_pop_placed_per_cell")]]));
                district.settlement_pop_max = static_cast<std::uint_least32_t>(std::stoul(sub_menu_v[menu[MurMur3::calcHash("max_pop_placed_per_cell")]]));
                district.init_pop = static_cast<std::uint_least32_t>(std::stoul(sub_menu_v[menu[MurMur3::calcHash("init_pop")]]));
                district.immigrant = static_cast<std::uint_least32_t>(std::stoul(sub_menu_v[menu[MurMur3::calcHash("immigrant")]]));
                district.immigrant_f64 = static_cast<double>(district.immigrant);
                district.increased_immigration = StringUtils::safeStod(sub_menu_v[menu[MurMur3::calcHash("increased_immigration")]], 0.0, true);

                district.direction_min_distance = static_cast<std::uint_least32_t>(std::stoul(sub_menu_v[menu[MurMur3::calcHash("direction_min_distance")]]));

                std::vector<std::string> direction_split = paxs::StringUtils::split(sub_menu_v[menu[MurMur3::calcHash("directions")]], '/');
                for (std::size_t di = 0; di < direction_split.size() && di < 8; ++di) {
                    district.direction_weight.emplace_back(StringUtils::safeStod(direction_split[di], 0.0, true));
                }
                if(district.direction_weight.size() == 0) district.direction_weight.emplace_back(0.0);
                // 方向の確率分布を生成
                district.direction_dist = std::discrete_distribution<>(district.direction_weight.begin(), district.direction_weight.end());

                const std::string& mtdna_region_str = sub_menu_v[menu[MurMur3::calcHash("mtdna_region")]];
                district.mtdna_region_hash = MurMur3::calcHash(mtdna_region_str.size(), mtdna_region_str.c_str());

                const std::string& language_region_str = sub_menu_v[menu[MurMur3::calcHash("language_region")]];
                district.language_region_hash = MurMur3::calcHash(language_region_str.size(), language_region_str.c_str());
                district_list.emplace_back(district);
                ++i;
            }

        }

    public:
        explicit JapanProvinces(const std::string& japan_provinces_path) noexcept {

            //const std::string japan_region_tsv_path = japan_provinces_path + "/JapanRegion.tsv";

            inputMtDNA_List(japan_provinces_path);
            inputMtDNA_Region(japan_provinces_path);
            inputLanguage_List(japan_provinces_path);
            inputLanguage_Region(japan_provinces_path);
            inputDistrict(japan_provinces_path);
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

        // 言語を取得
        std::uint_least32_t getLanguage(const std::uint_least8_t id) const noexcept {
            for (auto& district : district_list) {
                if (district.id == id) {
                    return district.language;
                }
            }
            PAXS_WARNING("Failed to get language: " + std::to_string(id));

            return 0;
        }

        // 狩猟採集を取得
        std::uint_least32_t getHunterGatherer(const std::uint_least8_t id) const noexcept {
            for (auto& district : district_list) {
                if (district.id == id) {
                    return district.hunter_gatherer;
                }
            }
            PAXS_WARNING("Failed to get hunter gatherer: " + std::to_string(id));

            return 0;
        }

        // 農耕文化を取得
        std::uint_least32_t getFarming(const std::uint_least8_t id) const noexcept {
            for (auto& district : district_list) {
                if (district.id == id) {
                    return district.farming;
                }
            }
            PAXS_WARNING("Failed to get farming: " + std::to_string(id));

            return 0;
        }

        // SNP を取得
        std::uint_least32_t getSNP(const std::uint_least8_t id) const noexcept {
            for (auto& district : district_list) {
                if (district.id == id) {
                    return district.snp;
                }
            }
            PAXS_WARNING("Failed to get SNP: " + std::to_string(id));

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
        std::uint_least8_t getLanguage(const std::uint_least8_t id, std::mt19937& gen) noexcept {
            for (const auto& district : district_list) {
                if (district.id == id) {
                    auto& weight_list = language_region_list.at(district.language_region_hash);
                    return weight_list.id[weight_list.dist(gen)];
                }
            }
            PAXS_WARNING("Failed to get District: " + std::to_string(id));

            auto& weight_list = language_region_list.at(district_list[0].language_region_hash);
            return weight_list.id[weight_list.dist(gen)];
        }
        const std::string& getLanguage_Name(const std::uint_least8_t id) const noexcept {
            return language_list[id];
        }
        std::size_t getSizeLanguage() const noexcept {
            return language_list.size();
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
        paxs::UnorderedMap<std::uint_least32_t, mtDNA_Region> mtdna_region_list; // mtDNA 地方区分
        paxs::UnorderedMap<std::uint_least32_t, mtDNA_Region> language_region_list; // 言語 地方区分
        //std::vector<std::uint_least32_t> mtdna_region_hash_list; // mtDNA ハッシュ計算用
        std::vector<std::string> mtdna_list; // mtDNA
        std::vector<std::string> language_list; // 言語
    };

}

#endif // !PAX_SAPIENTICA_SIMULATION_CONFIG_PROVINCES_JAPAN_HPP
