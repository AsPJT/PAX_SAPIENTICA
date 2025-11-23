/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIMULATION_CONFIG_DATA_HPP
#define PAX_SAPIENTICA_SIMULATION_CONFIG_DATA_HPP

#include <cmath>
#include <map>
#include <regex>
#include <stdexcept>

#include <PAX_SAPIENTICA/Core/Type/Vector2.hpp>
#include <PAX_SAPIENTICA/Core/Utility/StringUtils.hpp>
#include <PAX_SAPIENTICA/Geography/Terrain/Slope.hpp>
#include <PAX_SAPIENTICA/IO/File/FileSystem.hpp>
#include <PAX_SAPIENTICA/Interface/IProgressReporter.hpp>
#include <PAX_SAPIENTICA/Simulation/Config/SimulationConst.hpp>
#include <PAX_SAPIENTICA/Utility/Logger.hpp>

namespace paxs {

    /// @brief A class that holds data required for simulation.
    /// @brief シミュレーションに必要なデータを保持するクラス
    template <typename DataType>
    class Data {
    public:
        using Vector2 = paxs::Vector2<GridType>;

        /// @brief コンストラクタ
        /// @brief Constructor
        /// @param directory_path ディレクトリパス / Directory path
        /// @param name データの名前 / Data name
        /// @param default_z データのz値 / Data z value
        /// @param reporter 進捗報告インターフェース / Progress reporter interface
        explicit Data(const std::string& directory_path, const std::string name, const int default_z, IProgressReporter* reporter = nullptr) noexcept
            : name(name), default_z(default_z), progress_reporter_(reporter) {
            z_mag = std::pow(2, default_z - SimulationConstants::getInstance().getZ());
            GridType area_x = SimulationConstants::getInstance().getEndArea().x - SimulationConstants::getInstance().getStartArea().x + 1;
            column_size = static_cast<int>(area_x * pixel_size * z_mag);

            load(directory_path);
        }
        Data(const Data& other) noexcept
            : name(other.name),
            data(other.data),
            default_z(other.default_z),
            z_mag(other.z_mag),
            column_size(other.column_size),
            progress_reporter_(other.progress_reporter_) {}

        // Copy assignment operator
        constexpr Data& operator=(const Data& other) noexcept {
            if (this != &other) {
                name = other.name;
                data = other.data;
                default_z = other.default_z;
                z_mag = other.z_mag;
                column_size = other.column_size;
                progress_reporter_ = other.progress_reporter_;
            }
            return *this;
        }

        /// @brief Get the data of the specified position.
        /// @brief 指定した位置の値を取得する
        constexpr DataType getValue(const Vector2& position) const noexcept {
            const Vector2 converted_position = position * z_mag;
            auto itr = data.find(converted_position.to(DataGridsType{}));
            if(itr == data.end()) {
                return static_cast<DataType>(0);
            }
            return itr->second;
        }

        /// @brief Append all stored keys to the specified vector.
        /// @brief 登録済みのキーをベクターに追記する
        constexpr void appendKeys(std::vector<DataGridsType>& keys) const noexcept {
            keys.reserve(data.size());
            for (const auto& [key, value] : data) {
                keys.emplace_back(key);
            }
        }

        // データのz値を取得（セルの幅）
        [[nodiscard]] constexpr int getCellWidth() const noexcept {
            return (z_mag <= 0) ? 1 : int((1 / z_mag) + 0.5);
        }

    private:
        std::string name; // データの名前
        std::map<DataGridsType, DataType> data; // データ
        int default_z; // データのz値
        double z_mag; // シミュレーションのz値からデータのz値に変換するときの倍率
        int column_size; // シミュレーションの列数
        IProgressReporter* progress_reporter_ = nullptr; // 進捗報告インターフェース / Progress reporter interface

