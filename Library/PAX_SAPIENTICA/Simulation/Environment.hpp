/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIMULATION_ENVIRONMENT_HPP
#define PAX_SAPIENTICA_SIMULATION_ENVIRONMENT_HPP

/*##########################################################################################

##########################################################################################*/

#include <cmath>
#include <iostream>
#include <memory>
#include <random>
#include <regex>
#include <stdexcept>
#include <unordered_map>
#include <variant>

#include <PAX_SAPIENTICA/AppConfig.hpp>
#include <PAX_SAPIENTICA/File.hpp>
#include <PAX_SAPIENTICA/Logger.hpp>
#include <PAX_SAPIENTICA/MurMur3.hpp>
#include <PAX_SAPIENTICA/Simulation/Data.hpp>
#include <PAX_SAPIENTICA/Simulation/SimulationConst.hpp>
#include <PAX_SAPIENTICA/Type/Vector2.hpp>

namespace paxs {

    /// @brief A class that manages data required for simulation.
    /// @brief シミュレーションに必要なデータを管理するクラス
    class Environment {
    public:
        using Vector2 = paxs::Vector2<GridType>;

        using DataVariant = std::variant<Data<std::uint_least8_t>, Data<std::uint_least32_t>, Data<float>, Data<std::int_least16_t>>;

        /// @brief Start position of the simulation.
        /// @brief シミュレーションデータのマップ
        std::unordered_map<std::uint_least32_t, std::unique_ptr<DataVariant>> data_map;

        explicit Environment() noexcept = default;
        explicit Environment(const std::string& setting_file_path) noexcept {
            std::vector<std::vector<std::string>> settings;

            settings = File::readTSV(AppConfig::getInstance()->getRootPath() + setting_file_path);

            if (settings.empty()) {
                PAXS_ERROR("Failed to read TSV file: " + setting_file_path);
                return;
            }

            // 1行目からdata_typeのカラム番号を取得
            int key_column = -1;
            int data_type_column = -1;
            int file_path_column = -1;
            int z_column = -1;
            for (std::size_t i = 0; i < settings[0].size(); ++i) {
                if (settings[0][i] == "key") {
                    key_column = int(i);
                }
                if (settings[0][i] == "data_type") {
                    data_type_column = int(i);
                }
                if (settings[0][i] == "file_path") {
                    file_path_column = int(i);
                }
                if (settings[0][i] == "z") {
                    z_column = int(i);
                }
            }
            if (key_column == -1 || data_type_column == -1 || file_path_column == -1 || z_column == -1) {
                PAXS_WARNING("key or data_type or file_path or z is not found in " + setting_file_path);
                return;
            }

            for (std::size_t i = 1; i < settings.size(); ++i) {
                // 型名をstringからTに変換
                const std::uint_least32_t data_type = MurMur3::calcHash(settings[i][data_type_column].size(), settings[i][data_type_column].c_str());
                const std::uint_least32_t key = MurMur3::calcHash(settings[i][key_column].size(), settings[i][key_column].c_str());
                const std::string& key_str = settings[i][key_column];
                if (data_type == MurMur3::calcHash("u8")) {
                    data_map.emplace(key, std::make_unique<DataVariant>(Data<std::uint_least8_t>(settings[i][file_path_column], key_str, std::stoi(settings[i][z_column]))));
                }
                else if (data_type == MurMur3::calcHash("u32")) {
                    data_map.emplace(key, std::make_unique<DataVariant>(Data<std::uint_least32_t>(settings[i][file_path_column], key_str, std::stoi(settings[i][z_column]))));
                }
                else if (data_type == MurMur3::calcHash("f32")) {
                    data_map.emplace(key, std::make_unique<DataVariant>(Data<float>(settings[i][file_path_column], key_str, std::stoi(settings[i][z_column]))));
                }
                else if (data_type == MurMur3::calcHash("s16")) {
                    data_map.emplace(key, std::make_unique<DataVariant>(Data<std::int_least16_t>(settings[i][file_path_column], key_str, std::stoi(settings[i][z_column]))));
                }
                else {
                    PAXS_WARNING("data_type is not found in " + setting_file_path);
                }
            }
        }

        /// @brief Get data.
        /// @brief データの取得
        /// @param key Data's key. データのキー
        /// @param position Position of the data to be acquired. 取得したいデータの座標
        template <typename U>
        U getData(const std::uint_least32_t key, const Vector2& position) const {
            if (data_map.count(key) == 0) {
                const std::string message = "key is not found: " + std::to_string(key);
                PAXS_ERROR(message);
                throw std::runtime_error(message);
            }
            return std::get<Data<U>>(*data_map.at(key)).getValue(position);
        }

        /// @brief Get the land position list.
        /// @brief 陸の位置リストの取得
        void getLandPositions(std::vector<DataGridsType>& keys) const {
            std::get<Data<std::uint_least8_t>>(*data_map.at(SimulationConstants::getInstance()->land_key)).getKeys(keys);
        }

        /// @brief Is it possible to live?
        /// @brief 居住可能かどうかの判定
        /// @details It is land and the slope is less than a certain value.
        /// @details 陸地であるかつ、傾斜が一定以下であること
        bool isLive(const Vector2& position) const {
            return isLand(position) && static_cast<int>(getSlope(position)) <= 163;
        }

        /// @brief Get slope.
        /// @brief 傾斜の取得
        std::uint_least8_t getSlope(const Vector2& position) const noexcept {
            try {
                return getData<std::uint_least8_t>(MurMur3::calcHash("slope"), position);
            }
            catch (const std::exception&) {
                PAXS_ERROR("Failed to get slope");
                return 0;
            }
        }

        /// @brief Get elevation.
        /// @brief 標高の取得
        std::int_least16_t getElevation(const Vector2& position) const noexcept {
            try {
                return getData<std::int_least16_t>(MurMur3::calcHash("elevation"), position);
            }
            catch (const std::exception&) {
                PAXS_ERROR("Failed to get elevation");
                return 0;
            }
        }

        /// @brief Is it land?
        /// @brief 陸地かどうかの判定
        virtual bool isLand(const Vector2& position) const noexcept {
            try {
                auto value = getData<std::uint_least8_t>(SimulationConstants::getInstance()->land_key, position);
                return static_cast<int>(value) >= static_cast<int>(1);
            }
            catch (const std::exception&) {
                PAXS_ERROR("Failed to get land");
                return false;
            }
        }

        /// @brief Is it water?
        /// @brief 淡水かどうかの判定
        virtual bool isWater(const Vector2& position) const noexcept {
            try {
                auto value = getData<std::uint_least8_t>(MurMur3::calcHash("water"), position);
                return static_cast<int>(value) == static_cast<int>('1');
            }
            catch (const std::exception&) {
                PAXS_ERROR("Failed to get water");
                return false;
            }
        }
    };
}

#endif // !PAX_SAPIENTICA_SIMULATION_ENVIRONMENT_HPP
