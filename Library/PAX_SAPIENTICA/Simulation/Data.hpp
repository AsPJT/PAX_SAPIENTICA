/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIMULATION_DATA_HPP
#define PAX_SAPIENTICA_SIMULATION_DATA_HPP

/*##########################################################################################

##########################################################################################*/

#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <unordered_map>

#include <PAX_SAPIENTICA/AppConfig.hpp>
#include <PAX_SAPIENTICA/File.hpp>
#include <PAX_SAPIENTICA/GeographicInformation/Slope.hpp>
#include <PAX_SAPIENTICA/InputFile/SimulationRange.hpp>
#include <PAX_SAPIENTICA/Logger.hpp>
#include <PAX_SAPIENTICA/Simulation/SimulationConst.hpp>
#include <PAX_SAPIENTICA/StatusDisplayer.hpp>
#include <PAX_SAPIENTICA/StringExtensions.hpp>
#include <PAX_SAPIENTICA/Type/Vector2.hpp>

namespace paxs {

    /// @brief A class that holds data required for simulation.
    /// @brief シミュレーションに必要なデータを保持するクラス
    template <typename DataType>
    class Data {
    public:
        using Vector2 = paxs::Vector2<GridType>;
        explicit Data(const std::string& file_path, const std::string name, const int default_z) noexcept : name(name), default_z(default_z) {
            z_mag = std::pow(2, default_z - SimulationConstants::getInstance()->getZ());
            GridType area_x = SimulationConstants::getInstance()->getEndArea().x - SimulationConstants::getInstance()->getStartArea().x + 1;
            column_size = static_cast<int>(area_x * pixel_size * z_mag);

            load(file_path);
        }
        Data(const Data& other) noexcept
            : name(other.name),
            data(other.data),
            default_z(other.default_z),
            z_mag(other.z_mag),
            column_size(other.column_size) {}

        // Copy assignment operator
        constexpr Data& operator=(const Data& other) noexcept {
            if (this != &other) {
                name = other.name;
                data = other.data;
                default_z = other.default_z;
                z_mag = other.z_mag;
                column_size = other.column_size;
            }
            return *this;
        }

        /// @brief Get the data of the specified position.
        /// @brief 指定した位置の値を取得する
        constexpr DataType getValue(const Vector2& position) const noexcept {
            const Vector2 converted_position = position * z_mag;
            auto itr = data.find(converted_position.to<DataGridsType>());
            if(itr == data.end()) {
                return static_cast<DataType>(0);
            }
            return itr->second;
        }

        /// @brief Get the data of the key.
        /// @brief キーをvectorで取得
        /// @return keyのvector
        constexpr void getKeys(std::vector<DataGridsType>& keys) const noexcept {
            keys.reserve(data.size());
            for (const auto& [key, value] : data) {
                keys.emplace_back(key);
            }
        }

    private:
        std::string name; // データの名前
        std::unordered_map<DataGridsType, DataType> data; // データ
        int default_z; // データのz値
        double z_mag; // シミュレーションのz値からデータのz値に変換するときの倍率
        int column_size; // シミュレーションの列数

        /// @brief Load the file.
        /// @brief ファイルのロード
        void load(const std::string& file_path) noexcept {
            std::cout << "Loading " << name << " data..." << std::endl;
            std::vector<std::string> file_names = File::getFileNames(AppConfig::getInstance()->getRootPath() + file_path);

            std::cout << file_names.size() << " files are found." << std::endl;

            if(file_names.size() == 0) {
                PAXS_ERROR("No files are found: " + file_path);
                return;
            }

            if(file_names[0].find(".tsv") != std::string::npos) {
                loadNumericTSV(file_path);
            } else if(file_names[0].find(".txt") != std::string::npos) {
                if (z_mag > 1) loadNumericTextAndCompress(file_names);
                else loadNumericText(file_names);
            } else if(file_names[0].find(".bin") != std::string::npos) {
                if constexpr (std::is_same<DataType, std::uint_least8_t>::value) {
                    loadBinary<paxs::Input8BitBinary>(file_path);
                } else if constexpr (std::is_same<DataType, std::int_least16_t>::value) {
                    loadBinary<paxs::Input16BitBinary>(file_path);
                }
            } else {
                PAXS_WARNING("File type is invalid: " + file_names[0]);
            }
        }