        /// @brief Load the file.
        /// @brief ファイルのロード
        void load(const std::string& directory_path) noexcept {
            std::vector<std::string> file_paths = FileSystem::getFilePaths(directory_path);

            if(file_paths.size() == 0) {
                PAXS_ERROR("No files are found: " + directory_path);
                return;
            }

            if (progress_reporter_) {
                progress_reporter_->startProgress(static_cast<int>(file_paths.size()),
                                                 "Loading " + name + " data (" + std::to_string(file_paths.size()) + " files found)...");
            }

            if(file_paths[0].find(".tsv") != std::string::npos) {
                loadNumericTSV(directory_path);
            } else if(file_paths[0].find(".txt") != std::string::npos) {
                if (z_mag > 1) loadNumericTextAndCompress(file_paths);
                else loadNumericText(file_paths);
            } else if(file_paths[0].find(".bin") != std::string::npos) {
                if constexpr (std::is_same<DataType, std::uint_least8_t>::value) {
                    loadBinary<paxs::Input8BitBinary>(directory_path);
                } else if constexpr (std::is_same<DataType, std::int_least16_t>::value) {
                    loadBinary<paxs::Input16BitBinary>(directory_path);
                }
            } else {
                PAXS_WARNING("File type is invalid: " + file_paths[0]);
            }
        }

        /// @brief Load binary files.
        /// @brief バイナリファイルのロード
        template <typename BinaryDataType>
        void loadBinary(const std::string& directory_path) noexcept {
            const Vector2 start_position = SimulationConstants::getInstance().getStartArea() * z_mag;
            const Vector2 end_position = SimulationConstants::getInstance().getEndArea() * z_mag;

            const Vector2 start_xyz_position = SimulationConstants::getInstance().getStartArea() * pixel_size * z_mag;

            std::uint_least32_t file_count = (end_position.x - start_position.x + 1) * (end_position.y - start_position.y + 1);
            std::uint_least32_t load_count = 0;

            for (GridType y = start_position.y; y <= end_position.y; ++y) {
                for (GridType x = start_position.x; x <= end_position.x; ++x) {
                    const std::string file_name = "zxy_" + std::to_string(default_z) + "_" + std::to_string(x) + "_" + std::to_string(y) + ".bin";

                    if (!FileSystem::exists(directory_path + file_name)) {
                        --file_count;
                        if (progress_reporter_) {
                            progress_reporter_->reportProgress(static_cast<float>(load_count) / static_cast<float>(file_count));
                        }
                        continue;
                    }

                    const Vector2 default_position = Vector2(x, y) * pixel_size - start_xyz_position;
                    BinaryDataType bi(directory_path + file_name);
                    bi.calc([&](const DataType value_, const std::uint_least8_t px, const std::uint_least8_t py) {
                        const Vector2 position = default_position + Vector2((GridType)px, (GridType)py);
                        data[position.to(DataGridsType{})] = value_;
                        }
                    );

                    ++load_count;
                    if (progress_reporter_) {
                        progress_reporter_->reportProgress(static_cast<float>(load_count) / static_cast<float>(file_count));
                    }
                }
            }

            if (progress_reporter_) {
                progress_reporter_->endProgress();
                progress_reporter_->reportProgress(1.0f, "Loading " + name + " is completed. " + std::to_string(load_count) + " files are loaded.");
            }
        }

        /// @brief Load binary files.
        /// @brief バイナリファイルのロード
        template <typename BinaryDataType>
        [[maybe_unused]]
        void loadBinary(const std::vector<std::string>& file_paths) noexcept {
            std::uint_least32_t file_count = 0;
            std::uint_least32_t load_count = 0;

            const Vector2 start_xyz_position = SimulationConstants::getInstance().getStartArea() * pixel_size * z_mag;
            const Vector2 end_xyz_position = SimulationConstants::getInstance().getEndArea() * pixel_size * z_mag;
            const Vector2 size = end_xyz_position - start_xyz_position;

            for(const auto& file_path : file_paths) {
                if (progress_reporter_) {
                    progress_reporter_->reportProgress(static_cast<float>(file_count) / static_cast<float>(file_paths.size()));
                }

                Vector2 xyz_position;
                try {
                    xyz_position = getXAndYFromFileName(file_path);
                } catch (const std::exception&) {
                    PAXS_WARNING("File name is invalid: " + file_path);
                    ++file_count;
                    continue;
                }

                const Vector2 default_position = xyz_position * pixel_size - start_xyz_position;
                if(default_position.x < 0 || default_position.y < 0 || default_position.x > size.x || default_position.y > size.y) {
                    ++file_count;
                    continue;
                }

                BinaryDataType bi(file_path, "");
                bi.calc([&](const DataType value_, const std::uint_least8_t x, const std::uint_least8_t y) {
                        const Vector2 position = default_position + Vector2((GridType)x, (GridType)y);
                        data[position.to(DataGridsType{})] = value_;
                    }
                );

                ++file_count;
                ++load_count;
            }
            if (progress_reporter_) {
                progress_reporter_->endProgress();
                progress_reporter_->reportProgress(1.0f, "Loading " + name + " is completed. " + std::to_string(load_count) + " files are loaded.");
            }
        }

