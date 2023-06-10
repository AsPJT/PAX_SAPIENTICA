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
#include <map>
#include <random>
#include <regex>

#include <PAX_SAPIENTICA/CuiOutput/Graphic.hpp>
#include <PAX_SAPIENTICA/FileRead/Convert.hpp>
#include <PAX_SAPIENTICA/FileRead/Read.hpp>
#include <PAX_SAPIENTICA/FileRead/Split.hpp>
#include <PAX_SAPIENTICA/Simulation/Data.hpp>
#include <PAX_SAPIENTICA/Simulation/GeographicInformation.hpp>
#include <PAX_SAPIENTICA/Type/Vector2.hpp>

namespace paxs {
    template <typename T>
    class Environment {
    public:
        using Vector2 = paxs::Vector2<T>;

        const int pixel_size = 256;

        using DataVariant = std::variant<Data<std::uint_least8_t>, Data<std::uint_least32_t>, Data<float>>;
        std::map<std::string, DataVariant> data_map;

        Environment() = default;
        Environment(const std::string& setting_file_path, const Vector2& start_position, const Vector2& end_position, const int z) : start_position(start_position), end_position(end_position), z(z) {
            std::vector<std::vector<std::string>> settings = readTSV(setting_file_path);
            // 1行目からdata_typeのカラム番号を取得
            int key_column = -1;
            int data_type_column = -1;
            int file_path_column = -1;
            int z_column = -1;
            for(std::size_t i = 0;i < settings[0].size();++i) {
                if(settings[0][i] == "key") {
                    key_column = i;
                }
                if(settings[0][i] == "data_type") {
                    data_type_column = i;
                }
                if(settings[0][i] == "file_path") {
                    file_path_column = i;
                }
                if(settings[0][i] == "z") {
                    z_column = i;
                }
            }
            if(key_column == -1 | data_type_column == -1 | file_path_column == -1 | z_column == -1) {
                std::cerr << "Error: column is not found." << std::endl;
                exit(1);
            }

            for(std::size_t i = 1;i < settings.size();++i) {
                // 型名をstringからTに変換
                std::string data_type = settings[i][data_type_column];
                std::string key = settings[i][key_column];
                if(data_type == "u8"){
                    data_map.emplace(key, Data<std::uint_least8_t>(settings[i][file_path_column], key, start_position, end_position, std::stoi(settings[i][z_column]), z, DataType::u8));
                } else if(data_type == "u32"){
                    data_map.emplace(key, Data<std::uint_least32_t>(settings[i][file_path_column], key, start_position, end_position, std::stoi(settings[i][z_column]), z, DataType::u32));
                } else if(data_type == "f32"){
                    data_map.emplace(key, Data<float>(settings[i][file_path_column], key, start_position, end_position, std::stoi(settings[i][z_column]), z, DataType::f32));
                } else {
                    std::cerr << "Error: data_type is not found." << std::endl;
                    exit(1);
                }
            }
        }

        Vector2 getStartPosition() const { return start_position; }
        Vector2 getEndPosition() const { return end_position; }

        // keyとpositionを指定してデータを取得
        template <typename U>
        U getData(const std::string& key, const Vector2& position) const {
            if(data_map.count(key) == 0) {
                std::cerr << "Error: key is not found." << std::endl;
                exit(1);
            }
            return std::get<Data<U>>(data_map.at(key)).getValue(position);
        }

        bool isLand(const Vector2& position) const {
            auto value = getData<std::uint_least8_t>("gbank", position);
            return static_cast<int>(value) == 49; // char '1' -> int 49

        }
    private:
        Vector2 start_position;
        Vector2 end_position;
        int z;
    };
}

#endif // !PAX_SAPIENTICA_SIMULATION_ENVIRONMENT_HPP