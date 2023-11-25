/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_PROVINCES_JAPAN_HPP
#define PAX_SAPIENTICA_PROVINCES_JAPAN_HPP

/*##########################################################################################

##########################################################################################*/

#include <vector>

#include <PAX_SAPIENTICA/File.hpp>
#include <PAX_SAPIENTICA/Logger.hpp>

namespace paxs {

        /// @brief A struct that represents a region in Japan.
        /// @brief 日本の地方区分を表す構造体
        struct JapanRegion {
            std::uint_least8_t id;
            std::string name;
            std::uint_least32_t population; // 人口
        };

        /// @brief A struct that represents a prefecture in Japan.
        /// @brief 日本の令制国を表す構造体
        struct Ryoseikoku {
            std::uint_least8_t id;
            std::string name;
            std::uint_least8_t region_id; // 対応する地方区分ID
            std::uint_least32_t settlement_population_min_ad200;
            std::uint_least32_t settlement_population_max_ad200;
            std::uint_least32_t population_ad200;
            std::uint_least32_t population_ad725;
        };

    /// @brief A class that represents a prefecture in Japan.
    /// @brief 日本の州を表すクラス
    class JapanProvinces {
    public:
        explicit JapanProvinces(const std::string& japan_region_tsv_path, const std::string& ryoseikoku_tsv_path) {
            std::vector<std::vector<std::string>> japan_region_tsv;
            try {
                japan_region_tsv = File::readTSV(japan_region_tsv_path);
            } catch (const std::runtime_error&) {
                Logger logger("Save/error_log.txt");
                const std::string message = "Failed to read Japan region TSV file: " + japan_region_tsv_path;
                logger.log(Logger::Level::PAX_ERROR, __FILE__, __LINE__, message);
                throw std::runtime_error(message);
            }
            std::vector<std::vector<std::string>> ryoseikoku_tsv;
            try {
                ryoseikoku_tsv = File::readTSV(ryoseikoku_tsv_path);
            } catch (const std::runtime_error&) {
                Logger logger("Save/error_log.txt");
                const std::string message = "Failed to read Ryoseikoku TSV file: " + ryoseikoku_tsv_path;
                logger.log(Logger::Level::PAX_ERROR, __FILE__, __LINE__, message);
                throw std::runtime_error(message);
            }

            for (std::size_t i = 1; i < japan_region_tsv.size(); ++i) {
                try {
                    JapanRegion japan_region;
                    japan_region.id = std::stoi(japan_region_tsv[i][0]);
                    japan_region.name = japan_region_tsv[i][1];
                    japan_region.population = std::stoi(japan_region_tsv[i][2]);
                    japan_regions.emplace_back(japan_region);
                } catch (const std::invalid_argument&) {
                    Logger logger("Save/error_log.txt");
                    const std::string message = "Failed to read Japan region TSV file: " + japan_region_tsv_path + " at line " + std::to_string(i);
                    logger.log(Logger::Level::PAX_WARNING, __FILE__, __LINE__, message);
                }
            }

            for (std::size_t i = 1; i < ryoseikoku_tsv.size(); ++i) {
                try {
                    Ryoseikoku ryoseikoku;
                    ryoseikoku.id = std::stoi(ryoseikoku_tsv[i][0]);
                    ryoseikoku.name = ryoseikoku_tsv[i][1];
                    ryoseikoku.region_id = std::stoi(ryoseikoku_tsv[i][2]);
                    ryoseikoku.settlement_population_min_ad200 = std::stoi(ryoseikoku_tsv[i][3]);
                    ryoseikoku.settlement_population_max_ad200 = std::stoi(ryoseikoku_tsv[i][4]);
                    ryoseikoku.population_ad200 = std::stoi(ryoseikoku_tsv[i][5]);
                    ryoseikoku.population_ad725 = std::stoi(ryoseikoku_tsv[i][6]);
                    ryoseikoku_list.emplace_back(ryoseikoku);
                } catch (const std::invalid_argument&) {
                    Logger logger("Save/error_log.txt");
                    const std::string message = "Failed to read Ryoseikoku TSV file: " + ryoseikoku_tsv_path + " at line " + std::to_string(i);
                    logger.log(Logger::Level::PAX_WARNING, __FILE__, __LINE__, message);
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
            Logger logger("Save/error_log.txt");
            const std::string message = "Failed to get Japan region population: " + std::to_string(id);
            logger.log(Logger::Level::PAX_WARNING, __FILE__, __LINE__, message);

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
            Logger logger("Save/error_log.txt");
            const std::string message = "Failed to get Ryoseikoku: " + std::to_string(id);
            logger.log(Logger::Level::PAX_WARNING, __FILE__, __LINE__, message);

            return ryoseikoku_list[0];
        }
        const Ryoseikoku& cgetRyoseikoku(const std::uint_least8_t id) const noexcept {
            for (const auto& ryoseikoku : ryoseikoku_list) {
                if (ryoseikoku.id == id) {
                    return ryoseikoku;
                }
            }
            Logger logger("Save/error_log.txt");
            const std::string message = "Failed to get Ryoseikoku: " + std::to_string(id);
            logger.log(Logger::Level::PAX_WARNING, __FILE__, __LINE__, message);

            return ryoseikoku_list[0];
        }

        /// @brief 日本の令制国のIDから人口を取得する
        /// @param id 日本の令制国のID
        /// @return 人口
        /// @note IDが不正な場合は0を返す
        std::uint_least32_t getRyoseikokuPopulationAd200(const std::uint_least8_t id) const noexcept {
            for (const auto& ryoseikoku : ryoseikoku_list) {
                if (ryoseikoku.id == id) {
                    return ryoseikoku.population_ad200;
                }
            }
            Logger logger("Save/error_log.txt");
            const std::string message = "Failed to get Ryoseikoku population: " + std::to_string(id);
            logger.log(Logger::Level::PAX_WARNING, __FILE__, __LINE__, message);

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
            Logger logger("Save/error_log.txt");
            const std::string message = "Failed to get Japan region ID: " + std::to_string(id);
            logger.log(Logger::Level::PAX_WARNING, __FILE__, __LINE__, message);

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

    };

}

#endif // !PAX_SAPIENTICA_PROVINCES_JAPAN_HPP
