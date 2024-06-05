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
        /// @brief 日本の令制国を表す構造体
        struct Ryoseikoku {
            std::uint_least8_t id = 0;
            std::string name = "";
            std::uint_least8_t region_id = 0; // 対応する地方区分ID
            std::uint_least32_t settlement_population_min_ad200 = 0;
            std::uint_least32_t settlement_population_max_ad200 = 0;
            std::uint_least32_t population[4] = {};
            std::uint_least32_t mtdna_region_hash = 0;
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
                for (int i = 0; i < dist.size(); i += 2) {
                    for (int j = 0; j < mtdna_list.size();++j) {
                        // mtDNA の名称の index を取得し、確率分布と一緒に管理
                        if (mtdna_list[j] == dist[i]) {
                            mtdna_region.id.emplace_back(j);
                            mtdna_region.weight.emplace_back(std::stod(dist[i + 1]));
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

            const std::string ryoseikoku_tsv_path = japan_provinces_path + "/District.tsv";

            paxs::InputFile ryoseikoku_tsv(AppConfig::getInstance()->getRootPath() + ryoseikoku_tsv_path);
            if (ryoseikoku_tsv.fail()) {
                PAXS_WARNING("Failed to read Ryoseikoku TSV file: " + ryoseikoku_tsv_path);
                return;
            }
            // 1 行目を読み込む
            if (!(ryoseikoku_tsv.getLine())) {
                return; // 何もない場合
            }
            // BOM を削除
            ryoseikoku_tsv.deleteBOM();
            // 1 行目を分割する
            std::unordered_map<std::uint_least32_t, std::size_t> menu = ryoseikoku_tsv.splitHashMapMurMur3('\t');
            std::size_t i = 1;

            // 1 行ずつ読み込み（区切りはタブ）
            while (ryoseikoku_tsv.getLine()) {
                std::vector<std::string> sub_menu_v = ryoseikoku_tsv.split('\t');

                if (
                    sub_menu_v.size() <= getMenuIndex(menu, MurMur3::calcHash("id")) ||
                    sub_menu_v.size() <= getMenuIndex(menu, MurMur3::calcHash("name")) ||
                    sub_menu_v.size() <= getMenuIndex(menu, MurMur3::calcHash("region")) ||
                    sub_menu_v.size() <= getMenuIndex(menu, MurMur3::calcHash("min_pop_placed_per_cell")) ||
                    sub_menu_v.size() <= getMenuIndex(menu, MurMur3::calcHash("max_pop_placed_per_cell")) ||
                    sub_menu_v.size() <= getMenuIndex(menu, MurMur3::calcHash("init_pop")) ||
                    sub_menu_v.size() <= getMenuIndex(menu, MurMur3::calcHash("immigrant")) ||
                    sub_menu_v.size() <= getMenuIndex(menu, MurMur3::calcHash("mtdna_region"))
                    ) {
                    PAXS_WARNING("Failed to read Japan region TSV file: " + ryoseikoku_tsv_path + " at line " + std::to_string(i));
                    continue;
                }
                Ryoseikoku ryoseikoku;
                ryoseikoku.id = static_cast<std::uint_least8_t>(std::stoi(sub_menu_v[menu[MurMur3::calcHash("id")]]));
                ryoseikoku.name = sub_menu_v[menu[MurMur3::calcHash("name")]];
                ryoseikoku.region_id = static_cast<std::uint_least8_t>(std::stoi(sub_menu_v[menu[MurMur3::calcHash("region")]]));
                ryoseikoku.settlement_population_min_ad200 = std::stoi(sub_menu_v[menu[MurMur3::calcHash("min_pop_placed_per_cell")]]);
                ryoseikoku.settlement_population_max_ad200 = std::stoi(sub_menu_v[menu[MurMur3::calcHash("max_pop_placed_per_cell")]]);
                ryoseikoku.population[0/*ad200*/] = std::stoi(sub_menu_v[menu[MurMur3::calcHash("init_pop")]]);
                ryoseikoku.population[1/*ad725*/] = std::stoi(sub_menu_v[menu[MurMur3::calcHash("immigrant")]]);

                const std::string& mtdna_region_str = sub_menu_v[menu[MurMur3::calcHash("mtdna_region")]];
                ryoseikoku.mtdna_region_hash = MurMur3::calcHash(mtdna_region_str.size(), mtdna_region_str.c_str());
                ryoseikoku_list.emplace_back(ryoseikoku);
                ++i;
            }

        }

    public:
        explicit JapanProvinces(const std::string& japan_provinces_path) noexcept {

            //const std::string japan_region_tsv_path = japan_provinces_path + "/JapanRegion.tsv";

            inputMtDNA_List(japan_provinces_path);
            inputMtDNA_Region(japan_provinces_path);
            inputDistrict(japan_provinces_path);

        }

        // 古い実装
        explicit JapanProvinces(const std::string& japan_region_tsv_path, const std::string& ryoseikoku_tsv_path, int aaa) noexcept {
            std::vector<std::vector<std::string>> japan_region_tsv = File::readTSV(AppConfig::getInstance()->getRootPath() + japan_region_tsv_path);
            if (japan_region_tsv.empty()) {
                PAXS_WARNING("Failed to read Japan region TSV file: " + japan_region_tsv_path);
            }

            std::vector<std::vector<std::string>> ryoseikoku_tsv = File::readTSV(AppConfig::getInstance()->getRootPath() + ryoseikoku_tsv_path);
            if (ryoseikoku_tsv.empty()) {
                PAXS_WARNING("Failed to read Ryoseikoku TSV file: " + ryoseikoku_tsv_path);
            }

            for (std::size_t i = 1; i < japan_region_tsv.size(); ++i) {
                try {
                    JapanRegion japan_region;
                    japan_region.id = static_cast<std::uint_least8_t>(std::stoi(japan_region_tsv[i][0]));
                    japan_region.name = japan_region_tsv[i][1];
                    japan_region.population = std::stoi(japan_region_tsv[i][2]);
                    japan_regions.emplace_back(japan_region);
                } catch (const std::invalid_argument&) {
                    PAXS_WARNING("Failed to read Japan region TSV file: " + japan_region_tsv_path + " at line " + std::to_string(i));
                }
            }

            for (std::size_t i = 1; i < ryoseikoku_tsv.size(); ++i) {
                try {
                    Ryoseikoku ryoseikoku;
                    ryoseikoku.id = static_cast<std::uint_least8_t>(std::stoi(ryoseikoku_tsv[i][0]));
                    ryoseikoku.name = ryoseikoku_tsv[i][1];
                    ryoseikoku.region_id = static_cast<std::uint_least8_t>(std::stoi(ryoseikoku_tsv[i][2]));
                    ryoseikoku.settlement_population_min_ad200 = std::stoi(ryoseikoku_tsv[i][3]);
                    ryoseikoku.settlement_population_max_ad200 = std::stoi(ryoseikoku_tsv[i][4]);
                    ryoseikoku.population[0/*ad200*/] = std::stoi(ryoseikoku_tsv[i][5]);
                    ryoseikoku.population[1/*ad725*/] = std::stoi(ryoseikoku_tsv[i][6]);
                    ryoseikoku_list.emplace_back(ryoseikoku);
                } catch (const std::invalid_argument&) {
                    PAXS_WARNING("Failed to read Ryoseikoku TSV file: " + ryoseikoku_tsv_path + " at line " + std::to_string(i));
                }
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

        /// @brief Get a ryoseikoku from the ID of the ryoseikoku in Japan.
        /// @brief 日本の令制国のIDから令制国を取得する
        Ryoseikoku& getRyoseikoku(const std::uint_least8_t id) noexcept {
            for (auto& ryoseikoku : ryoseikoku_list) {
                if (ryoseikoku.id == id) {
                    return ryoseikoku;
                }
            }
            PAXS_WARNING("Failed to get Ryoseikoku: " + std::to_string(id));

            return ryoseikoku_list[0];
        }
        const Ryoseikoku& cgetRyoseikoku(const std::uint_least8_t id) const noexcept {
            for (const auto& ryoseikoku : ryoseikoku_list) {
                if (ryoseikoku.id == id) {
                    return ryoseikoku;
                }
            }
            PAXS_WARNING("Failed to get Ryoseikoku: " + std::to_string(id));

            return ryoseikoku_list[0];
        }
        std::uint_least8_t getMtDNA(const std::uint_least8_t id, std::mt19937& gen) noexcept {
            for (const auto& ryoseikoku : ryoseikoku_list) {
                if (ryoseikoku.id == id) {
                    auto& weight_list = mtdna_region_list.at(ryoseikoku.mtdna_region_hash);
                    return weight_list.id[weight_list.dist(gen)];
                }
            }
            PAXS_WARNING("Failed to get Ryoseikoku: " + std::to_string(id));

            auto& weight_list = mtdna_region_list.at(ryoseikoku_list[0].mtdna_region_hash);
            return weight_list.id[weight_list.dist(gen)];
        }

        /// @brief 日本の令制国のIDから人口を取得する
        /// @param id 日本の令制国のID
        /// @return 人口
        /// @note IDが不正な場合は0を返す
        std::uint_least32_t getRyoseikokuPopulationAd200(const std::uint_least8_t id) const noexcept {
            for (const auto& ryoseikoku : ryoseikoku_list) {
                if (ryoseikoku.id == id) {
                    return ryoseikoku.population[0/*ad200*/];
                }
            }
            PAXS_WARNING("Failed to get Ryoseikoku population: " + std::to_string(id));

            return 0;
        }

        /// @brief 日本の令制国のIDから地方区分のIDを取得する
        /// @param id 日本の令制国のID
        /// @return 地方区分のID
        /// @note IDが不正な場合は0を返す
        std::uint_least8_t getJapanRegionId(const std::uint_least8_t id) const noexcept {
            for (const auto& ryoseikoku : ryoseikoku_list) {
                if (ryoseikoku.id == id) {
                    return ryoseikoku.region_id;
                }
            }
            PAXS_WARNING("Failed to get Japan region ID: " + std::to_string(id));

            return 0;
        }

        /// @brief Get a list of Ryoseikoku
        /// @brief 令制国のリストを取得する
        std::vector<Ryoseikoku>& getRyoseikokuList() noexcept {
            return ryoseikoku_list;
        }
        const std::vector<Ryoseikoku>& cgetRyoseikokuList() const noexcept {
            return ryoseikoku_list;
        }
    private:
        std::vector<JapanRegion> japan_regions; // 日本の地方区分
        std::vector<Ryoseikoku> ryoseikoku_list; // 日本の令制国
        std::unordered_map<std::uint_least32_t, mtDNA_Region> mtdna_region_list; // mtDNA 地方区分
        //std::vector<std::uint_least32_t> mtdna_region_hash_list; // mtDNA ハッシュ計算用
        std::vector<std::string> mtdna_list; // mtDNA

    };

}

#endif // !PAX_SAPIENTICA_PROVINCES_JAPAN_HPP