        /// @brief Load numeric TSV files.
        /// @brief 数値TSVファイルのロード
        void loadNumericTSV(const std::string& directory_path) noexcept {
            const Vector2 start_position = SimulationConstants::getInstance().getStartArea() * z_mag;
            const Vector2 end_position = SimulationConstants::getInstance().getEndArea() * z_mag;

            const Vector2 start_xyz_position = SimulationConstants::getInstance().getStartArea() * pixel_size * z_mag;

            std::uint_least32_t file_count = (end_position.x - start_position.x + 1) * (end_position.y - start_position.y + 1);
            std::uint_least32_t load_count = 0;

            for (GridType y = start_position.y; y <= end_position.y; ++y) {
                for (GridType x = start_position.x; x <= end_position.x; ++x) {
                    const std::string file_name = "zxy_" + std::to_string(default_z) + "_" + std::to_string(x) + "_" + std::to_string(y) + ".tsv";

                    if (!FileSystem::exists(directory_path + file_name)) {
                        --file_count;
                        if (progress_reporter_) {
                            progress_reporter_->reportProgress(static_cast<float>(load_count) / static_cast<float>(file_count));
                        }
                        continue;
                    }

                    const std::string relative_path = directory_path + file_name;
                    std::vector<std::string> file = FileSystem::readFile(relative_path);
                    if (file.size() == 0) {
                        PAXS_WARNING("File is empty: " + relative_path);
                        ++file_count;
                        continue;
                    }


                    const Vector2 default_position = Vector2(x, y) * pixel_size - start_xyz_position;
                    for(std::size_t py = 0;py < file.size();++py) {
                        // タブ区切り
                        const std::vector<std::string> values = StringUtils::split(file[py], '\t');
                        for(std::size_t px = 0;px < values.size();++px) {
                            const Vector2 position = default_position + Vector2((GridType)px, (GridType)py);
                            if(values[px] == "") {
                                continue;
                            }
                            // T型に変換
                            if constexpr (std::is_same<DataType, std::uint_least8_t>::value || std::is_same<DataType, std::uint_least32_t>::value) {
                                auto value_opt = StringUtils::toInt(values[px]);
                                if (!value_opt) {
                                    PAXS_WARNING("File contains invalid integer value: " + relative_path + " (value: \"" + values[px] + "\")");
                                    continue;
                                }
                                int value = *value_opt;
                                if(value == 0) continue;
                                data[position.to(DataGridsType{})] = static_cast<DataType>(value);
                            } else if constexpr (std::is_same<DataType, float>::value) {
                                auto value_opt = StringUtils::toDouble(values[px]);
                                if (!value_opt) {
                                    PAXS_WARNING("File contains invalid float value: " + relative_path + " (value: \"" + values[px] + "\")");
                                    continue;
                                }
                                data[position.to(DataGridsType{})] = static_cast<DataType>(*value_opt);
                            }
                        }
                    }

                    ++load_count;
                    if (progress_reporter_) {
                        progress_reporter_->reportProgress(static_cast<float>(load_count) / static_cast<float>(file_count));
                    }
                }
            }

            if (progress_reporter_) {
                progress_reporter_->endProgress();
                progress_reporter_->reportProgress(1.0f, "Loading " + name + " is completed. " + std::to_string(load_count) + " files are loaded.");
            }
        }