        /// @brief Load binary files.
        /// @brief バイナリファイルのロード
        template <typename BinaryDataType>
        void loadBinary(const std::string& file_path) noexcept {
            const Vector2 start_position = SimulationConstants::getInstance()->getStartArea() * z_mag;
            const Vector2 end_position = SimulationConstants::getInstance()->getEndArea() * z_mag;

            const Vector2 start_xyz_position = SimulationConstants::getInstance()->getStartArea() * pixel_size * z_mag;

            std::uint_least32_t file_count = (end_position.x - start_position.x + 1) * (end_position.y - start_position.y + 1);
            std::uint_least32_t load_count = 0;

            DataType tmp_data[pixel_size * pixel_size]{};

            for (GridType y = start_position.y; y <= end_position.y; ++y) {
                for (GridType x = start_position.x; x <= end_position.x; ++x) {
                    const std::string file_name = "zxy_" + std::to_string(default_z) + "_" + std::to_string(x) + "_" + std::to_string(y) + ".bin";

                    if (!std::filesystem::exists(AppConfig::getInstance()->getRootPath() + file_path + file_name)) {
                        --file_count;
                        StatusDisplayer::displayProgressBar(load_count, file_count);
                        continue;
                    }

                    BinaryDataType bi(file_path + file_name, AppConfig::getInstance()->getRootPath());
                    bi.calc(tmp_data);

                    const Vector2 default_position = Vector2(x, y) * pixel_size - start_xyz_position;
                    for(std::size_t py = 0;py < pixel_size;++py) {
                        for(std::size_t px = 0;px < pixel_size;++px) {
                            const Vector2 position = default_position + Vector2((GridType)px, (GridType)py);
                            data[position.to<DataGridsType>()] = static_cast<DataType>(tmp_data[py * pixel_size + px]);
                        }
                    }

                    ++load_count;
                    StatusDisplayer::displayProgressBar(load_count, file_count);
                }
            }

            std::cout << std::endl << "Loading " << name << " is completed." << std::endl;
            std::cout << load_count << " files are loaded.\n" << std::endl;
        }

        /// @brief Load binary files.
        /// @brief バイナリファイルのロード
        template <typename BinaryDataType>
        [[maybe_unused]]
        void loadBinary(const std::vector<std::string>& file_names) noexcept {
            std::uint_least32_t file_count = 0;
            std::uint_least32_t load_count = 0;

            DataType tmp_data[pixel_size * pixel_size]{};

            const Vector2 start_xyz_position = SimulationConstants::getInstance()->getStartArea() * pixel_size * z_mag;
            const Vector2 end_xyz_position = SimulationConstants::getInstance()->getEndArea() * pixel_size * z_mag;
            const Vector2 size = end_xyz_position - start_xyz_position;

            for(const auto& file_name : file_names) {
                StatusDisplayer::displayProgressBar(file_count, int(file_names.size()));

                Vector2 xyz_position;
                try {
                    xyz_position = getXAndYFromFileName(file_name);
                } catch (const std::exception&) {
                    PAXS_WARNING("File name is invalid: " + file_name);
                    ++file_count;
                    continue;
                }

                const Vector2 default_position = xyz_position * pixel_size - start_xyz_position;
                if(default_position.x < 0 || default_position.y < 0 || default_position.x > size.x || default_position.y > size.y) {
                    ++file_count;
                    continue;
                }

                BinaryDataType bi(file_name, "");
                bi.calc(tmp_data);

                for(std::size_t y = 0;y < pixel_size;++y) {
                    for(std::size_t x = 0;x < pixel_size;++x) {
                        const Vector2 position = default_position + Vector2((GridType)x, (GridType)y);
                        data[position.toU64()] = static_cast<DataType>(tmp_data[y * pixel_size + x]);
                    }
                }

                ++file_count;
                ++load_count;
            }
            StatusDisplayer::displayProgressBar(file_count, int(file_names.size()));
            std::cout << std::endl << "Loading " << name << " is completed." << std::endl;
            std::cout << load_count << " files are loaded.\n" << std::endl;
        }

