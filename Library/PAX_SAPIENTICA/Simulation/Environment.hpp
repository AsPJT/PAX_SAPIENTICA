/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIMULATION_ENVIRONMENT_HPP
#define PAX_SAPIENTICA_SIMULATION_ENVIRONMENT_HPP

/*##########################################################################################

##########################################################################################*/

#include <cmath>
#include <iostream>
#include <random>
#include <regex>
#include <stdexcept>
#include <unordered_map>
#include <variant>

#include <PAX_SAPIENTICA/File.hpp>
#include <PAX_SAPIENTICA/Logger.hpp>
#include <PAX_SAPIENTICA/Simulation/Data.hpp>
#include <PAX_SAPIENTICA/Simulation/SimulationConst.hpp>
#include <PAX_SAPIENTICA/Type/Vector2.hpp>

namespace paxs {

    /// @brief A class that manages data required for simulation.
    /// @brief シミュレーションに必要なデータを管理するクラス
    template <typename GridType>
    class Environment {
    public:
        using Vector2 = paxs::Vector2<GridType>;

        using DataVariant = std::variant<Data<std::uint_least8_t>, Data<std::uint_least32_t>, Data<float>>;

        /// @brief Start position of the simulation.
        /// @brief シミュレーションデータのマップ
        std::unordered_map<std::string, DataVariant> data_map;

        constexpr explicit Environment() noexcept = default;
        explicit Environment(const std::string& setting_file_path, const Vector2& start_position, const Vector2& end_position, const int z) : start_position(start_position), end_position(end_position), z(z) {
            std::vector<std::vector<std::string>> settings;
            try {
                settings = File::readTSV(setting_file_path);
            } catch (const std::runtime_error&) {
                Logger logger("Save/error_log.txt");
                const std::string message = "Failed to read setting file: " + setting_file_path;
                logger.log(Logger::Level::ERROR, __FILE__, __LINE__, message);
                throw std::runtime_error(message);
            }

            // 1行目からdata_typeのカラム番号を取得
            int key_column = -1;
            int data_type_column = -1;
            int file_path_column = -1;
            int z_column = -1;
            for(std::size_t i = 0;i < settings[0].size();++i) {
                if(settings[0][i] == "key") {
                    key_column = int(i);
                }
                if(settings[0][i] == "data_type") {
                    data_type_column = int(i);
                }
                if(settings[0][i] == "file_path") {
                    file_path_column = int(i);
                }
                if(settings[0][i] == "z") {
                    z_column = int(i);
                }
            }
            if(key_column == -1 || data_type_column == -1 || file_path_column == -1 || z_column == -1) {
                Logger logger("Save/error_log.txt");
                const std::string message = "key or data_type or file_path or z is not found: " + setting_file_path;
                logger.log(Logger::Level::ERROR, __FILE__, __LINE__, message);
                throw std::runtime_error(message);
            }

            for(std::size_t i = 1;i < settings.size();++i) {
                // 型名をstringからTに変換
                const std::string data_type = settings[i][data_type_column];
                const std::string key = settings[i][key_column];
                if(data_type == "u8"){
                    data_map.emplace(key, Data<std::uint_least8_t>(settings[i][file_path_column], key, start_position, end_position, std::stoi(settings[i][z_column]), z, DataType::u8));
                } else if(data_type == "u32"){
                    data_map.emplace(key, Data<std::uint_least32_t>(settings[i][file_path_column], key, start_position, end_position, std::stoi(settings[i][z_column]), z, DataType::u32));
                } else if(data_type == "f32"){
                    data_map.emplace(key, Data<float>(settings[i][file_path_column], key, start_position, end_position, std::stoi(settings[i][z_column]), z, DataType::f32));
                } else {
                    Logger logger("Save/error_log.txt");
                    const std::string message = "data_type is not found: " + data_type + " in " + setting_file_path;
                    logger.log(Logger::Level::WARNING, __FILE__, __LINE__, message);
                }
            }
        }

        /// @brief シミュレーションの左上の座標の取得
        constexpr Vector2 getStartPosition() const noexcept { return start_position; }
        /// @brief シミュレーションの右下の座標の取得
        constexpr Vector2 getEndPosition() const noexcept { return end_position; }

        /// @brief Get data.
        /// @brief データの取得
        /// @param key Data's key. データのキー
        /// @param position Position of the data to be acquired. 取得したいデータの座標
        template <typename U>
        U getData(const std::string& key, const Vector2& position) const {
            if(data_map.count(key) == 0) {
                Logger logger("Save/error_log.txt");
                const std::string message = "key is not found: " + key;
                logger.log(Logger::Level::ERROR, __FILE__, __LINE__, message);
                throw std::runtime_error(message);
            }
            return std::get<Data<U>>(data_map.at(key)).getValue(position);
        }

        /// @brief Is it possible to live?
        /// @brief 居住可能かどうかの判定
        /// @details It is land and the slope is less than a certain value.
        /// @details 陸地であるかつ、傾斜が一定以下であること
        bool isLive(const Vector2& position) const {
            try {
                return isLand(position) && getSlope(position) <= 5;
            } catch (const std::exception&) {
                Logger logger("Save/error_log.txt");
                const std::string message = "Failed to judge live";
                logger.log(Logger::Level::ERROR, __FILE__, __LINE__, message);
                throw std::runtime_error(message);
            }
        }

        /// @brief Get slope.
        /// @brief 傾斜の取得
        float getSlope(const Vector2& position) const {
            try {
                return getData<float>("slope", position);
            } catch (const std::exception&) {
                Logger logger("Save/error_log.txt");
                const std::string message = "Failed to get slope";
                logger.log(Logger::Level::ERROR, __FILE__, __LINE__, message);
                throw std::runtime_error(message);
            }
        }

        /// @brief Get elevation.
        /// @brief 標高の取得
        float getElevation(const Vector2& position) const {
            try {
                return getData<float>("elevation", position);
            } catch (const std::exception&) {
                Logger logger("Save/error_log.txt");
                const std::string message = "Failed to get elevation";
                logger.log(Logger::Level::ERROR, __FILE__, __LINE__, message);
                throw std::runtime_error(message);
            }
        }

        /// @brief Is it land?
        /// @brief 陸地かどうかの判定
        virtual bool isLand(const Vector2& position) const {
            try {
                auto value = getData<std::uint_least8_t>("gbank", position);
                return static_cast<int>(value) == static_cast<int>('1');
            } catch (const std::exception&) {
                Logger logger("Save/error_log.txt");
                const std::string message = "Failed to get gbank";
                logger.log(Logger::Level::ERROR, __FILE__, __LINE__, message);
                throw std::runtime_error(message);
            }
        }

        /// @brief Is it water?
        /// @brief 淡水かどうかの判定
        virtual bool isWater(const Vector2& position) const {
            try {
                auto value = getData<std::uint_least8_t>("water", position);
                return static_cast<int>(value) == static_cast<int>('1');
            } catch (const std::exception&) {
                Logger logger("Save/error_log.txt");
                const std::string message = "Failed to get water";
                logger.log(Logger::Level::ERROR, __FILE__, __LINE__, message);
                throw std::runtime_error(message);
            }
        }
    private:
        Vector2 start_position; // シミュレーションの左上の座標
        Vector2 end_position; // シミュレーションの右下の座標
        int z; // シミュレーションのz値
    };
}

#endif // !PAX_SAPIENTICA_SIMULATION_ENVIRONMENT_HPP