        /// @brief Load numeric TSV files.
        /// @brief 数値TSVファイルのロード
        [[maybe_unused]]
        void loadNumericTSV(const std::vector<std::string>& file_paths) noexcept {
            std::uint_least32_t file_count = 0;
            std::uint_least32_t load_count = 0;

            const Vector2 start_xyz_position = SimulationConstants::getInstance().getStartArea() * pixel_size * z_mag;
            const Vector2 end_xyz_position = SimulationConstants::getInstance().getEndArea() * pixel_size * z_mag;
            const Vector2 size = end_xyz_position - start_xyz_position;

            for(const auto& file_path : file_paths) {
                if (progress_reporter_) {
                    progress_reporter_->reportProgress(static_cast<float>(file_count) / static_cast<float>(file_paths.size()));
                }

                Vector2 xyz_position;
                try {
                    xyz_position = getXAndYFromFileName(file_path);
                } catch (const std::exception&) {
                    PAXS_WARNING("File name is invalid: " + file_path);
                    ++file_count;
                    continue;
                }

                const Vector2 default_position = xyz_position * pixel_size - start_xyz_position;
                if(default_position.x < 0 || default_position.y < 0 || default_position.x > size.x || default_position.y > size.y) {
                    ++file_count;
                    continue;
                }

                std::vector<std::string> file = FileSystem::readFile(file_path);
                if (file.size() == 0) {
                    PAXS_WARNING("File is empty: " + file_path);
                    ++file_count;
                    continue;
                }

                for(std::size_t y = 0;y < file.size();++y) {
                    // タブ区切り
                    const std::vector<std::string> values = StringUtils::split(file[y], '\t');
                    for(std::size_t x = 0;x < values.size();++x) {
                        const Vector2 position = default_position + Vector2((GridType)x, (GridType)y);
                        if(values[x] == "") {
                            continue;
                        }
                        // T型に変換
                        if constexpr (std::is_same<DataType, std::uint_least8_t>::value || std::is_same<DataType, std::uint_least32_t>::value) {
                            auto value_opt = StringUtils::toInt(values[x]);
                            if (!value_opt) {
                                PAXS_WARNING("File contains invalid integer value: " + file_path + " (value: \"" + values[x] + "\")");
                                continue;
                            }
                            int value = *value_opt;
                            if(value == 0) continue;
                            data[position.to(DataType{})] = static_cast<DataType>(value);
                        } else if constexpr (std::is_same<DataType, float>::value) {
                            auto value_opt = StringUtils::toDouble(values[x]);
                            if (!value_opt) {
                                PAXS_WARNING("File contains invalid float value: " + file_path + " (value: \"" + values[x] + "\")");
                                continue;
                            }
                            data[position.to(DataType{})] = static_cast<DataType>(*value_opt);
                        }
                    }
                }
                ++file_count;
                ++load_count;
            }
            if (progress_reporter_) {
                progress_reporter_->endProgress();
                progress_reporter_->reportProgress(1.0f, "Loading " + name + " is completed. " + std::to_string(load_count) + " files are loaded.");
            }
        }