        /// @brief Load numeric TSV files.
        /// @brief 数値TSVファイルのロード
        void loadNumericTSV(const std::string& file_path) noexcept {
            const Vector2 start_position = SimulationConstants::getInstance()->getStartArea() * z_mag;
            const Vector2 end_position = SimulationConstants::getInstance()->getEndArea() * z_mag;

            const Vector2 start_xyz_position = SimulationConstants::getInstance()->getStartArea() * pixel_size * z_mag;

            std::uint_least32_t file_count = (end_position.x - start_position.x + 1) * (end_position.y - start_position.y + 1);
            std::uint_least32_t load_count = 0;

            for (GridType y = start_position.y; y <= end_position.y; ++y) {
                for (GridType x = start_position.x; x <= end_position.x; ++x) {
                    const std::string file_name = "zxy_" + std::to_string(default_z) + "_" + std::to_string(x) + "_" + std::to_string(y) + ".tsv";

                    if (!std::filesystem::exists(AppConfig::getInstance()->getRootPath() + file_path + file_name)) {
                        --file_count;
                        StatusDisplayer::displayProgressBar(load_count, file_count);
                        continue;
                    }

                    std::vector<std::string> file = File::readFile(AppConfig::getInstance()->getRootPath() + file_path + file_name);
                    if (file.size() == 0) {
                        PAXS_WARNING("File is empty: " + file_name);
                        ++file_count;
                        continue;
                    }


                    const Vector2 default_position = Vector2(x, y) * pixel_size - start_xyz_position;
                    for(std::size_t py = 0;py < file.size();++py) {
                        // タブ区切り
                        const std::vector<std::string> values = StringExtensions::split(file[py], '\t');
                        for(std::size_t px = 0;px < values.size();++px) {
                            const Vector2 position = default_position + Vector2((GridType)px, (GridType)py);
                            if(values[px] == "") {
                                continue;
                            }
                            // T型に変換
                            try {
                                if constexpr (std::is_same<DataType, std::uint_least8_t>::value || std::is_same<DataType, std::uint_least32_t>::value) {
                                    int value = std::stoi(values[px]);
                                    if(value == 0) continue;
                                    data[position.to<DataGridsType>()] = static_cast<DataType>(value);
                                } else if constexpr (std::is_same<DataType, float>::value) {
                                    data[position.to<DataGridsType>()] = static_cast<DataType>(std::stod(values[px]));
                                }
                            } catch (const std::invalid_argument&/*ia*/) {
                                PAXS_WARNING("File contains invalid value: " + file_name);
                                continue;
                            } catch (const std::out_of_range&/*oor*/) {
                                PAXS_WARNING("File contains out of range value: " + file_name);
                                continue;
                            }
                        }
                    }

                    ++load_count;
                    StatusDisplayer::displayProgressBar(load_count, file_count);
                }
            }

            std::cout << std::endl << "Loading " << name << " is completed." << std::endl;
            std::cout << load_count << " files are loaded.\n" << std::endl;
        }

