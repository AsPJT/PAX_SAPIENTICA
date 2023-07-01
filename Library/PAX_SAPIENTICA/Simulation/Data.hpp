/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIMULATION_DATA_HPP
#define PAX_SAPIENTICA_SIMULATION_DATA_HPP

/*##########################################################################################

##########################################################################################*/

#include <iostream>
#include <map>

#include <PAX_SAPIENTICA/File.hpp>
#include <PAX_SAPIENTICA/Simulation/SimulationConst.hpp>
#include <PAX_SAPIENTICA/StatusLogger.hpp>
#include <PAX_SAPIENTICA/StringExtensions.hpp>
#include <PAX_SAPIENTICA/Type/Vector2.hpp>

namespace paxs {
    
    /// @brief Dataクラスのデータ型
    enum class DataType {
        u8,
        u32,
        f32
    };

    /// @brief シミュレーションに必要なデータを保持するクラス
    /// @tparam T 読み込むデータの型
    template <typename T>
    class Data {
    public:
        using Vector2 = paxs::Vector2<int>;
        Data(const std::string& file_path, const std::string name, const Vector2& start_position, const Vector2& end_position,  const int default_z, const int pj_z, const  DataType data_type) : name(name), start_position(start_position), end_position(end_position), default_z(default_z), pj_z(pj_z), data_type(data_type) {
            z_mag = std::pow(2, default_z - pj_z);
            load(file_path);
        }
        Data(const Data& other)
            : start_position(other.start_position),
            end_position(other.end_position),
            name(other.name),
            data(other.data),
            default_z(other.default_z),
            pj_z(other.pj_z),
            z_mag(other.z_mag),
            data_type(other.data_type) {}

        // Copy assignment operator
        Data& operator=(const Data& other) {
            if (this != &other) {
                start_position = other.start_position;
                end_position = other.end_position;
                name = other.name;
                data = other.data;
                default_z = other.default_z;
                pj_z = other.pj_z;
            }
            return *this;
        }

        /// @brief 指定した位置の値を取得する
        /// @param position シミュレーションのZ値に対する座標
        /// @return 指定した位置の値
        T getValue(const Vector2& position) const {
            Vector2 converted_position = position * z_mag;
            auto itr = data.find(converted_position);
            if(itr == data.end()) {
                // TODO: この0はT型のデフォルト値にしたい
                return 0;
            }
            return itr->second;
        }
    private:
        Vector2 start_position; // シミュレーションの左上の座標
        Vector2 end_position; // シミュレーションの右下の座標
        std::string name; // データの名前
        std::map<Vector2, T> data; // データ
        int default_z; // データのz値
        int pj_z; // シミュレーションのz値
        double z_mag; // シミュレーションのz値からデータのz値に変換するときの倍率
        DataType data_type; // データの型

        // ファイルのロード
        void load(const std::string& file_path) {
            std::cout << "Loading " << name << " data..." << std::endl;
            const std::vector<std::string> file_names = File::getFileNames(file_path);
            std::cout << file_names.size() << " files are found." << std::endl; 

            if(file_names.size() == 0) {
                std::cout << "No files are found." << std::endl;
                std::exit(1);
            }

            if(file_names[0].find(".tsv") != std::string::npos) {
                loadNumericTSV(file_names);
            } else if(file_names[0].find(".txt") != std::string::npos) {
                loadNumericText(file_names);
            } else {
                std::cout << "File format is not supported." << std::endl;
                std::exit(1);
            }
        }

        // 数値TSVファイルのロード
        void loadNumericTSV(const std::vector<std::string>& file_names) {
            unsigned int file_count = 0;
            int load_count = 0;

            for(const auto& file_name : file_names) {
                StatusLogger::displayProgressBar(file_count, int(file_names.size()));

                Vector2 xyz_position = getXAndYFromFileName(file_name);
                Vector2 default_position = (xyz_position * pixel_size - start_position * (pixel_size * z_mag));
                if(default_position.x < 0 || default_position.y < 0 || default_position.x > (end_position.x - start_position.x) * pixel_size || default_position.y > (end_position.y - start_position.y) * pixel_size) {
                    ++file_count;
                    continue;
                }
                std::vector<std::string> file = File::readFile(file_name);
                for(std::size_t y = 0;y < file.size();++y) {
                    // タブ区切り
                    std::vector<std::string> values = StringExtensions::split(file[y], '\t');
                    for(std::size_t x = 0;x < values.size();++x) {
                        Vector2 position = default_position + Vector2((int)x, (int)y);
                        if(values[x] == "") {
                            continue;
                        }
                        // T型に変換
                        try {
                            if(data_type == DataType::u8 || data_type == DataType::u32)
                                data[position] = static_cast<T>(std::stoi(values[x]));
                            else if(data_type == DataType::f32)
                                data[position] = static_cast<T>(std::stod(values[x]));
                        } catch (const std::invalid_argument& ia) {
                            // str is not convertible to double
                            continue;
                        } catch (const std::out_of_range& oor) {
                            // str is out of range for a double
                            continue;
                        }
                        
                    }
                }
                ++file_count;
                ++load_count;
            }
            StatusLogger::displayProgressBar(file_count, int(file_names.size()));
            std::cout << std::endl << "Loading " << name << " is completed." << std::endl;
            std::cout << load_count << " files are loaded.\n" << std::endl;
        }

        // 数値ファイルのロード
        void loadNumericText(const std::vector<std::string>& file_names) {
            unsigned int file_count = 0;
            int load_count = 0;

            for(const auto& file_name : file_names) {
                StatusLogger::displayProgressBar(file_count, int(file_names.size()));

                Vector2 xyz_position = getXAndYFromFileName(file_name);
                Vector2 default_position = (xyz_position * pixel_size - start_position * (pixel_size * z_mag));
                if(default_position.x < 0 || default_position.y < 0 || default_position.x > (end_position.x - start_position.x) * pixel_size || default_position.y > (end_position.y - start_position.y) * pixel_size) {
                    ++file_count;
                    continue;
                }
                std::vector<std::string> file = File::readFile(file_name);
                for(std::size_t y = 0;y < file.size();++y) {
                    for(std::size_t x = 0;x < file[y].size();++x) {
                        Vector2 position = default_position + Vector2((int)x, (int)y);
                        // T型に変換
                        data[position] = static_cast<T>(file[y][x]);
                    }
                }
                ++file_count;
                ++load_count;
            }
            StatusLogger::displayProgressBar(file_count, int(file_names.size()));
            std::cout << std::endl << "Loading " << name << " is completed." << std::endl;
            std::cout << load_count << " files are loaded.\n" << std::endl;
        }

        // ファイルの名前からX座標とY座標を取得
        Vector2 getXAndYFromFileName(const std::string& file_name) {
            std::regex pattern(R"((\w+)_(\d+)_(\d+)_(\d+)\.(\w+))");
            std::smatch matches;

            if(std::regex_search(file_name, matches, pattern)) {
                return Vector2(std::stoi(matches[3]), std::stoi(matches[4]));
            }
            else {
                std::cerr << "Error: The format of " << file_name << " is incorrect." << std::endl;
                std::exit(1);
            }
        }
    };
}

#endif // !PAX_SAPIENTICA_SIMULATION_DATA_HPP