        /// @brief Load numeric text file.
        /// @brief 数値テキストファイルのロード
        void loadNumericText(const std::vector<std::string>& file_paths) noexcept {
            std::uint_least32_t file_count = 0;
            std::uint_least32_t load_count = 0;

            const Vector2 start_xyz_position = SimulationConstants::getInstance().getStartArea() * pixel_size * z_mag;
            const Vector2 end_xyz_position = SimulationConstants::getInstance().getEndArea() * pixel_size * z_mag;
            const Vector2 size = end_xyz_position - start_xyz_position;

            for(const auto& file_path : file_paths) {
                if (progress_reporter_) {
                    progress_reporter_->reportProgress(static_cast<float>(file_count) / static_cast<float>(file_paths.size()));
                }

                Vector2 xyz_position;
                try {
                    xyz_position = getXAndYFromFileName(file_path);
                } catch (const std::exception&) {
                    PAXS_WARNING("File name is invalid: " + file_path);
                    ++file_count;
                    continue;
                }

                const Vector2 default_position = xyz_position * pixel_size - start_xyz_position;
                if(default_position.x < 0 || default_position.y < 0 || default_position.x > size.x || default_position.y > size.y) {
                    ++file_count;
                    continue;
                }

                std::vector<std::string> file = FileSystem::readFile(file_path);
                if (file.size() == 0) {
                    PAXS_WARNING("File is empty: " + file_path);
                    ++file_count;
                    continue;
                }

                for(std::size_t y = 0;y < file.size();++y) {
                    for(std::size_t x = 0;x < file[y].size();++x) {
                        // T型に変換
                        if (file[y][x] == '0') continue;
                        const Vector2 position = default_position + Vector2((GridType)x, (GridType)y);
                        data[position.to(DataGridsType{})] = static_cast<DataType>(file[y][x]);
                    }
                }
                ++file_count;
                ++load_count;
            }
            if (progress_reporter_) {
                progress_reporter_->endProgress();
                progress_reporter_->reportProgress(1.0f, "Loading " + name + " is completed. " + std::to_string(load_count) + " files are loaded.");
            }
        }

        /// @brief Load numeric text files and compress the data.
        /// @brief 数値テキストファイルのロードしてデータを圧縮する
        void loadNumericTextAndCompress(const std::vector<std::string>& file_paths) noexcept {
            std::uint_least32_t file_count = 0;
            std::uint_least32_t load_count = 0;

            const Vector2 start_xyz_position = SimulationConstants::getInstance().getStartArea() * pixel_size;
            const Vector2 end_xyz_position = SimulationConstants::getInstance().getEndArea() * pixel_size;
            const Vector2 size = end_xyz_position - start_xyz_position;

            for(const auto& file_path : file_paths) {
                if (progress_reporter_) {
                    progress_reporter_->reportProgress(static_cast<float>(file_count) / static_cast<float>(file_paths.size()));
                }

                Vector2 xyz_position;
                try {
                    xyz_position = getXAndYFromFileName(file_path);
                } catch (const std::exception&) {
                    PAXS_WARNING("File name is invalid: " + file_path);
                    ++file_count;
                    continue;
                }

                const Vector2 default_position = xyz_position * pixel_size / z_mag - start_xyz_position;
                if(default_position.x < 0 || default_position.y < 0 || default_position.x > size.x || default_position.y > size.y) {
                    ++file_count;
                    continue;
                }

                std::vector<std::string> file = FileSystem::readFile(file_path);
                if (file.size() == 0) {
                    PAXS_WARNING("File is empty: " + file_path);
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
                        data[position.to(DataGridsType{})] = static_cast<DataType>('1');
                    }
                }
                ++file_count;
                ++load_count;
            }

            z_mag = 1;
            default_z = SimulationConstants::getInstance().getZ();

            if (progress_reporter_) {
                progress_reporter_->endProgress();
                progress_reporter_->reportProgress(1.0f, "Loading " + name + " is completed. " + std::to_string(load_count) + " files are loaded.");
            }
        }

        /// @brief Get X and Y coordinates from file name.
        /// @brief ファイルの名前からX座標とY座標を取得
        Vector2 getXAndYFromFileName(const std::string& file_path) {
            std::string filename = FileSystem::getFilename(file_path);

            const std::regex pattern(R"((\w+)_(\d+)_(\d+)_(\d+)\.(\w+))");
            std::smatch matches;

            if(std::regex_search(filename, matches, pattern)) {
                return {
                    StringUtils::safeStoi(matches[3].str(), 0, true),
                    StringUtils::safeStoi(matches[4].str(), 0, true)
                };
            }

            const std::string message = "File name is invalid: " + file_path;
            PAXS_ERROR(message);
            throw std::runtime_error(message);
        }
    };
}

#endif // !PAX_SAPIENTICA_SIMULATION_CONFIG_DATA_HPP
