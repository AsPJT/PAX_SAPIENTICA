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

#include <PAX_SAPIENTICA/IO/Data/TsvTable.hpp>
#include <PAX_SAPIENTICA/IO/File/FileSystem.hpp>
#include <PAX_SAPIENTICA/Utility/Logger.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>
#include <PAX_SAPIENTICA/Utility/StringUtils.hpp>

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
            std::string name;
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
            std::uint_least8_t agricultural_capable = 1; // 農耕が可能か (1: 可能, 0: 不可能)

            std::uint_least32_t direction_min_distance = 100;
            // std::array<double, 8> direction_weight{};
            std::vector<double> direction_weight;
            std::discrete_distribution<> direction_dist;
        };

    /// @brief A class that represents a prefecture in Japan.
    /// @brief 日本の州を表すクラス
    class JapanProvinces {
    private:
        void inputLanguage_List(const std::string& japan_provinces_path) noexcept {
            const std::string path = japan_provinces_path + "/Language_List.tsv";
            paxs::TsvTable table(path);

            if (!table.isSuccessfullyLoaded()) {
                PAXS_WARNING("Failed to read Language_List TSV file: " + path);
                return;
            }

            static const std::uint_least32_t language_hash = MurMur3::calcHash("language");

            if (!table.hasColumn(language_hash)) {
                PAXS_ERROR("Language_List.tsv: missing required column 'language'");
                return;
            }

            for (std::size_t row = 0; row < table.rowCount(); ++row) {
                const std::string& language_value = table.get(row, language_hash);
                if (!language_value.empty()) {
                    language_list.emplace_back(language_value);
                }
            }
        }

        void inputMtDNA_List(const std::string& japan_provinces_path) noexcept {
            const std::string path = japan_provinces_path + "/mtDNA_List.tsv";
            paxs::TsvTable table(path);

            if (!table.isSuccessfullyLoaded()) {
                PAXS_WARNING("Failed to read MtDNA_List TSV file: " + path);
                return;
            }

            static const std::uint_least32_t mtdna_hash = MurMur3::calcHash("mtdna");

            if (!table.hasColumn(mtdna_hash)) {
                PAXS_ERROR("mtDNA_List.tsv: missing required column 'mtdna'");
                return;
            }

            for (std::size_t row = 0; row < table.rowCount(); ++row) {
                const std::string& mtdna_value = table.get(row, mtdna_hash);
                if (!mtdna_value.empty()) {
                    mtdna_list.emplace_back(mtdna_value);
                }
            }
        }

        void inputLanguage_Region(const std::string& japan_provinces_path) noexcept {
            const std::string path = japan_provinces_path + "/Language.tsv";
            paxs::TsvTable table(path);

            if (!table.isSuccessfullyLoaded()) {
                PAXS_WARNING("Failed to read Language TSV file: " + path);
                return;
            }

            static const std::uint_least32_t language_region_hash = MurMur3::calcHash("language_region");
            static const std::uint_least32_t language_dist_hash = MurMur3::calcHash("language_dist");

            if (!table.hasColumn(language_region_hash) || !table.hasColumn(language_dist_hash)) {
                PAXS_ERROR("Language.tsv: missing required columns");
                return;
            }

            for (std::size_t row = 0; row < table.rowCount(); ++row) {
                mtDNA_Region language_region;
                std::vector<std::string> dist = paxs::StringUtils::split(table.get(row, language_dist_hash), '/');

                if (dist.size() % 2 == 1 || dist.size() <= 1) {
                    continue;
                }

                for (std::size_t j = 0; j < dist.size(); j += 2) {
                    for (std::size_t k = 0; k < language_list.size(); ++k) {
                        if (language_list[k] == dist[j]) {
                            language_region.id.emplace_back(static_cast<std::uint_least8_t>(k));
                            language_region.weight.emplace_back(StringUtils::safeStod(dist[j + 1], 0.0, true));
                            break;
                        }
                    }
                }

                if (language_region.weight.empty()) {
                    continue;
                }

                language_region.dist = std::discrete_distribution<>(language_region.weight.begin(), language_region.weight.end());

                const std::string& region_str = table.get(row, language_region_hash);
                language_region_list.emplace(MurMur3::calcHash(region_str.size(), region_str.c_str()), language_region);
            }
        }

        void inputMtDNA_Region(const std::string& japan_provinces_path) noexcept {
            const std::string path = japan_provinces_path + "/mtDNA.tsv";
            paxs::TsvTable table(path);

            if (!table.isSuccessfullyLoaded()) {
                PAXS_WARNING("Failed to read MtDNA TSV file: " + path);
                return;
            }

            static const std::uint_least32_t haplo_group_region_hash = MurMur3::calcHash("haplo_group_region");
            static const std::uint_least32_t haplo_dist_hash = MurMur3::calcHash("haplo_dist");

            if (!table.hasColumn(haplo_group_region_hash) || !table.hasColumn(haplo_dist_hash)) {
                PAXS_ERROR("mtDNA.tsv: missing required columns");
                return;
            }

            for (std::size_t row = 0; row < table.rowCount(); ++row) {
                mtDNA_Region mtdna_region;
                std::vector<std::string> dist = paxs::StringUtils::split(table.get(row, haplo_dist_hash), '/');

                if (dist.size() % 2 == 1 || dist.size() <= 1) {
                    continue;
                }

                for (std::size_t j = 0; j < dist.size(); j += 2) {
                    for (std::size_t k = 0; k < mtdna_list.size(); ++k) {
                        if (mtdna_list[k] == dist[j]) {
                            mtdna_region.id.emplace_back(static_cast<std::uint_least8_t>(k));
                            mtdna_region.weight.emplace_back(StringUtils::safeStod(dist[j + 1], 0.0, true));
                            break;
                        }
                    }
                }

                if (mtdna_region.weight.empty()) {
                    continue;
                }

                mtdna_region.dist = std::discrete_distribution<>(mtdna_region.weight.begin(), mtdna_region.weight.end());

                const std::string& region_str = table.get(row, haplo_group_region_hash);
                mtdna_region_list.emplace(MurMur3::calcHash(region_str.size(), region_str.c_str()), mtdna_region);
            }
        }

        void inputDistrict(const std::string& japan_provinces_path) noexcept {
            const std::string district_tsv_path = japan_provinces_path + "/District.tsv";
            paxs::TsvTable table(district_tsv_path);

            if (!table.isSuccessfullyLoaded()) {
                PAXS_WARNING("Failed to read District TSV file: " + district_tsv_path);
                return;
            }

            static const std::uint_least32_t id_hash = MurMur3::calcHash("id");
            static const std::uint_least32_t name_hash = MurMur3::calcHash("name");
            static const std::uint_least32_t region_hash = MurMur3::calcHash("region");
            static const std::uint_least32_t language_hash = MurMur3::calcHash("language");
            static const std::uint_least32_t hunter_gatherer_hash = MurMur3::calcHash("hunter_gatherer");
            static const std::uint_least32_t farming_hash = MurMur3::calcHash("farming");
            static const std::uint_least32_t snp_hash = MurMur3::calcHash("snp");
            static const std::uint_least32_t min_pop_hash = MurMur3::calcHash("min_pop_placed_per_cell");
            static const std::uint_least32_t max_pop_hash = MurMur3::calcHash("max_pop_placed_per_cell");
            static const std::uint_least32_t init_pop_hash = MurMur3::calcHash("init_pop");
            static const std::uint_least32_t immigrant_hash = MurMur3::calcHash("immigrant");
            static const std::uint_least32_t increased_immigration_hash = MurMur3::calcHash("increased_immigration");
            static const std::uint_least32_t mtdna_region_hash_key = MurMur3::calcHash("mtdna_region");
            static const std::uint_least32_t language_region_hash_key = MurMur3::calcHash("language_region");
            static const std::uint_least32_t direction_min_distance_hash = MurMur3::calcHash("direction_min_distance");
            static const std::uint_least32_t directions_hash = MurMur3::calcHash("directions");
            static const std::uint_least32_t agricultural_capable_hash = MurMur3::calcHash("agricultural_capable");

            if (!table.hasColumn(id_hash) || !table.hasColumn(name_hash) ||
                !table.hasColumn(region_hash) || !table.hasColumn(language_hash) ||
                !table.hasColumn(hunter_gatherer_hash) || !table.hasColumn(farming_hash) ||
                !table.hasColumn(snp_hash) || !table.hasColumn(min_pop_hash) ||
                !table.hasColumn(max_pop_hash) || !table.hasColumn(init_pop_hash) ||
                !table.hasColumn(immigrant_hash) || !table.hasColumn(increased_immigration_hash) ||
                !table.hasColumn(mtdna_region_hash_key) || !table.hasColumn(language_region_hash_key) ||
                !table.hasColumn(direction_min_distance_hash) || !table.hasColumn(directions_hash)) {
                PAXS_ERROR("District.tsv: missing required columns");
                return;
            }

            for (std::size_t row = 0; row < table.rowCount(); ++row) {
                District district;

                district.id = static_cast<std::uint_least8_t>(std::stoul(table.get(row, id_hash)));
                district.name = table.get(row, name_hash);
                district.region_id = static_cast<std::uint_least8_t>(std::stoul(table.get(row, region_hash)));
                district.language = static_cast<std::uint_least8_t>(std::stoul(table.get(row, language_hash)));
                district.hunter_gatherer = static_cast<std::uint_least8_t>(std::stoul(table.get(row, hunter_gatherer_hash)));
                district.farming = static_cast<std::uint_least8_t>(std::stoul(table.get(row, farming_hash)));
                district.snp = static_cast<std::uint_least8_t>(std::stoul(table.get(row, snp_hash)));
                district.settlement_pop_min = static_cast<std::uint_least32_t>(std::stoul(table.get(row, min_pop_hash)));
                district.settlement_pop_max = static_cast<std::uint_least32_t>(std::stoul(table.get(row, max_pop_hash)));
                district.init_pop = static_cast<std::uint_least32_t>(std::stoul(table.get(row, init_pop_hash)));
                district.immigrant = static_cast<std::uint_least32_t>(std::stoul(table.get(row, immigrant_hash)));
                district.immigrant_f64 = static_cast<double>(district.immigrant);
                district.increased_immigration = StringUtils::safeStod(table.get(row, increased_immigration_hash), 0.0, true);
                district.direction_min_distance = static_cast<std::uint_least32_t>(std::stoul(table.get(row, direction_min_distance_hash)));
                district.agricultural_capable = static_cast<std::uint_least8_t>(std::stoul(table.get(row, agricultural_capable_hash)));

                std::vector<std::string> direction_split = paxs::StringUtils::split(table.get(row, directions_hash), '/');
                for (std::size_t di = 0; di < direction_split.size() && di < 8; ++di) {
                    district.direction_weight.emplace_back(StringUtils::safeStod(direction_split[di], 0.0, true));
                }
                if (district.direction_weight.size() == 0) district.direction_weight.emplace_back(0.0);
                district.direction_dist = std::discrete_distribution<>(district.direction_weight.begin(), district.direction_weight.end());

                const std::string& mtdna_region_str = table.get(row, mtdna_region_hash_key);
                district.mtdna_region_hash = MurMur3::calcHash(mtdna_region_str.size(), mtdna_region_str.c_str());

                const std::string& language_region_str = table.get(row, language_region_hash_key);
                district.language_region_hash = MurMur3::calcHash(language_region_str.size(), language_region_str.c_str());

                district_list.emplace_back(district);
            }
        }

    public:
        explicit JapanProvinces(const std::string& japan_provinces_path) noexcept {
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
        const District& getDistrict(const std::uint_least8_t id) const noexcept {
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
                    auto* const weight_list = language_region_list.try_get(district.language_region_hash);
                    if (weight_list == nullptr) {
                        break;
                    }
                    return weight_list->id[weight_list->dist(gen)];
                }
            }
            PAXS_WARNING("Failed to get District: " + std::to_string(id));

            auto& weight_list = language_region_list.at(district_list[0].language_region_hash);
            return weight_list.id[weight_list.dist(gen)];
        }
        const std::string& getLanguageName(const std::uint_least8_t id) const noexcept {
            return language_list[id];
        }
        std::size_t getSizeLanguage() const noexcept {
            return language_list.size();
        }

        std::uint_least8_t getMtDNA(const std::uint_least8_t id, std::mt19937& gen) noexcept {
            for (const auto& district : district_list) {
                if (district.id == id) {
                    auto* const weight_list = mtdna_region_list.try_get(district.mtdna_region_hash);
                    if (weight_list == nullptr) {
                        break;
                    }
                    return weight_list->id[weight_list->dist(gen)];
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

        const std::vector<District>& getDistrictList() const noexcept {
            return district_list;
        }
    private:
        std::vector<JapanRegion> japan_regions; // 日本の地方区分
        std::vector<District> district_list; // 日本の地区
        paxs::UnorderedMap<std::uint_least32_t, mtDNA_Region> mtdna_region_list; // mtDNA 地方区分
        paxs::UnorderedMap<std::uint_least32_t, mtDNA_Region> language_region_list; // 言語 地方区分
        std::vector<std::string> mtdna_list; // mtDNA
        std::vector<std::string> language_list; // 言語
    };

}

#endif // !PAX_SAPIENTICA_SIMULATION_CONFIG_PROVINCES_JAPAN_HPP