        /// @brief Load numeric TSV files.
        /// @brief 数値TSVファイルのロード
        [[maybe_unused]]
        void loadNumericTSV(const std::vector<std::string>& file_names) noexcept {
            std::uint_least32_t file_count = 0;
            std::uint_least32_t load_count = 0;

            const Vector2 start_xyz_position = SimulationConstants::getInstance()->getStartArea() * pixel_size * z_mag;
            const Vector2 end_xyz_position = SimulationConstants::getInstance()->getEndArea() * pixel_size * z_mag;
            const Vector2 size = end_xyz_position - start_xyz_position;

            for(const auto& file_name : file_names) {
                StatusDisplayer::displayProgressBar(file_count, int(file_names.size()));

                Vector2 xyz_position;
                try {
                    xyz_position = getXAndYFromFileName(file_name);
                } catch (const std::exception&) {
                    PAXS_WARNING("File name is invalid: " + file_name);
                    ++file_count;
                    continue;
                }

                const Vector2 default_position = xyz_position * pixel_size - start_xyz_position;
                if(default_position.x < 0 || default_position.y < 0 || default_position.x > size.x || default_position.y > size.y) {
                    ++file_count;
                    continue;
                }

                std::vector<std::string> file = File::readFile(file_name);
                if (file.size() == 0) {
                    PAXS_WARNING("File is empty: " + file_name);
                    ++file_count;
                    continue;
                }

                for(std::size_t y = 0;y < file.size();++y) {
                    // タブ区切り
                    const std::vector<std::string> values = StringExtensions::split(file[y], '\t');
                    for(std::size_t x = 0;x < values.size();++x) {
                        const Vector2 position = default_position + Vector2((GridType)x, (GridType)y);
                        if(values[x] == "") {
                            continue;
                        }
                        // T型に変換
                        try {
                            if constexpr (std::is_same<DataType, std::uint_least8_t>::value || std::is_same<DataType, std::uint_least32_t>::value) {
                                int value = std::stoi(values[x]);
                                if(value == 0) continue;
                                data[position.to<DataType>()] = static_cast<DataType>(value);
                            } else if constexpr (std::is_same<DataType, float>::value) {
                                data[position.to<DataType>()] = static_cast<DataType>(std::stod(values[x]));
                            }
                        } catch (const std::invalid_argument&/*ia*/) {
                            PAXS_WARNING("File contains invalid value: " + file_name);
                            continue;
                        } catch (const std::out_of_range&/*oor*/) {
                            PAXS_WARNING("File contains out of range value: " + file_name);
                            continue;
                        }
                    }
                }
                ++file_count;
                ++load_count;
            }
            StatusDisplayer::displayProgressBar(file_count, int(file_names.size()));
            std::cout << std::endl << "Loading " << name << " is completed." << std::endl;
            std::cout << load_count << " files are loaded.\n" << std::endl;
        }

        /// @brief Load numeric text file.
        /// @brief 数値テキストファイルのロード
        void loadNumericText(const std::vector<std::string>& file_names) noexcept {
            std::uint_least32_t file_count = 0;
            std::uint_least32_t load_count = 0;

            const Vector2 start_xyz_position = SimulationConstants::getInstance()->getStartArea() * pixel_size * z_mag;
            const Vector2 end_xyz_position = SimulationConstants::getInstance()->getEndArea() * pixel_size * z_mag;
            const Vector2 size = end_xyz_position - start_xyz_position;

            for(const auto& file_name : file_names) {
                StatusDisplayer::displayProgressBar(file_count, int(file_names.size()));

                Vector2 xyz_position;
                try {
                    xyz_position = getXAndYFromFileName(file_name);
                } catch (const std::exception&) {
                    PAXS_WARNING("File name is invalid: " + file_name);
                    ++file_count;
                    continue;
                }

                const Vector2 default_position = xyz_position * pixel_size - start_xyz_position;
                if(default_position.x < 0 || default_position.y < 0 || default_position.x > size.x || default_position.y > size.y) {
                    ++file_count;
                    continue;
                }

                std::vector<std::string> file = File::readFile(file_name);
                if (file.size() == 0) {
                    PAXS_WARNING("File is empty: " + file_name);
                    ++file_count;
                    continue;
                }

                for(std::size_t y = 0;y < file.size();++y) {
                    for(std::size_t x = 0;x < file[y].size();++x) {
                        // T型に変換
                        if (file[y][x] == '0') continue;
                        const Vector2 position = default_position + Vector2((GridType)x, (GridType)y);
                        data[position.to<DataGridsType>()] = static_cast<DataType>(file[y][x]);
                    }
                }
                ++file_count;
                ++load_count;
            }
            StatusDisplayer::displayProgressBar(file_count, int(file_names.size()));
            std::cout << std::endl << "Loading " << name << " is completed." << std::endl;
            std::cout << load_count << " files are loaded.\n" << std::endl;
        }

        /// @brief Load numeric text files and compress the data.
        /// @brief 数値テキストファイルのロードしてデータを圧縮する
        void loadNumericTextAndCompress(const std::vector<std::string>& file_names) noexcept {
            std::uint_least32_t file_count = 0;
            std::uint_least32_t load_count = 0;

            const Vector2 start_xyz_position = SimulationConstants::getInstance()->getStartArea() * pixel_size;
            const Vector2 end_xyz_position = SimulationConstants::getInstance()->getEndArea() * pixel_size;
            const Vector2 size = end_xyz_position - start_xyz_position;

            for(const auto& file_name : file_names) {
                StatusDisplayer::displayProgressBar(file_count, int(file_names.size()));

                Vector2 xyz_position;
                try {
                    xyz_position = getXAndYFromFileName(file_name);
                } catch (const std::exception&) {
                    PAXS_WARNING("File name is invalid: " + file_name);
                    ++file_count;
                    continue;
                }

                const Vector2 default_position = xyz_position * pixel_size / z_mag - start_xyz_position;
                if(default_position.x < 0 || default_position.y < 0 || default_position.x > size.x || default_position.y > size.y) {
                    ++file_count;
                    continue;
                }

                std::vector<std::string> file = File::readFile(file_name);
                if (file.size() == 0) {
                    PAXS_WARNING("File is empty: " + file_name);
                    ++file_count;
                    continue;
                }

                std::size_t z_mag_t = static_cast<std::size_t>(z_mag);
                for(std::size_t y = 0;y < file.size();y += z_mag_t) {
                    for(std::size_t x = 0;x < file[y].size();x += z_mag_t) {
                        bool is_contain_one = false;
                        for(std::size_t y_diff = 0;y_diff < z_mag;++y_diff) {
                            for(std::size_t x_diff = 0;x_diff < z_mag;++x_diff) {
                                if(file[y + y_diff][x + x_diff] == '1') {
                                    is_contain_one = true;
                                    break;
                                }
                            }
                            if(is_contain_one) break;
                        }
                        if(!is_contain_one) continue;
                        const Vector2 position = default_position + Vector2(static_cast<GridType>(x / z_mag), static_cast<GridType>(y / z_mag));
                        data[position.to<DataGridsType>()] = static_cast<DataType>('1');
                    }
                }
                ++file_count;
                ++load_count;
            }

            z_mag = 1;
            default_z = SimulationConstants::getInstance()->getZ();

            StatusDisplayer::displayProgressBar(file_count, int(file_names.size()));
            std::cout << std::endl << "Loading " << name << " is completed." << std::endl;
            std::cout << load_count << " files are loaded.\n" << std::endl;
        }

        /// @brief Get X and Y coordinates from file name.
        /// @brief ファイルの名前からX座標とY座標を取得
        Vector2 getXAndYFromFileName(const std::string& file_name) {
            const std::regex pattern(R"((\w+)_(\d+)_(\d+)_(\d+)\.(\w+))");
            std::smatch matches;

            if(std::regex_search(file_name, matches, pattern)) {
                return Vector2(std::stoi(matches[3]), std::stoi(matches[4]));
            }
            else {
                const std::string message = "File name is invalid: " + file_name;
                PAXS_ERROR(message);
                throw std::runtime_error(message);
            }
        }
    };
}

#endif // !PAX_SAPIENTICA_SIMULATION_DATA_HPP
